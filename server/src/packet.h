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

/** @} */

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* SRC_PACKET_H */

