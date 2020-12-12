/******************************** INCLUDE FILES *******************************/
#include <argp.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "util/log.h"
#include "server.h"


/******************************** LOCAL DEFINES *******************************/
typedef enum args {
    VERBOSITY_LEVEL = 0,
    SOCK_FILE,
    ARGS
} args_t;

typedef struct serverArgs_t
{
    int verbose;
    const char *server_params[ARGS];
} serverArgs_t;

/******************************* LOCAL TYPEDEFS ******************************/

/******************************** INCLUDE FILES *******************************/

/********************************* LOCAL DATA *********************************/
static struct argp_option options[] = {
    {"verbose", 'v', "VERBOSITY_LEVEL", 0, "Verbose level"},
    {"fsock", 'f', "SOCK_FILE", 0, "Socket file for the UNIX socket"},
    { 0 }
};

/******************************* LOCAL FUNCTIONS ******************************/
static
error_t parse_option( int key, char *arg, struct argp_state *state )
{
     serverArgs_t *arguments = state->input;

    switch (key) {
        case 'v':
            arguments->verbose= atoi(arg);
            break;

        case 'f':
            arguments->server_params[SOCK_FILE]= arg;
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static void sig_handler(int sig)
{
    fprintf(stdout, "[%s] Stopping the server\n", __func__);

    server_stop();

    return;
}

static struct argp argp = {options, parse_option};
/********************************** MAIN ************************************/
int main(int argc, char *argv[])
{
     serverArgs_t args = { 0 };

    if( argp_parse(&argp, argc, argv, 0, 0, &args) != 0 )
    {
        fprintf(stderr, "[%s] Invalid argument!\n", __func__);
        return -1;
    }

    if ((args.verbose) > 0)
        log_Setlevel(args.verbose);

    server_start(args.server_params[SOCK_FILE]);
    signal(SIGINT, sig_handler);

    return 0;
}
