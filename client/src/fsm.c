/******************************** INCLUDE FILES *******************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "fsm.h"
#include "protocol.h"
#include "packet.h"

/******************************** LOCAL DEFINES *******************************/
#define MODULE_NAME "FSM"

/******************************** TYPEDEFS ************************************/
typedef enum {
    start_Event,
    send_Event,
    ack_Event,
    invalid_Event,
    last_Event
} fsmEvent_t;

typedef enum {
    Idle_State,
    Send_State,
    Ack_State,
    Term_State,
    Err_State,
    last_State
} fsmState_t;

typedef fsmState_t (*eventHandler[last_State][last_Event])(int, msg_t *);
/******************************** GLOBALDATA *******************************/

/********************************* FSM STATE *********************************/
static fsmState_t eNextState;

/******************************* INTERFACE DATA *******************************/

/************************ LOCAL FUNCTIONS PROTOTYPES***************************/
static fsmState_t start_handler(int, msg_t *);
static fsmState_t send_handler(int, msg_t *);
static fsmState_t ack_handler(int, msg_t *);
static fsmState_t err_handler(int, msg_t *);

/******************************* LOCAL FUNCTIONS ******************************/
static eventHandler StateMachine = {
    [Idle_State] = {
                    [start_Event] = start_handler,
                    [invalid_Event] = err_handler,
                    },

    [Send_State] = {
                    [send_Event] = send_handler,
                    [invalid_Event] = err_handler,
                    },

    [Ack_State] =  {
                    [ack_Event] = ack_handler,
                    [invalid_Event] = err_handler,
                   },
};

static fsmState_t start_handler(int id, msg_t *outMsg)
{
    packet_placeCmd(outMsg, START_EXCHG);
    return Send_State;
}

static fsmState_t send_handler(int id, msg_t *outMsg)
{
    packet_placeCmd(outMsg, GET_RESPONSE);
    return Ack_State;
}

static fsmState_t ack_handler(int id, msg_t *outMsg)
{
    packet_placeCmd(outMsg, ACK);
    return Send_State;
}

static fsmState_t err_handler(int id, msg_t *outMsg)
{
    packet_placeCmd(outMsg, ERR);
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

static void fsm_run(char *rxBuff, char *txBuff)
{
    msg_t *newInMsg = packet_parse(rxBuff);
    msg_t newOutMsg;

    fsmEvent_t eNewEvent = fsm_readEvent(newInMsg->header.command);
    if((eNextState < last_State) && (eNewEvent < last_Event) && StateMachine[eNextState][eNewEvent] != NULL)
    {
        eNextState = (*StateMachine[eNextState][eNewEvent])(newInMsg->header.clientId, &newOutMsg);
         //packet_send(connfd, &newOutMsg);
    }
    else
    {
        /* Invalid */
    }

    return;
}


/***************************** INTERFACE FUNCTIONS ****************************/
fsmObj_t *fsmNew(void)
{
    eNextState = Idle_State;

    fsmObj_t *fsm = (fsmObj_t *)(malloc(sizeof(fsmObj_t)));
    if (fsm == NULL)
    {
        fprintf(stderr, "[%s] Fsm creation failed.\n", MODULE_NAME);
    }
    fsm->run = fsm_run;

    return fsm;
}
