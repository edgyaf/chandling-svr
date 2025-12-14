#if !defined _chandlingsvr_included
    #endinput
#endif

#if defined _chandlingsvr_handling_enum_included
    #endinput
#endif
#define _chandlingsvr_handling_enum_included
/* game structures */

enum E_HANDLING_DATA
{
    E_HANDLING_INDEX,
    Float:E_HANDLING_MASS, // 1.0 to 50000.0
    Float:E_HANDLING_FIELD_8,
    Float:E_HANDLING_TURN_MASS,
    Float:E_HANDLING_DRAG_MULT,

    Float:E_HANDLING_CENTRE_OF_MASS_X, // 1.0 to 50000.0
    Float:E_HANDLING_CENTRE_OF_MASS_Y, // 1.0 to 50000.0
    Float:E_HANDLING_CENTRE_OF_MASS_Z, // 1.0 to 50000.0

    E_HANDLING_PERCENT_SUBMERGED, // 10 to 120 (> 100% vehicle sinks)
    Float:E_HANDLING_BUOYANCY_CONSTANT,
    Float:E_HANDLING_TRACTION_MULTIPLIER, // 0.5 to 2.0

    Float:E_HANDLING_TRANSMISSION_GEARS[6 * 3],

    E_HANDLING_TRANSMISSION_DRIVE_TYPE, // F/R/4
    E_HANDLING_TRANSMISSION_ENGINE_TYPE, // P/D/E
    E_HANDLING_TRANSMISSION_NUMBER_OF_GEARS, // 1 to 6
    E_HANDLING_TRANSMISSION_FIELD_48,
    E_HANDLING_TRANSMISSION_HANDLING_FLAGS,
    Float:E_HANDLING_TRANSMISSION_ENGINE_ACCELERATION, // 0.1 to 10.0
    Float:E_HANDLING_TRANSMISSION_ENGINE_INERTIA, // 0.0 to 50.0
    Float:E_HANDLING_TRANSMISSION_MAX_GEAR_VELOCITY, // 5.0 to 150.0
    E_HANDLING_TRANSMISSION_FIELD_5C,
    Float:E_HANDLING_TRANSMISSION_MIN_GEAR_VELOCITY,
    Float:E_HANDLING_TRANSMISSION_CURRENT_SPEED,

    Float:E_HANDLING_BRAKE_DECELERATION, // 0.1 to 10.0
    Float:E_HANDLING_BRAKE_BIAS, // 0.0 > x > 1.0
    E_HANDLING_ABS, // 0/1
    E_HANDLING_FIELD_9D,
    E_HANDLING_FIELD_9E,
    E_HANDLING_FIELD_9F,
    Float:E_HANDLING_STEERING_LOCK, // 10.0 to 40.0
    Float:E_HANDLING_TRACTION_LOSS,
    Float:E_HANDLING_TRACTION_BIAS,
    Float:E_HANDLING_SUSPENSION_FORCE_LEVEL, // not [L/M/H]
    Float:E_HANDLING_SUSPENSION_DAMPING_LEVEL, // not [L/M/H]
    Float:E_HANDLING_SUSPENSION_HIGH_SPD_COM_DAMP, // often zero - 200.0 or more for bouncy vehicles
    Float:E_HANDLING_SUSPENSION_UPPER_LIMIT,
    Float:E_HANDLING_SUSPENSION_LOWER_LIMIT,
    Float:E_HANDLING_SUSPENSION_BIAS_BETWEEN_FRONT_AND_REAR,
    Float:E_HANDLING_SUSPENSION_ANTI_DIVE_MULTIPLIER,
    Float:E_HANDLING_COLLISION_DAMAGE_MULTIPLIER, // 0.2 to 5.0,

    VehicleHandlingModelFlag:E_HANDLING_MODEL_FLAGS,

    VehicleHandlingFlag:E_HANDLING_FLAGS,
    Float:E_HANDLING_SEAT_OFFSET_DISTANCE,
    E_HANDLING_MONETARY_VALUE, // 1 to 100000
    CHandlingLights:E_HANDLING_FRONT_LIGHTS,
    CHandlingLights:E_HANDLING_REAR_LIGHTS,
    E_HANDLING_ANIM_GROUP
}

