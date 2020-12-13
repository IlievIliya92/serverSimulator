#ifndef SRC_PACKET_H
#define SRC_PACKET_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************** INCLUDE FILES *******************************/
#include "protocol.h"

/*********************************** DEFINES **********************************/


/************************** INTERFACE DATA DEFINITIONS ************************/

/************************** INTERFACE FUNCTIONS DEFINITIONS ************************/
int packet_rcv(int confd, char *rxBuffer, int rxBufferLen);
msg_t *packet_parse(char *buff);

int packet_placeCmd(msg_t *msg, int cmd);
int packet_placeData(msg_t *msg, int data);

msg_t *packet_create(char *buff);
int packet_send(int confd, msg_t *msg);

/** @} */

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* SRC_PACKET_H */

