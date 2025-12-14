#if !defined _chandlingsvr_included
    #endinput
#endif

#if defined _chandlingsvr_handling_manager_included
    #endinput
#endif
#define _chandlingsvr_handling_manager_included

#define CHECK_TYPE(%0,%1) \
    if (GetHandlingAttribType((%0)) != (%1)) \
    { \
        printf("[chandling] Invalid type (%d) specified for attribute %d", _:(%0), _:(%1)); \
        return false; \
    }

enum E_HANDLING_MOD_DATA
{
    E_HANDLING_MOD_VALUE,
    CHandlingAttribType:E_HANDLING_MOD_TYPE
}

new ModelHandlingData[MAX_VEHICLE_MODELS][E_HANDLING_DATA];
new Map:ModelHandlingModMap[MAX_VEHICLE_MODELS];

new VehicleHandlingData[MAX_VEHICLES][E_HANDLING_DATA];
new Map:VehicleHandlingModMap[MAX_VEHICLES];
new bool:VehicleUsesModelHandling[MAX_VEHICLES char];

new List:OutgoingVehicleMods;
new List:OutgoingModelMods;

/*
 *  INTERNAL FUNCTIONS
*/
static __WriteHandlingEntryToBitStream(BitStream:bs, Map:map)
{
    BS_WriteUint8(bs, map_size(map));

    for (new Iter:i = map_iter(map), mod[E_HANDLING_MOD_DATA]; iter_inside(i); iter_move_next(i))
    {
        BS_WriteUint8(bs, iter_get_key(i)); // attribute

        #pragma warning push
        #pragma warning disable 229

        iter_get_arr(i, mod);

        #pragma warning pop

        switch (mod[E_HANDLING_MOD_TYPE])
        {
            case TYPE_BYTE:
                BS_WriteValue(bs, PR_UINT8, mod[E_HANDLING_MOD_VALUE]);
            case
                TYPE_UINT,
                TYPE_FLAG:
                BS_WriteValue(bs, PR_UINT32, mod[E_HANDLING_MOD_VALUE]);
            case TYPE_FLOAT:
                BS_WriteValue(bs, PR_FLOAT, Float:mod[E_HANDLING_MOD_VALUE]);
            default:
                {}
        }
    }
}

// dis funcion no use no no
static stock __addMod(handling[E_HANDLING_DATA], Map:map, CHandlingAttrib:attribute, const mod[E_HANDLING_MOD_DATA])
{
    #pragma warning push
    #pragma warning disable 229

    map_set_arr(map, _:attribute, mod);

    #pragma warning pop

    new E_HANDLING_DATA:type = GetHandlingAttribEnum(attribute);
    /* write the value to the handling data so we can Get it later on */
    handling[type] = mod[E_HANDLING_MOD_VALUE];
}

// dis use
static stock bool:__AddModelHandlingMod(modelid, CHandlingAttrib:attribute, const mod[E_HANDLING_MOD_DATA])
{
    if (!IS_VALID_VEHICLE_MODEL(modelid))
    {
        return false;
    }
    __addMod(ModelHandlingData[VEHICLE_MODEL_INDEX(modelid)], ModelHandlingModMap[VEHICLE_MODEL_INDEX(modelid)], attribute, mod);
    
    list_add(OutgoingModelMods, modelid);
    return true;
}

static stock bool:__AddVehicleHandlingMod(vehicleid, CHandlingAttrib:attribute, const mod[E_HANDLING_MOD_DATA])
{
    if (!IS_VALID_VEHICLEID(vehicleid))
    {
        return false;
    }

    // copy the handling of the model & apply the changed value
    if (VehicleUsesModelHandling{vehicleid})
    {
        new const modelid = GetVehicleModel(vehicleid);

        VehicleUsesModelHandling{vehicleid} = false;
        VehicleHandlingData[vehicleid] = ModelHandlingData[VEHICLE_MODEL_INDEX(modelid)];
        VehicleHandlingModMap[vehicleid] = map_clone(ModelHandlingModMap[VEHICLE_MODEL_INDEX(modelid)]);
    }
    __addMod(VehicleHandlingData[vehicleid], VehicleHandlingModMap[vehicleid], attribute, mod);

    list_add(OutgoingVehicleMods, vehicleid);
    return true;
}

