/******************************************************************************/
/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>

#include "client.h"

/******************************** LOCAL DEFINES *******************************/
#define  MODULE_NAME  "CLIENT"
/********************************* LOCAL DATA *********************************/

/******************************* Typedefs    *******************************/
typedef struct _clientCtx_t {
    const char *sockPath;
    int sockFd;
    int clientId;
    int connTries;
    int connected;
} clientCtx_t;

/*******************************  Client State Data ***************************/
static clientCtx_t client_ctx;
static clientMethods_t clientMethods;

/******************************* LOCAL FUNCTIONS ******************************/
static int client_socketInit(const char *socket_path, int retries)
{
    struct sockaddr_un addr;
    int sockfd;

    memset(&addr, 0, sizeof(addr));
    sockfd = socket(PF_UNIX, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        fprintf(stderr, "[%s] socket creation failed\n", MODULE_NAME);
        return -1;
    }

    /* start with a clean addr structure */
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
    while(retries > 0)
    {
        if(connect(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) != 0)
        {
            fprintf(stderr, "[%s] connection to server failed\n", MODULE_NAME);
            fprintf(stderr, "[%s] retry %d\n", MODULE_NAME, retries);
            sleep(1);
        }
        else
        {
            return sockfd;
        }
        retries--;
    }

    return -1;
}

/***************************** LOCAL FUNCTIONS ****************************/
static int client_connect(void)
{
    client_ctx.sockFd = client_socketInit(client_ctx.sockPath, client_ctx.connTries);
    if (client_ctx.sockFd < 0 )
    {
        fprintf(stderr, "[%s] failed to connect to the server on %s\n",
                MODULE_NAME, client_ctx.sockPath);
        return -1;
    }
    client_ctx.connected = 1;
    return 0;
}

static int client_send(const char *buff, size_t buffLen)
{
    int nbytes = 0;

    if (client_ctx.connected == 0)
    {
        fprintf(stderr, "[%s] send failed. Client not connected!\n", MODULE_NAME);
        return -1;
    }
    nbytes = write(client_ctx.sockFd, buff, buffLen);

    return nbytes;
}

static int client_rcv(char *buff, size_t buffLen)
{
    int nbytes = 0;

    if (client_ctx.connected == 0)
    {
        fprintf(stderr, "[%s] receive failed. Client not connected!\n", MODULE_NAME);
        return -1;
    }
    nbytes = read(client_ctx.sockFd, buff, buffLen);

    return nbytes;
}

/********************************** GETTERS ***************************/
static int client_getState(void)
{
    return client_ctx.connected;
}

static int client_getId(void)
{
    return client_ctx.clientId;
}

/********************************** PUBLIC FUNCS ***************************/
clientMethods_t *clientGet(const char *sockPath, int clientID, int clientConnTries)
{
    if (client_ctx.connected == 1) {
        fprintf(stdout, "[%s] You already have a running client!\n", MODULE_NAME);
        return NULL;
    }

    client_ctx.sockFd = -1;
    client_ctx.sockPath = sockPath;
    client_ctx.clientId = clientID;
    client_ctx.connTries = clientConnTries;
    client_ctx.connected = 0;

    clientMethods.connect = client_connect;
    clientMethods.send = client_send;
    clientMethods.rcv = client_rcv;
    clientMethods.getState = client_getState;
    clientMethods.getId = client_getId;

    return &clientMethods;
}

void clientRelease()
{
    if (client_ctx.connected && (client_ctx.sockFd > 0))
    {
        close(client_ctx.sockFd);
    }
    client_ctx.connected = 0;

    return;
}