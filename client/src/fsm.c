/******************************** INCLUDE FILES *******************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "fsm.h"
#include "protocol.h"
#include "packet.h"

/******************************** LOCAL DEFINES *******************************/
#define MODULE_NAME "FSM"

#define MSG_SLOTS   5

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
    Rcv_State,
    Ack_State,
    Term_State,
    Err_State,
    last_State
} fsmState_t;

typedef struct _fsmCtx_t {
    msg_t *newInMsg;
    msg_t newOutMsg;
    fsmState_t eNextState;
    fsmEvent_t eNewEvent;
    int running;
    int clientId;
} fsmCtx_t;

typedef fsmState_t (*eventHandler[last_State][last_Event])(int, msg_t *);

/********************************* FSM STATE *********************************/
static fsmCtx_t fsm_ctx;

/******************************* INTERFACE DATA *******************************/

/************************ LOCAL FUNCTIONS PROTOTYPES***************************/
static fsmState_t start_handler(int, msg_t *);
static fsmState_t rcv_handler(int, msg_t *);
static fsmState_t ack_handler(int, msg_t *);
static fsmState_t err_handler(int, msg_t *);

/******************************* LOCAL FUNCTIONS ******************************/
static eventHandler StateMachine = {
    [Idle_State] = {
                    [start_Event] = start_handler,
                    [invalid_Event] = err_handler,
                    },

    [Rcv_State] = {
                    [ack_Event] = ack_handler,
                    [send_Event] = rcv_handler,
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
    return Rcv_State;
}

static fsmState_t rcv_handler(int id, msg_t *outMsg)
{
    packet_placeCmd(outMsg, ACK);
    return Ack_State;
}

static fsmState_t ack_handler(int id, msg_t *outMsg)
{
    packet_placeCmd(outMsg, GET_REQUEST);
    return Rcv_State;
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

        case GET_RESPONSE:
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

static void fsm_init(int clientId)
{
    fsm_ctx.eNextState = Idle_State;
    fsm_ctx.eNewEvent = start_Event;
    fsm_ctx.running = 0;
    fsm_ctx.clientId = clientId;

    return;
}

static void fsm_run(char *rxBuff, char *txBuff, int *txLen)
{
    fsm_ctx.newInMsg = packet_parse(rxBuff);
    /* TODO: Verify the packet */

    packet_placeCookie(&fsm_ctx.newOutMsg, COOKIE);
    packet_placeID(&fsm_ctx.newOutMsg, fsm_ctx.clientId);
    fsm_ctx.newOutMsg.header.payloadLen = 0;

    /*
     * The first itteration the rxBuff is not received from the socket.
     * This serves to trigger the FSM from the IDLE state.
    */
    if (fsm_ctx.running) {
        fsm_ctx.eNewEvent = fsm_readEvent(fsm_ctx.newInMsg->header.command);
    }

    if((fsm_ctx.eNextState < last_State) && (fsm_ctx.eNewEvent < last_Event) &&
        StateMachine[fsm_ctx.eNextState][fsm_ctx.eNewEvent] != NULL)
    {
        fsm_ctx.eNextState = (*StateMachine[fsm_ctx.eNextState][fsm_ctx.eNewEvent])(
                                                        fsm_ctx.newInMsg->header.clientId,
                                                        &fsm_ctx.newOutMsg
                                                        );


        if (fsm_ctx.newInMsg->header.command == GET_RESPONSE)
        {
            fprintf(stdout, " Server Es produced  “%s”\n", fsm_ctx.newInMsg->payload);
        }
        /* move to send buffer */
        *txLen = fsm_ctx.newOutMsg.header.payloadLen + HEADER_SIZE;

        /* TODO: this is kinda slow the outmsg is already prepared it can be send. */
        memcpy(txBuff, &fsm_ctx.newOutMsg, *txLen);

        /* Indication that the fsm is triggered and not in IDLE */
        fsm_ctx.running = 1;
    }
    else
    {
        /* Invalid */
    }

    return;
}

static int fsm_status (void)
{
    return fsm_ctx.running;
}

/***************************** INTERFACE FUNCTIONS ****************************/
fsmObj_t *fsmNew(void)
{
    fsmObj_t *fsm = (fsmObj_t *)(malloc(sizeof(fsmObj_t)));
    if (fsm == NULL)
    {
        fprintf(stderr, "[%s] Fsm creation failed.\n", MODULE_NAME);
    }
    fsm->init = fsm_init;
    fsm->run = fsm_run;
    fsm->statusRun = fsm_status;

    return fsm;
}


void fsmDelete(fsmObj_t *fsm)
{
    /* Free the memory for the fsm's methods */
    if (fsm != NULL)
    {
        free(fsm);
    }

    return;
}