forward ProcessCHandlingTick();
public ProcessCHandlingTick()
{
    if (list_size(OutgoingVehicleMods))
    {
        for (new Iter:i = list_iter(OutgoingVehicleMods), vehicleid, BitStream:p; iter_inside(i); iter_move_next(i))
        {
            vehicleid = iter_get(i);

            if (!IsValidVehicle(vehicleid) || VehicleUsesModelHandling{vehicleid})
            {
                continue;
            }

            p = CHandlingActionPacket(ACTION_SET_VEHICLE_HANDLING);
            BS_WriteUint16(p, vehicleid);
            __WriteHandlingEntryToBitStream(p, VehicleHandlingModMap[vehicleid]);

            PR_SendPacket(p, -1, PR_HIGH_PRIORITY, PR_RELIABLE_ORDERED, 0);
            BS_Delete(p);
        }

        list_clear(OutgoingVehicleMods);
    }

    if (list_size(OutgoingModelMods))
    {
        for (new Iter:i = list_iter(OutgoingVehicleMods), modelid, BitStream:p; iter_inside(i); iter_move_next(i))
        {
            modelid = iter_get(i);

            if (!IS_VALID_VEHICLE_MODEL(modelid) || !map_size(ModelHandlingModMap[VEHICLE_MODEL_INDEX(modelid)]))
            {
                continue;
            }

            p = CHandlingActionPacket(ACTION_SET_MODEL_HANDLING);
            BS_WriteUint16(p, modelid);
            __WriteHandlingEntryToBitStream(p, ModelHandlingModMap[VEHICLE_MODEL_INDEX(modelid)]);

            PR_SendPacket(p, -1, PR_HIGH_PRIORITY, PR_RELIABLE_ORDERED, 0);
            BS_Delete(p);
        }

        list_clear(OutgoingModelMods);
    }
}

InitializeModelHandlings()
{
    for (new i; i < MAX_VEHICLE_MODELS; ++ i)
    {
        ModelHandlingModMap[i] = map_new();
    }
    for (new i; i < MAX_VEHICLES; ++ i)
    {
        VehicleHandlingModMap[i] = map_new();
    }

    OutgoingVehicleMods = list_new();
    OutgoingModelMods = list_new();

    SetTimer("ProcessCHandlingTick", 0, true);
}

forward HandlingMgr_OnCreateVehicle(vehicleid);
public HandlingMgr_OnCreateVehicle(vehicleid)
{
    ResetVehicleHandling(vehicleid);
}

HandlingMgr_OnPlayerConnect(playerid)
{
    if (!IS_VALID_PLAYERID(playerid) || !IsPlayerUsingCHandling(playerid))
    {
        return;
    }

    for (new model, BitStream:p; model < MAX_VEHICLE_MODELS; ++ model)
    {
        if (map_size(ModelHandlingModMap[model]))
        {
            p = CHandlingActionPacket(ACTION_SET_MODEL_HANDLING);

            BS_WriteUint16(p, model + 400);

            __WriteHandlingEntryToBitStream(p, ModelHandlingModMap[model]);

            PR_SendPacket(p, playerid, PR_HIGH_PRIORITY, PR_RELIABLE_ORDERED, 0);
            BS_Delete(p);
        }
    }
}

HandlingMgr_OnVehicleStreamIn(vehicleid, forplayerid)
{
    if (!IsValidVehicle(vehicleid) || !map_size(VehicleHandlingModMap[vehicleid]) || !IS_VALID_PLAYERID(forplayerid) || !IsPlayerUsingCHandling(forplayerid))
    {
        return;
    }

    new BitStream:p = CHandlingActionPacket(ACTION_SET_VEHICLE_HANDLING);
    BS_WriteUint16(p, vehicleid);

    __WriteHandlingEntryToBitStream(p, VehicleHandlingModMap[vehicleid]);

    PR_SendPacket(p, forplayerid, PR_HIGH_PRIORITY, PR_RELIABLE_ORDERED, 0);
    BS_Delete(p);
}

