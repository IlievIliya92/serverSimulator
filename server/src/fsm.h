#ifndef SRC_FSM_H_
#define SRC_FSM_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************** INCLUDE FILES *******************************/
#include "util/queue.h"

/*********************************** DEFINES **********************************/
typedef enum {
    Idle_State,
    Send_State,
    Ack_State,
    Err_State,
    last_State
} fsmState_t;

typedef void (*voidConn_t)(fsmState_t *,
                           char *,
                           mt_queue_t *q,
                           int);

typedef struct {
    voidConn_t run;
    mt_queue_t *q;
    fsmState_t state;
} fsm_t;

/************************** INTERFACE DATA DEFINITIONS ************************/

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
fsm_t *getFsm(void);

#ifdef __cplusplus
}
#endif

#endif /* SRC_FSM_H_ */
