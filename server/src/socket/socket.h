#ifndef SRC_SOCKET_SOCKET_H_
#define SRC_SOCKET_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************** INCLUDE FILES *******************************/

#include <stdio.h>

/*********************************** DEFINES **********************************/


/************************** INTERFACE DATA DEFINITIONS ************************/


/************************* INTERFACE FUNCTION PROTOTYPES **********************/
typedef void (*socket_new_t)(int conn_fd, void *arg);
void socket_serverStart(const char *socket_file, socket_new_t cb, void *arg);
void socket_serverStop(void);

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* SRC_SOCKET_SOCKET_H_ */

