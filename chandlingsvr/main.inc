#if !defined _chandlingsvr_included
    #endinput
#endif

#if defined _chandlingsvr_main_included
    #endinput
#endif
#define _chandlingsvr_main_included

#define CHANDLING_VERSION "v1.0-dev-omp"

/*
 * The compaatibility version number decides if the client supports our CHandling version or not
 * Clients with smaller compat version won't be able to use CHandling
 * 
 * Increase this number only if doing things that break the compatibility with older client
*/
#define CHANDLING_COMPAT_VERSION 0x1001D

#define MAX_VEHICLE_MODELS (212)

// keep in mind  vehicle ids in samp start from 1
#define IS_VALID_VEHICLEID(%0) \
	(1 <= (%0) < MAX_VEHICLES)

#define IS_VALID_VEHICLE_MODEL(%0) \
	(400 <= (%0) <= (MAX_VEHICLE_MODELS + 400))

#define VEHICLE_MODEL_INDEX(%0) \
	((%0) - 400)

#include <chandlingsvr\packet-enum>
#include <chandlingsvr\player>
#include <chandlingsvr\actions>
#include <chandlingsvr\handling-enum>
#include <chandlingsvr\handling-default>
#include <chandlingsvr\handling-manager>
#include <chandlingsvr\natives>
#include <chandlingsvr\packets>

public OnIncomingConnection(playerid, ip_address[], port)
{
	if (IS_VALID_PLAYERID(playerid))
	{
		ResetPlayerCHandlingData(playerid);
	}
	
	#if defined CHandling_OnIncomingConnection
		return CHandling_OnIncomingConnection(playerid, ip_address, port);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnIncomingConnection
	#undef OnIncomingConnection
#else
	#define _ALS_OnIncomingConnection
#endif
#define OnIncomingConnection CHandling_OnIncomingConnection
#if defined CHandling_OnIncomingConnection
	forward CHandling_OnIncomingConnection(playerid, ip_address[], port);
#endif

public OnPlayerConnect(playerid)
{
	// print("[chandling] OnPlayerConnect");
	HandlingMgr_OnPlayerConnect(playerid);
	
	#if defined CHandling_OnPlayerConnect
		return CHandling_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect CHandling_OnPlayerConnect
#if defined CHandling_OnPlayerConnect
	forward CHandling_OnPlayerConnect(playerid);
#endif

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	// printf("[chandling] OnVehicleStreamIn(%d,%d)", vehicleid, forplayerid);

	// Send handling modifications for this vehicle
	HandlingMgr_OnVehicleStreamIn(vehicleid, forplayerid);
	
	#if defined CHandling_OnVehicleStreamIn
		return CHandling_OnVehicleStreamIn(vehicleid, forplayerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleStreamIn
	#undef OnVehicleStreamIn
#else
	#define _ALS_OnVehicleStreamIn
#endif
#define OnVehicleStreamIn CHandling_OnVehicleStreamIn
#if defined CHandling_OnVehicleStreamIn
	forward CHandling_OnVehicleStreamIn(vehicleid, forplayerid);
#endif

static bool:ScriptInit()
{
	InitializeModelHandlings();

	printf("[###] CHandlingSvr " CHANDLING_VERSION " by .silent loaded [###]");
	return true;
}

#if defined FILTERSCRIPT
	public OnFilterScriptInit()
	{
		ScriptInit();

		#if defined CHandling_OnFilterScriptInit
			return CHandling_OnFilterScriptInit();
		#else
			return 1;
		#endif
	}
	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif
	#define OnFilterScriptInit CHandling_OnFilterScriptInit
	#if defined CHandling_OnFilterScriptInit
		forward CHandling_OnFilterScriptInit();
	#endif
#else
	public OnGameModeInit()
	{
		ScriptInit();

		#if defined CHandling_OnGameModeInit
			return CHandling_OnGameModeInit();
		#else
			return 1;
		#endif
	}
	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit CHandling_OnGameModeInit
	#if defined CHandling_OnGameModeInit
		forward CHandling_OnGameModeInit();
	#endif
#endif
