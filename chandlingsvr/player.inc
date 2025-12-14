#if !defined _chandlingsvr_included
    #endinput
#endif

#if defined _chandlingsvr_player_included
    #endinput
#endif
#define _chandlingsvr_player_included

#define IS_VALID_PLAYERID(%0) \
	(0 <= (%0) < MAX_PLAYERS)

static enum E_PLAYER_DATA
{
    bool:E_PLAYER_HAS_CHANDLING
}

static s_Player[MAX_PLAYERS][E_PLAYER_DATA];

IsPlayerUsingCHandling(playerid)
{
    if (!IS_VALID_PLAYERID(playerid))
    {
        return 0;
    }
    return s_Player[playerid][E_PLAYER_HAS_CHANDLING];
}

SetPlayerHasCHandling(playerid)
{
    if (IS_VALID_PLAYERID(playerid))
    {
        s_Player[playerid][E_PLAYER_HAS_CHANDLING] = true;
    }
}

ResetPlayerCHandlingData(playerid)
{
    if (IS_VALID_PLAYERID(playerid))
    {
        s_Player[playerid][E_PLAYER_HAS_CHANDLING] = false;
    }
}
