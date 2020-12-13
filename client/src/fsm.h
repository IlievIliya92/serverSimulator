#ifndef SRC_FSM_H_
#define SRC_FSM_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************** INCLUDE FILES *******************************/
#include "generic_t.h"

/*********************************** DEFINES **********************************/
typedef struct _fsmObj_t {
    voidCharPtrCharPtr_t run;
} fsmObj_t;

/************************** INTERFACE DATA DEFINITIONS ************************/

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
fsmObj_t *fsmNew(void);

#ifdef __cplusplus
}
#endif

#endif /* SRC_FSM_H_ */
