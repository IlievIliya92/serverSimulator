/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "fsm.h"
#include "protocol.h"

#include "socket/socket.h"

#include "util/plf.h"
#include "util/list.h"
#include "util/queue.h"
#include "util/log.h"

/******************************** LOCAL DEFINES *******************************/
#define RX_BUFF_SIZE (DATA_SIZE(msgHeader_t) + PL_SIZE)
#define TX_BUFF_SIZE  RX_BUFF_SIZE

/******************************* LOCAL TYPEDEFS ******************************/
typedef struct conn_t {
    int connfd;
    fsm_t *fsm;
    pthread_t threadId;
    char rxBuffer[RX_BUFF_SIZE];
    char txBuffer[TX_BUFF_SIZE];
} conn_t;

typedef struct server_t {
    int connections;
    Node_t *conns;
    pthread_mutex_t data_lock;
} server_t;

/********************************* LOCAL DATA *********************************/
void *connectionHandler(void *arg)
{
    server_t *server = NULL;

    server = (server_t *)arg;
    pthread_mutex_lock(&server->data_lock);
    conn_t conn = *((conn_t *)server->conns->data);
    pthread_mutex_unlock(&server->data_lock);

    fprintf(stdout, "%d\n", conn.connfd);
    for (;;)
    {
        memset(conn.rxBuffer, 0x0, RX_BUFF_SIZE);
        if(read(conn.connfd, conn.rxBuffer, RX_BUFF_SIZE) > 0)
        {
            conn.fsm->run(&conn.fsm->state, conn.rxBuffer, conn.fsm->q);
        }
        else
        {
            log_err("Connection closed\n");
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
    conn_t newConn;

    server = (server_t *)arg;

    newConn.connfd = connfd;
    newConn.fsm = getFsm();

    pthread_mutex_lock(&server->data_lock);
    server->connections++;
    fprintf(stdout, "Clients connected %d\n", server->connections);
    list_push(&server->conns, &newConn, DATA_SIZE(conn_t));
    pthread_mutex_unlock(&server->data_lock);

    pthread_create(&newConn.threadId, NULL, connectionHandler, server);

}

static void server_init(server_t *server)
{
    server->connections = 0;
    server->conns = NULL;
    pthread_mutex_init(&server->data_lock, NULL);

    return;
}

static void sendData(Node_t *node, int *data)
{
    while(node != NULL)
    {
        conn_t conn = *((conn_t *)node->data);
        printf("%d\n", conn.connfd);
        mt_queueSend(conn.fsm->q, data);
        node = node->next;
    }
}


static void *producerThread(void *arg)
{
    server_t *server = (server_t *)arg;
    int data = 0;

    for(;;)
    {
        pthread_mutex_lock(&server->data_lock);
        /* If there is somebody listening lets send him the data */
        if (0 !=  server->connections)
        {
            sendData(server->conns, &data);
        }
        pthread_mutex_unlock(&server->data_lock);

        data++;
        sleep(1);
    }


    return NULL;
}

/******************************* INTERFACE FUNCTIONS ******************************/
int server_start(const char *sockFile)
{
    server_t server;
    pthread_t producerId;
    pthread_create(&producerId, NULL, producerThread, &server);

    server_init(&server);
    if (sockFile != NULL)
    {
        socket_serverStart(sockFile, newConnection, &server);
    }
    else
    {
        log_err("Missing valid socket file!\n");
    }

    return 0;
}

void server_stop(void)
{
    socket_serverStop();
}