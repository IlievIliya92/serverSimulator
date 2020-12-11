/******************************** INCLUDE FILES *******************************/
#include <argp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "socket/socket.h"
/******************************** LOCAL DEFINES *******************************/
typedef enum args {
    SOCK_FILE = 0,
    TEST_CONTENT,
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
            arguments->client_params[SOCK_FILE]= arg;
            break;
        case 't':
            arguments->client_params[TEST_CONTENT]= arg;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = {options, parse_option};
/********************************** MAIN ************************************/
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

    if (args.client_params[TEST_CONTENT] == NULL) {
        fprintf(stderr, "[%s] Missing test content\n", __func__);
        return -1;
    }


    const char *testTxBuffer = args.client_params[TEST_CONTENT];

    int sendSock = socket_unixTx(args.client_params[SOCK_FILE], 5);
    if (sendSock < 0)
    {
        return -1;
    }

    for(;;)
    {
        fprintf(stdout, "Sending new test message to the server\n");
        write(sendSock, testTxBuffer, strlen(testTxBuffer));
        sleep(1);
    }

    return 0;
}
