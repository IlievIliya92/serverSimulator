/******************************** INCLUDE FILES *******************************/
#include <argp.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "client.h"

#include "protocol.h"
#include "fsm.h"


/******************************** LOCAL DEFINES *******************************/
#define CONNECTION_TRIES  (4)

#define RX_BUFF_SIZE     (MAX_PKT_SIZE)
#define TX_BUFF_SIZE     (MAX_PKT_SIZE)

/******************************* LOCAL TYPEDEFS ******************************/

typedef enum args {
    SOCK_FILE = 0,
    CLIENT_ID,
    ARGS
} args_t;

typedef struct clientArgs_t
{
    int verbose;
    const char *client_params[ARGS];
} clientArgs_t;

/******************************** INCLUDE FILES *******************************/

/********************************* LOCAL DATA *********************************/
static volatile int STOP_CLIENT = 0;

static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Enable verbose prints"},
    {"fsock", 'f', "SOCK_FILE", 0, "Socket file for the UNIX socket"},
    {"id", 'i', "CLIENT_ID", 0, "Client ID"},
    { 0 }
};

/******************************* LOCAL FUNCTIONS ******************************/
static
error_t parse_option( int key, char *arg, struct argp_state *state )
{
     clientArgs_t *arguments = state->input;

    switch (key) {
        case 'f':
            arguments->client_params[SOCK_FILE] = arg;
            break;
        case 'i':
            arguments->client_params[CLIENT_ID] = arg;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = {options, parse_option};
/********************************** MAIN ************************************/

static void sig_handler(int sig)
{
    fprintf(stdout, "[%s] Stopping the client\n", __func__);
    STOP_CLIENT = 1;
    return;
}

int main(int argc, char *argv[])
{
    clientArgs_t args = { 0 };

    int clientId = -1;
    int rcvLen = 0;
    int txLen = 0;

    char buffRx[RX_BUFF_SIZE];
    char buffTx[TX_BUFF_SIZE];

    if( argp_parse(&argp, argc, argv, 0, 0, &args) != 0 )
    {
        fprintf(stderr, "[%s] Invalid argument!\n", __func__);
        return -1;
    }

    if (args.client_params[SOCK_FILE] == NULL)
    {
        fprintf(stderr, "[%s] Missing socket file path\n", __func__);
        return -1;
    }
    const char *sockPath = args.client_params[SOCK_FILE];

    if (args.client_params[CLIENT_ID] != NULL)
    {
        clientId = atoi(args.client_params[CLIENT_ID]);
    }

    if (clientId < 0)
    {
        fprintf(stderr, "[%s] Missing client ID\n", __func__);
        return -1;
    }

    /* Create a new client */
    clientMethods_t *client = clientGet(sockPath,
                                        clientId,
                                        CONNECTION_TRIES);
    if (client == NULL)
        return -1;

    /* Create a new state machine */
    fsmObj_t *fsm = fsmNew();
    if (fsm == NULL)
        return -1;

    /* Try to connect to the server  */
    if (client->connect() < 0)
    {
        goto exit;
    }

    signal(SIGINT, sig_handler);

    memset(buffTx, 0x0, TX_BUFF_SIZE);
    memset(buffRx, 0x0, RX_BUFF_SIZE);

    fsm->init(client->getId());
    fprintf(stdout, CLIENT_TAG(client->getId()));
    fprintf(stdout, "Client %d started\n", client->getId());

    while(!STOP_CLIENT)
    {
        if (fsm->statusRun())
        {
            rcvLen = client->rcv(buffRx, RX_BUFF_SIZE);
        }

        if (rcvLen > 0 || (!fsm->statusRun()))
        {
            txLen = 0;
            fsm->run(buffRx, buffTx, &txLen);
            client->send(buffTx, txLen);
        }
    }

exit:
    clientRelease(client);
    fsmDelete(fsm);

    fprintf(stderr, "Clossing client application ...\n");

    return 0;
}