/* 
 * Resets handling of specified vehicle model to it's original default one
 * NOTE: This doesn't reset vehicles that have modified the model handling, these use their own (older) copy
*/
stock ResetModelHandling(modelid)
{
    new modelIndex = VEHICLE_MODEL_INDEX(modelid);
    if (!IS_VALID_VEHICLE_MODEL(modelid))
    {
        return 0;
    }
    
    map_clear(ModelHandlingModMap[modelIndex]);

    copyDefaultModelHandling(modelid, ModelHandlingData[modelIndex]);

    new BitStream:p = CHandlingActionPacket(ACTION_RESET_MODEL);
    BS_WriteUint16(p, modelid);
    // this needs to be announced to every player
    PR_SendPacket(p, -1, HIGH_PRIORITY, PR_RELIABLE_ORDERED, 0);
    return 1;
}

/* 
 * Resets the handling of specified vehicle to it's model handling (only if needed)
 * sendToPlayers is true by default, set it to false only when resetting model handling
*/
ResetVehicleHandling(vehicleid/*, bool:sendToPlayers = true*/)
{
    new const bool:sendToPlayers = true;

    new modelid = GetVehicleModel(vehicleid);
    if (!IS_VALID_VEHICLEID(vehicleid) || !IS_VALID_VEHICLE_MODEL(modelid))
    {
        return 0;
    }

    map_clear(VehicleHandlingModMap[vehicleid]);
    VehicleHandlingData[vehicleid] = ModelHandlingData[VEHICLE_MODEL_INDEX(modelid)];
    VehicleUsesModelHandling{vehicleid} = true;

    // use sendToPlayers = false when resetting model handling, client code can handle it on his own
    if (sendToPlayers)
    {
        new BitStream:p = CHandlingActionPacket(ACTION_RESET_VEHICLE);
        BS_WriteUint16(p, vehicleid);

        PR_SendPacket(p, -1, PR_HIGH_PRIORITY, PR_RELIABLE_ORDERED, 0);
        BS_Delete(p);
    }
    return 1;
}

/* SET HANDLING FUNCTIONS */

stock SetVehicleHandlingFloat(vehicleid, CHandlingAttrib:attribute, Float:value)
{
    if (!IsValidVehicle(vehicleid) || !CanSetHandlingAttrib(attribute))
    {
        return 0;
    }
    CHECK_TYPE(attribute, TYPE_FLOAT)

    if (!IsValidHandlingFloatValue(attribute, value))
    {
        return 0;
    }

    new mod[E_HANDLING_MOD_DATA];
    mod[E_HANDLING_MOD_VALUE] = _:value;
    mod[E_HANDLING_MOD_TYPE] = TYPE_FLOAT;

    return __AddVehicleHandlingMod(vehicleid, attribute, mod);
}

stock SetVehicleHandlingInt(vehicleid, CHandlingAttrib:attribute, value)
{
    if (!IsValidVehicle(vehicleid) || !CanSetHandlingAttrib(attribute)) // no validation checking for unsigned integers
    {
        return 0;
    }

    new CHandlingAttribType:type = GetHandlingAttribType(attribute);
    switch (type)
    {
        case TYPE_BYTE:
        {
            if (!IsValidHandlingIntValue(attribute, value))
            {
                return 0;
            }
        }
        case TYPE_UINT, TYPE_FLAG:
            {}
        default:
            return 0;
    }

    new mod[E_HANDLING_MOD_DATA];
    mod[E_HANDLING_MOD_VALUE] = value;
    mod[E_HANDLING_MOD_TYPE] = type == TYPE_FLAG ? TYPE_UINT : type;
    return __AddVehicleHandlingMod(vehicleid, attribute, mod);
}

stock SetModelHandlingFloat(modelid, CHandlingAttrib:attribute, Float:value)
{
    if (!IS_VALID_VEHICLE_MODEL(modelid) || !CanSetHandlingAttrib(attribute))
    {
        return 0;
    }

    CHECK_TYPE(attribute, TYPE_FLOAT)

    if (!IsValidHandlingValue(attribute, value))
    {
        return 0;
    }

    new mod[E_HANDLING_MOD_DATA];
    mod[E_HANDLING_MOD_VALUE] = _:value;
    mod[E_HANDLING_MOD_TYPE] = TYPE_FLOAT;
    return __AddModelHandlingMod(modelid, attribute, mod);
}

