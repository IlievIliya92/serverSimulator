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

typedef fsmState_t (*eventHandler[last_State][last_Event])(int, msg_t *, mt_queue_t *);
/******************************** GLOBALDATA *******************************/

/********************************* LOCAL DATA *********************************/

/******************************* INTERFACE DATA *******************************/

/************************ LOCAL FUNCTIONS PROTOTYPES***************************/
static fsmState_t start_handler(int, msg_t *, mt_queue_t *);
static fsmState_t send_handler(int, msg_t *, mt_queue_t *);
static fsmState_t ack_handler(int, msg_t *, mt_queue_t *);
static fsmState_t err_handler(int, msg_t *, mt_queue_t *);

/******************************* LOCAL FUNCTIONS ******************************/
static eventHandler StateMachine = {
    [Idle_State] = {
                    [start_Event] = start_handler,
                    [invalid_Event] = err_handler,
                    },

    [Send_State] = {
                    [start_Event] = NULL,
                    [send_Event] = send_handler,
                    [invalid_Event] = err_handler,
                    },

    [Ack_State] = {
                    [ack_Event] = ack_handler,
                    [invalid_Event] = err_handler,
                   },
};

static fsmState_t start_handler(int id, msg_t *outMsg, mt_queue_t *dataQueue)
{
    fprintf(stdout, "Client %d connected.\n", id);

    packet_placeCmd(outMsg, ACK);


    return Send_State;
}

static fsmState_t send_handler(int id, msg_t *outMsg, mt_queue_t *dataQueue)
{
    int data = 0;
    mt_queueReceive(dataQueue, &data);
    packet_placeData(outMsg, data);
    packet_placeCmd(outMsg, GET_RESPONSE);

    return Ack_State;
}

static fsmState_t ack_handler(int id, msg_t *outMsg, mt_queue_t *dataQueue)
{
    fprintf(stdout, "Client %d message receive Ack.\n", id);
    packet_placeCmd(outMsg, ACK);

    return Send_State;
}

static fsmState_t err_handler(int id, msg_t *outMsg, mt_queue_t *dataQueue)
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
        /*TODO: Add all the events from the protocol */

        default:
            event = invalid_Event;
        break;
    }

    return event;
}

static void fsm_run(fsmState_t *eNextState,
                    char *rxBuff,
                    mt_queue_t *dataQueue,
                    int connfd)
{
    msg_t *newInMsg = packet_parse(rxBuff);
    /* TODO: Verify the packet */
    msg_t newOutMsg;

    newOutMsg.header.cookie = COOKIE;
    newOutMsg.header.command = INVALID;
    newOutMsg.header.clientId = newInMsg->header.clientId;
    newOutMsg.header.payloadLen = 0;
    memset(newOutMsg.payload, 0x0, PL_SIZE);

    fsmEvent_t eNewEvent = fsm_readEvent(newInMsg->header.command);
    if((*eNextState < last_State) && (eNewEvent < last_Event) && StateMachine[*eNextState][eNewEvent] != NULL)
    {
        *eNextState = (*StateMachine[*eNextState][eNewEvent])(newInMsg->header.clientId, &newOutMsg, dataQueue);
         packet_send(connfd, &newOutMsg);
    }
    else
    {
        /* Invalid */
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
