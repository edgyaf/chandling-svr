#if !defined _chandlingsvr_included
    #endinput
#endif

#if defined _chandlingsvr_packet_enum_included
    #endinput
#endif
#define _chandlingsvr_packet_enum_included
// CHandling packets

enum _:CHandlingPacketID
{
    ID_CHANDLING = 251 // one packet to rule them all, we don't want to use any ID occupied by samp so let's keep it minimal
};
