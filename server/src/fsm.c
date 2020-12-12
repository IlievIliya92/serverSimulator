/******************************** INCLUDE FILES *******************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "fsm.h"
#include "protocol.h"
#include "packet.h"

/******************************** LOCAL DEFINES *******************************/

/******************************** TYPEDEFS ************************************/
typedef enum {
    start_Event,
    invalid_Event,
    last_Event
} fsmEvent_t;

typedef fsmState_t (*eventHandler[last_State][last_Event])(void *args);

/******************************** GLOBALDATA *******************************/

/********************************* LOCAL DATA *********************************/
static fsmState_t eNextState;
/******************************* INTERFACE DATA *******************************/

/************************ LOCAL FUNCTIONS PROTOTYPES***************************/
static fsmState_t start_handler(void *args);
static fsmState_t stop_handler(void *args);

/******************************* LOCAL FUNCTIONS ******************************/
static eventHandler StateMachine = {
    [Idle_State] = {
                    [start_Event] = start_handler,
                    [invalid_Event] = NULL
                    },

    [Start_State] = {
                    [start_Event] = NULL,
                    [invalid_Event] = NULL
                    },
};

static fsmState_t start_handler(void *args)
{
    return Start_State;
}

static fsmState_t stop_handler(void *args)
{
    return Idle_State;
}

static fsmEvent_t fsm_readEvent(char *cmd)
{
    fsmEvent_t event = invalid_Event;
    return event;
}

static void fsm_run(fsmState_t *eNextState, char *buff)
{
    msg_t *newMsg = packet_parse(buff);
    fsmEvent_t eNewEvent = fsm_readEvent(buff);
    if((*eNextState < last_State) && (eNewEvent < last_Event) && StateMachine[*eNextState][eNewEvent] != NULL)
    {
        *eNextState = (*StateMachine[*eNextState][eNewEvent])(NULL);
    }
    else
    {
        //Invalid
    }

    return;
}

static
fsm_t fsm = {
    fsm_run,
    Idle_State,
};

/***************************** INTERFACE FUNCTIONS ****************************/

fsm_t *getFsm(void)
{
    return &fsm;
}
