#ifndef SRC_FSM_H_
#define SRC_FSM_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************** INCLUDE FILES *******************************/

/*********************************** DEFINES **********************************/
typedef enum {
    Idle_State,
    Start_State,
    last_State
} fsmState_t;

typedef void (*voidVoidPtr_t)(fsmState_t *, char *buff);

typedef struct {
   voidVoidPtr_t run;
   fsmState_t  state;
} fsm_t;

/************************** INTERFACE DATA DEFINITIONS ************************/

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
fsm_t *getFsm(void);

#ifdef __cplusplus
}
#endif

#endif /* SRC_FSM_H_ */
