#if !defined _chandlingsvr_included
    #endinput
#endif

#if defined _chandlingsvr_packets_included
    #endinput
#endif
#define _chandlingsvr_packets_included

IPacket:ID_CHANDLING(playerid, BitStream:bs)
{
    printf("ID_CHANDLING");

    new number;
    BS_GetNumberOfBytesUsed(bs, number);

    if (number > 2)
    {
        new CHandlingAction:id;

        BS_ReadValue(bs,
            PR_IGNORE_BITS, 8,
            PR_UINT8,       _:id
        );

        ProcessCHandlingAction(id, bs, playerid);
    }

    return 1;
}
