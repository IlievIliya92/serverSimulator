/******************************** INCLUDE FILES *******************************/
#include <argp.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "protocol.h"
#include "socket/socket.h"
/******************************** LOCAL DEFINES *******************************/
typedef enum args {
    SOCK_FILE = 0,
    TEST_CONTENT,
    CLIENT_ID,
    ARGS
} args_t;

typedef struct clientArgs_t
{
    int verbose;
    const char *client_params[ARGS];
} clientArgs_t;

/******************************* LOCAL TYPEDEFS ******************************/

/******************************** INCLUDE FILES *******************************/

/********************************* LOCAL DATA *********************************/
static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Enable verbose prints"},
    {"fsock", 'f', "SOCK_FILE", 0, "Socket file for the UNIX socket"},
    {"fsock", 'i', "CLIENT_ID", 0, "Client ID"},
    {"test", 't', "TEST_CONTENT", 0, "Test content of the message"},
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
        case 't':
            arguments->client_params[TEST_CONTENT] = arg;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = {options, parse_option};
/********************************** MAIN ************************************/
static volatile bool_t STOP_CLIENT = FALSE;

static void sig_handler(int sig)
{
    fprintf(stdout, "[%s] Stopping the client\n", __func__);
    STOP_CLIENT = TRUE;
}


int main(int argc, char *argv[])
{
    clientArgs_t args = { 0 };

    if( argp_parse(&argp, argc, argv, 0, 0, &args) != 0 )
    {
        fprintf(stderr, "[%s] Invalid argument!\n", __func__);
        return -1;
    }

    if (args.client_params[SOCK_FILE] == NULL) {
        fprintf(stderr, "[%s] Missing socket file path\n", __func__);
        return -1;
    }

    int clientId = -1;
    if (args.client_params[CLIENT_ID] != NULL)
    {
        clientId = atoi(args.client_params[CLIENT_ID]);
    }
    if (clientId < 0)
    {
        fprintf(stderr, "[%s] Missing client ID\n", __func__);
        return -1;
    }

    if (args.client_params[TEST_CONTENT] == NULL) {
        fprintf(stderr, "[%s] Missing test content\n", __func__);
    }
    const char *payload = args.client_params[TEST_CONTENT];

    int sendSock = socket_unixTx(args.client_params[SOCK_FILE], 5);
    if (sendSock < 0)
    {
        return -1;
    }


    msg_t *newOutMsg[4];
    msg_t *newInMsg[4];

    int i = 0;
    for (i = 0; i < 4; i++)
    {
        newOutMsg[i] = (msg_t *)malloc(sizeof(msg_t));
        newInMsg[i] = (msg_t *)malloc(sizeof(msg_t));

        newOutMsg[i]->header.cookie = COOKIE;
        newOutMsg[i]->header.payloadLen = strlen(payload);
        newOutMsg[i]->header.clientId = clientId;
        memcpy(newOutMsg[i]->payload, payload, newOutMsg[i]->header.payloadLen);
    }
    newOutMsg[0]->header.command = START_EXCHG;
    newOutMsg[1]->header.command = GET_REQUEST;
    newOutMsg[2]->header.command = ACK;
    newOutMsg[3]->header.command = TERM;

    signal(SIGINT, sig_handler);


    write(sendSock, (char *)newOutMsg[0], (sizeof(msg_t)));
    read(sendSock, (char *)newInMsg[0], (sizeof(msg_t)));

    fprintf(stdout, "Client %d\n", clientId);
    while (!STOP_CLIENT)
    {
        for (i = 1; i < 3; i++)
        {
            write(sendSock, (char *)newOutMsg[i], (sizeof(msg_t)));
            read(sendSock, (char *)newInMsg[i], (sizeof(msg_t)));
            if (newInMsg[i]->payload != NULL && newInMsg[i]->header.command == GET_RESPONSE)
                fprintf(stdout, "Server Es produced “%s”\n", newInMsg[i]->payload);
            memset(newInMsg[i], 0x0, sizeof(msg_t));
        }
    }

    write(sendSock, (char *)newOutMsg[4], (sizeof(msg_t)));
    read(sendSock, (char *)newInMsg[4], (sizeof(msg_t)));
    close(sendSock);

    return 0;
}
