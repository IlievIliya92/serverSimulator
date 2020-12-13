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
typedef struct _clientObj_t {
    intVoid_t connect;
    constCharPtrSizeT_t send;
    charPtrSizeT_t rcv;
    intVoid_t getState;
    intVoid_t getId;
} clientObj_t;

/********************************** INTERFACE FUNCS ***************************/
clientObj_t *clientNew(const char *, int, int );
void clientDelete(clientObj_t *);


/** @} */

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* SRC_CLIENT_H_ */

