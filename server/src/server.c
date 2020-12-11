/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

#include "util/list.h"
#include "socket/socket.h"

/******************************** LOCAL DEFINES *******************************/
#define RX_BUFF_SIZE 256

/******************************* LOCAL TYPEDEFS ******************************/
typedef struct server_t {
    int connections;
    Node_t *connfds;
} server_t;

/********************************* LOCAL DATA *********************************/
void *connectionHandler(void *arg)
{
    int *connfd = 0;
    char rxBuffer[RX_BUFF_SIZE] = {0};

    connfd = (int *)arg;

    for (;;)
    {
        if(read(*connfd, &rxBuffer, RX_BUFF_SIZE) > 0)
            printf("%s\n", rxBuffer);

        printf("%d\n", *connfd);
    }

    return NULL;
}

/******************************* LOCAL FUNCTIONS ******************************/
static void newConnection(int connfd, void *arg)
{
    server_t *server = NULL;
    pthread_t thread_id;

    server = (server_t *)arg;

    server->connections++;
    list_push(&server->connfds, connfd);

    fprintf(stdout, "Clients connected %d\n", server->connections);
    pthread_create(&thread_id, NULL, connectionHandler, &server->connfds->data);
}

static void server_init(server_t *server)
{
    server->connections = 0;
    server->connfds = NULL;

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