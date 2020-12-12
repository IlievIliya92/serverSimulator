/******************************** INCLUDE FILES *******************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "fsm.h"
#include "protocol.h"
#include "packet.h"

#include "util/log.h"

/******************************** LOCAL DEFINES *******************************/

/******************************** TYPEDEFS ************************************/
typedef enum {
    start_Event,
    send_Event,
    ack_Event,
    invalid_Event,
    last_Event
} fsmEvent_t;

typedef fsmState_t (*eventHandler[last_State][last_Event])(mt_queue_t *dataQueue);

/******************************** GLOBALDATA *******************************/

/********************************* LOCAL DATA *********************************/

/******************************* INTERFACE DATA *******************************/

/************************ LOCAL FUNCTIONS PROTOTYPES***************************/
static fsmState_t start_handler(mt_queue_t *dataQueue);
static fsmState_t stop_handler(mt_queue_t *dataQueue);
static fsmState_t send_handler(mt_queue_t *dataQueue);
static fsmState_t ack_handler(mt_queue_t *dataQueue);

/******************************* LOCAL FUNCTIONS ******************************/
static eventHandler StateMachine = {
    [Idle_State] = {
                    [start_Event] = start_handler,
                    [invalid_Event] = NULL
                    },

    [Send_State] = {
                    [send_Event] = send_handler,
                    [invalid_Event] = NULL
                    },

    [Ack_State] = {
                    [ack_Event] = ack_handler,
                    [invalid_Event] = NULL
                   },
};

static fsmState_t start_handler(mt_queue_t *dataQueue)
{
    log_debug("START");

    return Send_State;
}

static fsmState_t send_handler(mt_queue_t *dataQueue)
{
    int data = 0;
    mt_queueReceive(dataQueue, &data);
    fprintf(stdout, "%d\n", data);
    log_debug("SEND");

    return Ack_State;
}

static fsmState_t ack_handler(mt_queue_t *dataQueue)
{
    log_debug("ACK");

    return Send_State;
}

static fsmState_t stop_handler(mt_queue_t *dataQueue)
{
    return Idle_State;
}

static fsmEvent_t fsm_readEvent(int command)
{
    fsmEvent_t event = invalid_Event;

    switch(command)
    {
        case START_EXCHG:
            event = start_Event;
        break;

        case GET_REQUEST:
            event = send_Event;
        break;

        case ACK:
            event = ack_Event;
        break;

        default:
            event = invalid_Event;
        break;
    }

    return event;
}

static void fsm_run(fsmState_t *eNextState, char *buff, mt_queue_t *dataQueue)
{
    msg_t *newMsg = packet_parse(buff);

    fsmEvent_t eNewEvent = fsm_readEvent(newMsg->header.command);

    if((*eNextState < last_State) && (eNewEvent < last_Event) && StateMachine[*eNextState][eNewEvent] != NULL)
    {
        *eNextState = (*StateMachine[*eNextState][eNewEvent])(dataQueue);
    }
    else
    {
        //Invalid
    }

    return;
}


/***************************** INTERFACE FUNCTIONS ****************************/
fsm_t *getFsm(void)
{
    fsm_t *fsm = (fsm_t *)(malloc(sizeof(fsm_t)));
    fsm->run = fsm_run;
    fsm->state = Idle_State;
    fsm->q = mt_queueNew(10, sizeof(int));

    return fsm;
}
