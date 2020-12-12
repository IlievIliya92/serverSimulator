/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <unistd.h>

#include "packet.h"
/******************************** LOCAL DEFINES *******************************/

/******************************* LOCAL TYPEDEFS ******************************/

/********************************* LOCAL DATA *********************************/

/******************************* LOCAL FUNCTIONS ******************************/

/******************************* INTERFACE FUNCTIONS ******************************/
msg_t *packet_parse(char *buff)
{
    msg_t *newMsg = (msg_t *)buff;

    fprintf(stdout, "COOKIE: %c\n", newMsg->header.cookie);
    fprintf(stdout, "COMM: %d\n", newMsg->header.command);
    fprintf(stdout, "LEN: %d\n", newMsg->header.payloadLen);
    fprintf(stdout, "PL: %s\n", newMsg->payload);

    return newMsg;
}


