#if !defined _chandlingsvr_included
    #endinput
#endif

#if defined _chandlingsvr_actions_included
    #endinput
#endif
#define _chandlingsvr_actions_included

// action identifier is sent as single byte
enum CHandlingAction
{
	ACTION_INIT = 10, // This is the only packet that is sent  by the player, to indicate that we can speak to him
	ACTION_INIT_RESPONSE,

	ACTION_RESET_MODEL = 15,
	ACTION_RESET_VEHICLE,
	ACTION_SET_VEHICLE_HANDLING,
	ACTION_SET_MODEL_HANDLING
};

BitStream:CHandlingActionPacket(CHandlingAction:actionID)
{
    new BitStream:data = BS_New();

    BS_WriteValue(data,
        PR_UINT8,   ID_CHANDLING,
        PR_UINT8,   _:actionID
    );

    return data;
}

ProcessCHandlingAction(CHandlingAction:id, BitStream:bs, playerid)
{
    switch (id)
    {
        case ACTION_INIT:
        {
            if (IS_VALID_PLAYERID(playerid))
            {
                new compatVer;
                BS_ReadUint32(bs, compatVer);

                new BitStream:pkt = CHandlingActionPacket(ACTION_INIT_RESPONSE);
                BS_WriteUint32(bs, CHANDLING_COMPAT_VERSION);

                if (compatVer >= CHANDLING_COMPAT_VERSION)
                {
                    BS_WriteBool(pkt, true);
                    SetPlayerHasCHandling(playerid);
                    printf("[chandling] Player %d reports having chandling plugin", playerid);
                }
                else
                {
                    BS_WriteBool(pkt, false);
                }

                PR_SendPacket(bs, playerid, PR_HIGH_PRIORITY, PR_RELIABLE_ORDERED, 0);
                BS_Delete(pkt);
            }

            return true;
        }
    }
    return false;
}
