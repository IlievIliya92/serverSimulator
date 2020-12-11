#ifndef SRC_SOCKET_SOCKET_H_
#define SRC_SOCKET_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************** INCLUDE FILES *******************************/
#include "util/plf.h"

/*********************************** DEFINES **********************************/


/************************** INTERFACE DATA DEFINITIONS ************************/


/************************* INTERFACE FUNCTION PROTOTYPES **********************/
 int socket_unixTx(const char *socket_path, int retries);

/** @} */

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* SRC_SOCKET_SOCKET_H_ */

