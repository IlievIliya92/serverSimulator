/******************************** INCLUDE FILES *******************************/
#include <argp.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "util/log.h"
#include "server.h"


/******************************** LOCAL DEFINES *******************************/
typedef struct serverArgs_t
{
    int verbose;
    int slots;
    const char *sockFile;
} serverArgs_t;

/******************************* LOCAL TYPEDEFS ******************************/

/******************************** INCLUDE FILES *******************************/

/********************************* LOCAL DATA *********************************/
static struct argp_option options[] = {
    {"verbose", 'v', "VERBOSITY_LEVEL", 0, "Verbose level"},
    {"fsock", 'f', "SOCK_FILE", 0, "Socket file for the UNIX socket"},
    {"slots", 's', "SLOTS", 0, "Server connection slots"},
    { 0 }
};

/******************************* LOCAL FUNCTIONS ******************************/
static
error_t parse_option( int key, char *arg, struct argp_state *state )
{
     serverArgs_t *arguments = state->input;

    switch (key) {
        case 'v':
            arguments->verbose = atoi(arg);
            break;
        case 'f':
            arguments->sockFile = arg;
            break;
        case 's':
            arguments->slots = atoi(arg);
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

    exit(0);
}

static struct argp argp = {options, parse_option};
/********************************** MAIN ************************************/
int main(int argc, char *argv[])
{
    serverArgs_t args = { 0 };
    int slots = 0;

    if( argp_parse(&argp, argc, argv, 0, 0, &args) != 0 )
    {
        fprintf(stderr, "[%s] Invalid argument!\n", __func__);
        return -1;
    }

    if (args.verbose > 0) {
        log_Setlevel(args.verbose);
    }

    if (args.slots <= 0) {
        log_err("The connection slots can not be les than 1");
        return -3;
    }

    signal(SIGINT, sig_handler);
    server_start(args.sockFile, args.slots);

    return 0;
}