stock SetModelHandlingInt(modelid, CHandlingAttrib:attribute, value)
{
    if (!IS_VALID_VEHICLE_MODEL(modelid) || !CanSetHandlingAttrib(attribute))
    {
        return 0;
    }

    new CHandlingAttribType:type = GetHandlingAttribType(attribute);
    switch (type)
    {
        case TYPE_BYTE:
        {
            if (!IsValidHandlingIntValue(attribute, value))
            {
                return 0;
            }
        }
        case TYPE_UINT, TYPE_FLAG:
            {}
        default:
            return 0;
    }

    new mod[E_HANDLING_MOD_DATA];
    mod[E_HANDLING_MOD_VALUE] = value;
    mod[E_HANDLING_MOD_TYPE] = type == TYPE_FLAG ? TYPE_UINT : type;
    return __AddModelHandlingMod(modelid, attribute, mod);
}

/* GET */

stock GetVehicleHandlingFloat(vehicleid, CHandlingAttrib:attribute, &Float:result)
{
	if (!IsValidVehicle(vehicleid))
    {
		return 0;
    }
	CHECK_TYPE(attribute, TYPE_FLOAT)

    if (VehicleUsesModelHandling{vehicleid})
    {
        result = Float:ModelHandlingData[VEHICLE_MODEL_INDEX(GetVehicleModel(vehicleid))][GetHandlingAttribEnum(attribute)];
    }
    else
    {
        result = Float:VehicleHandlingData[vehicleid][GetHandlingAttribEnum(attribute)];
    }
	return true;
}

stock GetVehicleHandlingInt(vehicleid, CHandlingAttrib:attribute, &result)
{
	if (!IsValidVehicle(vehicleid))
    {
		return 0;
    }
	new CHandlingAttribType:type = GetHandlingAttribType(attribute);

    switch (type)
    {
        case TYPE_UINT, TYPE_FLAG, TYPE_BYTE:
            {}
        default:
            return false;
    }

    if (VehicleUsesModelHandling{vehicleid})
    {
        result = ModelHandlingData[VEHICLE_MODEL_INDEX(GetVehicleModel(vehicleid))][GetHandlingAttribEnum(attribute)];
    }
    else
    {
        result = VehicleHandlingData[vehicleid][GetHandlingAttribEnum(attribute)];
    }
	return true;
}

stock GetModelHandlingFloat(modelid, CHandlingAttrib:attribute, &Float:result)
{
	if (!IS_VALID_VEHICLE_MODEL(modelid))
    {
		return false;
    }
	CHECK_TYPE(attribute, TYPE_FLOAT)

    result = Float:ModelHandlingData[VEHICLE_MODEL_INDEX(modelid)][GetHandlingAttribEnum(attribute)];
	return true;
}

stock GetModelHandlingInt(modelid, CHandlingAttrib:attribute, &result)
{
	if (!IS_VALID_VEHICLE_MODEL(modelid))
    {
		return 0;
    }
	new CHandlingAttribType:type = GetHandlingAttribType(attribute);

    switch (type)
    {
        case TYPE_UINT, TYPE_FLAG, TYPE_BYTE:
            {}
        default:
            return false;
    }

    result = ModelHandlingData[VEHICLE_MODEL_INDEX(modelid)][GetHandlingAttribEnum(attribute)];
	return true;
}

stock GetDefaultHandlingFloat(modelid, CHandlingAttrib:attribute, &Float:value)
{
	if (!IS_VALID_VEHICLE_MODEL(modelid))
    {
		return 0;
    }

	CHECK_TYPE(attribute, TYPE_FLOAT)


    new handl[E_HANDLING_DATA];
    copyDefaultModelHandling(modelid, handl);
    value = Float:handl[GetHandlingAttribEnum(attribute)];
	return 1;
}

stock GetDefaultHandlingInt(modelid, CHandlingAttrib:attribute, &value)
{
	if (!IS_VALID_VEHICLE_MODEL(modelid))
    {
		return 0;
    }

    new CHandlingAttribType:type = GetHandlingAttribType(attribute);

    switch (type)
    {
        case TYPE_UINT, TYPE_FLAG, TYPE_BYTE:
            {}
        default:
            return false;
    }

    new handl[E_HANDLING_DATA];
    copyDefaultModelHandling(modelid, handl);
    value = handl[GetHandlingAttribEnum(attribute)];
	return 1;
}
