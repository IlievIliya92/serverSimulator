/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "fsm.h"
#include "util/plf.h"
#include "util/list.h"
#include "socket/socket.h"

/******************************** LOCAL DEFINES *******************************/
#define RX_BUFF_SIZE 256

/******************************* LOCAL TYPEDEFS ******************************/
typedef struct conn_t {
    int connfd;
    fsm_t *fsm;
    char rxBuffer[RX_BUFF_SIZE];
} conn_t;

typedef struct server_t {
    int connections;
    Node_t *conns;
} server_t;

/********************************* LOCAL DATA *********************************/
void *connectionHandler(void *arg)
{
    server_t *server = NULL;

    server = (server_t *)arg;
    conn_t conn = *((conn_t *)server->conns->data);

    int connfd = conn.connfd;
    fsm_t *fsm = conn.fsm;
    char *rxBuffer = conn.rxBuffer;

    for (;;)
    {
        if(read(connfd, rxBuffer, RX_BUFF_SIZE) > 0)
        {
            fprintf(stdout, "%d\n", connfd);
            fsm->run(&fsm->state, rxBuffer);
        }
        else
        {
            fprintf(stderr, "Clossing the connection\n");
            server->connections--;
            break;
        }
    }

    return NULL;
}

/******************************* LOCAL FUNCTIONS ******************************/
static void newConnection(int connfd, void *arg)
{
    server_t *server = NULL;
    conn_t newConnection;
    pthread_t thread_id;

    server = (server_t *)arg;

    server->connections++;
    fprintf(stdout, "Clients connected %d\n", server->connections);

    newConnection.connfd = connfd;
    newConnection.fsm = getFsm();
    list_push(&server->conns, &newConnection, DATA_SIZE(conn_t));

    pthread_create(&thread_id, NULL, connectionHandler, server);
}

static void server_init(server_t *server)
{
    server->connections = 0;
    server->conns = NULL;

    return;
}

/******************************* INTERFACE FUNCTIONS ******************************/
int server_start(const char *sockFile)
{
    server_t server;

    server_init(&server);

    if (sockFile != NULL)
    {
        socket_serverStart(sockFile, newConnection, &server);
    }
    else
    {
        fprintf(stderr, "[%s] Missing valid socket file!\n", __func__);
    }

    return 0;
}

void server_stop(void)
{
    socket_serverStop();
}