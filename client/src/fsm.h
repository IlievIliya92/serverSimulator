#ifndef SRC_FSM_H_
#define SRC_FSM_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************** INCLUDE FILES *******************************/
#include "generic_t.h"

/*********************************** DEFINES **********************************/
typedef struct _fsmObj_t {
    VoidInt_t init;
    voidCharPtrCharPtrInt_t run;
    intVoid_t statusRun;
} fsmObj_t;

/************************** INTERFACE DATA DEFINITIONS ************************/

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
fsmObj_t *fsmNew(void);
void fsmDelete(fsmObj_t *fsm);

#ifdef __cplusplus
}
#endif

#endif /* SRC_FSM_H_ */
