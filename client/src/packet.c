/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>

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

int packet_placeCmd(msg_t *msg, int cmd)
{
    msg->header.command = cmd;

    return 0;
}

int packet_placeCookie(msg_t *msg, char cookie)
{
    msg->header.cookie = cookie;

    return 0;
}

int packet_placeID(msg_t *msg, int id)
{
    msg->header.clientId = id;

    return 0;
}

int packet_placeData(msg_t *msg, int data)
{
    msg->header.payloadLen = sprintf(msg->payload, "Unit %d", data);

    return 0;
}

int packet_send(int connfd, msg_t *msg)
{
    int ret = 0;
    ret = write(connfd, (char *)msg, (sizeof(msg_t)));

    return ret;
}

int packet_rcv(int connfd, char *rxBuffer, int rxBufferLen)
{
    return read(connfd, rxBuffer, rxBufferLen);
}

