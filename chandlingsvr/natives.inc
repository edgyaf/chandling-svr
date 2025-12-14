#if !defined _chandlingsvr_included
    #endinput
#endif

#if defined _chandlingsvr_natives_included
    #endinput
#endif
#define _chandlingsvr_natives_included

stock CHandling_CreateVehicle(modelid, Float:spawnX, Float:spawnY, Float:spawnZ, Float:angle, colour1, colour2, respawnDelay, bool:addSiren = false)
{
    new const vehicleid = CreateVehicle(modelid, spawnX, spawnY, spawnZ, angle, colour1, colour2, respawnDelay, addSiren);

    CallRemoteFunction("HandlingMgr_OnCreateVehicle", "i", vehicleid);
    return vehicleid;
}
#if defined _ALS_CreateVehicle
    #undef CreateVehicle
#else
    #define _ALS_CreateVehicle
#endif
#define CreateVehicle CHandling_CreateVehicle

stock CHandling_AddStaticVehicle(modelid, Float:spawnX, Float:spawnY, Float:spawnZ, Float:angle, colour1, colour2)
{
    new const vehicleid = AddStaticVehicle(modelid, spawnX, spawnY, spawnZ, angle, colour1, colour2);

    CallRemoteFunction("HandlingMgr_OnCreateVehicle", "i", vehicleid);
    return vehicleid;
}
#if defined _ALS_AddStaticVehicle
    #undef AddStaticVehicle
#else
    #define _ALS_AddStaticVehicle
#endif
#define AddStaticVehicle CHandling_AddStaticVehicle

stock CHandling_AddStaticVehicleEx(modelid, Float:spawnX, Float:spawnY, Float:spawnZ, Float:angle, colour1, colour2, respawnDelay, bool:addSiren = false)
{
    new const vehicleid = AddStaticVehicleEx(modelid, spawnX, spawnY, spawnZ, angle, colour1, colour2, respawnDelay, addSiren);

    CallRemoteFunction("HandlingMgr_OnCreateVehicle", "i", vehicleid);
    return vehicleid;
}
#if defined _ALS_AddStaticVehicleEx
    #undef AddStaticVehicleEx
#else
    #define _ALS_AddStaticVehicleEx
#endif
#define AddStaticVehicleEx CHandling_AddStaticVehicleEx
