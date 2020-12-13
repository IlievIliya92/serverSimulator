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
msg_t *packet_parse(char *buff);

int packet_placeCmd(msg_t *msg, int cmd);
int packet_placeData(msg_t *msg, int data);
int packet_placeCookie(msg_t *msg, char cookie);
int packet_placeID(msg_t *msg, int id);

/** @} */

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* SRC_PACKET_H */

