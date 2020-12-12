/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <unistd.h>

#include "protocol.h"
#include "packet.h"
/******************************** LOCAL DEFINES *******************************/

/******************************* LOCAL TYPEDEFS ******************************/

/********************************* LOCAL DATA *********************************/

/******************************* LOCAL FUNCTIONS ******************************/

/******************************* INTERFACE FUNCTIONS ******************************/
msg_t *packet_parse(char *buff)
{
    msg_t *newMsg = (msg_t *)buff;
    return newMsg;
}

msg_t *packet_create(char *buff, int cmd, int data)
{
    msg_t *newMsg = (msg_t *)buff;

    newMsg->header.cookie = COOKIE;
    newMsg->header.command = cmd;

    newMsg->header.payloadLen = sprintf(newMsg->payload, "Server Es produced \"Unit %d\"", data);

    return newMsg;
}