stock CHandlingAttribType:GetHandlingAttribType(CHandlingAttrib:attribute)
{
    switch (attribute)
	{
        case
            HANDL_FMASS,
            HANDL_FTURNMASS,
            HANDL_FDRAGMULTIPLIER,
            HANDL_CENTREOFMASS_X,
            HANDL_CENTREOFMASS_Y,
            HANDL_CENTREOFMASS_Z,
            HANDL_FTRACTIONMULTIPLIER,
            HANDL_FTRACTIONLOSS,
            HANDL_FTRACTIONBIAS,
            HANDL_TR_FMAXVELOCITY,
            HANDL_TR_FENGINEACCELERATION,
            HANDL_TR_FENGINEINERTIA,
            HANDL_FBRAKEDECELERATION,
            HANDL_FBRAKEBIAS,
            HANDL_FSTEERINGLOCK,
            HANDL_FSUSPENSIONFORCELEVEL,
            HANDL_FSUSPENSIONDAMPINGLEVEL,
            HANDL_FSUSPENSIONHIGHSPDCOMDAMP,
            HANDL_FSUSPENSIONUPPERLIMIT,
            HANDL_FSUSPENSIONLOWERLIMIT,
            HANDL_FSUSPENSIONBIAS,
            HANDL_FSUSPENSIONANTIDIVEMULT,
            HANDL_FSEATOFFSETDISTANCE,
            HANDL_FCOLLISIONDAMAGEMULT:
            return TYPE_FLOAT;
        case
            HANDL_NPERCENTSUBMERGED,
            HANDL_ANIMGROUP,
            HANDL_FRONTLIGHTS,
            HANDL_REARLIGHTS,
            HANDL_TR_NDRIVETYPE,
            HANDL_TR_NENGINETYPE,
            HANDL_TR_NNUMBEROFGEARS,
            HANDL_BABS:
            return TYPE_BYTE;
        case
            HANDL_IDENTIFIER,
            HANDL_NMONETARYVALUE:
            return TYPE_UINT;
        case
            HANDL_HANDLINGFLAGS,
            HANDL_MODELFLAGS:
            return TYPE_FLAG;
	}
	return TYPE_NONE;
}

stock bool:CanSetHandlingAttrib(CHandlingAttrib:attribute)
{
	switch (attribute)
	{
        case
            HANDL_IDENTIFIER,
            HANDL_ANIMGROUP,
            HANDL_NMONETARYVALUE:
            return false;
	}
	return true;
}

stock bool:IsValidHandlingFloatValue(CHandlingAttrib:attribute, Float:value)
{
	if (GetHandlingAttribType(attribute) != TYPE_FLOAT)
    {
		return false;
    }

	switch (attribute)
	{
        case HANDL_FMASS:
            return 1.0 <= value <= 50000.0;
        case HANDL_FTURNMASS:
            return 0.0 <= value <= 50000.0;
        case HANDL_FDRAGMULTIPLIER:
            return -200.0 <= value <= 200.0;
        case
            HANDL_CENTREOFMASS_X,
            HANDL_CENTREOFMASS_Y,
            HANDL_CENTREOFMASS_Z:
            return -10.0 <= value <= 10.0;
        case HANDL_FTRACTIONMULTIPLIER:
            return -100.0 <= value <= 100.0;
        case HANDL_FTRACTIONLOSS:
            return 0.0 <= value <= 100.0;
        case HANDL_FTRACTIONBIAS:
            return 0.0 <= value <= 1.0;
        case HANDL_TR_FMAXVELOCITY:
            return 0.1 <= value <= 10000.0;
        case HANDL_TR_FENGINEACCELERATION:
            return 0.0 <= value <= 10000.0;
        case HANDL_TR_FENGINEINERTIA:
            return -1000.0 <= value <= 1000.0 && value;
        case HANDL_FBRAKEDECELERATION:
            return 0.1 <= value <= 10000.0;
        case HANDL_FBRAKEBIAS:
            return 0.0 <= value <= 1.0;
        case HANDL_FSTEERINGLOCK:
            return 0.0 <= value <= 360.0;
        case
            HANDL_FSUSPENSIONFORCELEVEL,
            HANDL_FSUSPENSIONDAMPINGLEVEL,
            HANDL_FSUSPENSIONHIGHSPDCOMDAMP:
            return 0.0 <= value <= 600.0;
        case
            HANDL_FSUSPENSIONUPPERLIMIT,
            HANDL_FSUSPENSIONLOWERLIMIT:
            return -50.0 <= value <= 50.0;
        case HANDL_FSUSPENSIONBIAS:
            return 0.0 <= value <= 1.0;
        case HANDL_FSUSPENSIONANTIDIVEMULT:
            return 0.0 <= value <= 30.0;
        case HANDL_FSEATOFFSETDISTANCE:
            return -20.0 <= value <= 20.0;
        case HANDL_FCOLLISIONDAMAGEMULT:
            return 0.0 <= value <= 10.0;
	}

	return false;
}

stock bool:IsValidHandlingIntValue(CHandlingAttrib:attribute, value)
{
	if (GetHandlingAttribType(attribute) != TYPE_BYTE)
    {
		return false;
    }

	switch (attribute)
	{
        case HANDL_BABS:
            return 0 <= value <= 1;
        case HANDL_NPERCENTSUBMERGED:
            return 1 <= value <= 255;
        case HANDL_TR_NDRIVETYPE:
            return value == 'R' || value == 'F' || value == '4';
        case HANDL_TR_NNUMBEROFGEARS:
            return 1 <= value <= 5;
        case HANDL_TR_NENGINETYPE:
            return value == 'P' || value == 'D' || value == 'E';
        case
            HANDL_FRONTLIGHTS,
            HANDL_REARLIGHTS:
            return value == _:LIGHTS_SMALL || value == _:LIGHTS_BIG || value == _:LIGHTS_TALL || value == _:LIGHTS_LONG;
        case HANDL_ANIMGROUP:
            return 0 <= value <= 255;
	}
	return false;
}

