#ifndef SRC_CLIENT_H_
#define SRC_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************** INCLUDE FILES *******************************/
#include "generic_t.h"
/*********************************** DEFINES **********************************/

/****************************** GENERIC TYPEDEFS *****************************/

/********************************** METHODS ***********************************/
typedef struct _clientMethods_t {
    intVoid_t connect;
    constCharPtrSizeT_t send;
    charPtrSizeT_t rcv;
    intVoid_t getState;
    intVoid_t getId;
} clientMethods_t;

/********************************** INTERFACE FUNCS ***************************/
clientMethods_t *clientGet(const char *, int, int );
void clientRelease();


/** @} */

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* SRC_CLIENT_H_ */