stock E_HANDLING_DATA:GetHandlingAttribEnum(CHandlingAttrib:attrib)
{
    switch (attrib)
    {
        case HANDL_FMASS:
            return E_HANDLING_MASS;
        case HANDL_FTURNMASS:
            return E_HANDLING_TURN_MASS;
        case HANDL_FDRAGMULTIPLIER:
            return E_HANDLING_DRAG_MULT;
        case HANDL_CENTREOFMASS_X:
            return E_HANDLING_CENTRE_OF_MASS_X;
        case HANDL_CENTREOFMASS_Y:
            return E_HANDLING_CENTRE_OF_MASS_Y;
        case HANDL_CENTREOFMASS_Z:
            return E_HANDLING_CENTRE_OF_MASS_Z;
        case HANDL_FTRACTIONMULTIPLIER:
            return E_HANDLING_TRACTION_MULTIPLIER;
        case HANDL_FTRACTIONLOSS:
            return E_HANDLING_TRACTION_LOSS;
        case HANDL_FTRACTIONBIAS:
            return E_HANDLING_TRACTION_BIAS;
        case HANDL_TR_FMAXVELOCITY:
            return E_HANDLING_TRANSMISSION_MAX_GEAR_VELOCITY;
        case HANDL_TR_FENGINEACCELERATION:
            return E_HANDLING_TRANSMISSION_ENGINE_ACCELERATION;
        case HANDL_TR_FENGINEINERTIA:
            return E_HANDLING_TRANSMISSION_ENGINE_INERTIA;
        case HANDL_FBRAKEDECELERATION:
            return E_HANDLING_BRAKE_DECELERATION;
        case HANDL_FBRAKEBIAS:
            return E_HANDLING_BRAKE_BIAS;
        case HANDL_FSTEERINGLOCK:
            return E_HANDLING_STEERING_LOCK;
        case HANDL_FSUSPENSIONFORCELEVEL:
            return E_HANDLING_SUSPENSION_FORCE_LEVEL;
        case HANDL_FSUSPENSIONDAMPINGLEVEL:
            return E_HANDLING_SUSPENSION_DAMPING_LEVEL;
        case HANDL_FSUSPENSIONHIGHSPDCOMDAMP:
            return E_HANDLING_SUSPENSION_HIGH_SPD_COM_DAMP;
        case HANDL_FSUSPENSIONUPPERLIMIT:
            return E_HANDLING_SUSPENSION_UPPER_LIMIT;
        case HANDL_FSUSPENSIONLOWERLIMIT:
            return E_HANDLING_SUSPENSION_LOWER_LIMIT;
        case HANDL_FSUSPENSIONBIAS:
            return E_HANDLING_SUSPENSION_BIAS_BETWEEN_FRONT_AND_REAR;
        case HANDL_FSUSPENSIONANTIDIVEMULT:
            return E_HANDLING_SUSPENSION_ANTI_DIVE_MULTIPLIER;
        case HANDL_FSEATOFFSETDISTANCE:
            return E_HANDLING_SEAT_OFFSET_DISTANCE;
        case HANDL_FCOLLISIONDAMAGEMULT:
            return E_HANDLING_COLLISION_DAMAGE_MULTIPLIER;

        case HANDL_NPERCENTSUBMERGED:
            return E_HANDLING_PERCENT_SUBMERGED;
        case HANDL_ANIMGROUP:
            return E_HANDLING_ANIM_GROUP;
        case HANDL_FRONTLIGHTS:
            return E_HANDLING_FRONT_LIGHTS;
        case HANDL_REARLIGHTS:
            return E_HANDLING_REAR_LIGHTS;
        case HANDL_TR_NDRIVETYPE:
            return E_HANDLING_TRANSMISSION_DRIVE_TYPE;
        case HANDL_TR_NENGINETYPE:
            return E_HANDLING_TRANSMISSION_ENGINE_TYPE;
        case HANDL_TR_NNUMBEROFGEARS:
            return E_HANDLING_TRANSMISSION_NUMBER_OF_GEARS;
        case HANDL_BABS:
            return E_HANDLING_ABS;
            
        case HANDL_IDENTIFIER:
            return E_HANDLING_INDEX;
        case HANDL_NMONETARYVALUE:
            return E_HANDLING_MONETARY_VALUE;

        case HANDL_HANDLINGFLAGS:
            return E_HANDLING_FLAGS;
        case HANDL_MODELFLAGS:
            return E_HANDLING_MODEL_FLAGS;
    }
    return E_HANDLING_DATA:-1;
}
