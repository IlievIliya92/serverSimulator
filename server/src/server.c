/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "fsm.h"
#include "protocol.h"


#include "packet.h"
#include "socket/socket.h"

#include "util/plf.h"
#include "util/list.h"
#include "util/queue.h"
#include "util/log.h"

/******************************** LOCAL DEFINES *******************************/
#define RX_BUFF_SIZE (DATA_SIZE(msgHeader_t) + PL_SIZE)

/******************************* LOCAL TYPEDEFS ******************************/
typedef struct conn_t {
    bool_t slotActive;
    int connfd;
    fsm_t *fsm;
    pthread_t threadId;
    char rxBuffer[RX_BUFF_SIZE];
} conn_t;

typedef struct server_t {
    int connections;
    int conSlots;
    float simSpeed;
    pthread_t producerId;

    /* Connection Slots */
    conn_t **conns;
    pthread_mutex_t data_lock;
} server_t;

/********************************* LOCAL DATA *********************************/
static volatile bool_t stopConnection;

/******************************* LOCAL FUNCTIONS ******************************/
static void closeConnection(conn_t *conn)
{
    close(conn->connfd);
    mt_queueDelete(conn->fsm->q);
    free(conn->fsm);
    conn->slotActive = FALSE;

    return;
}

static void *connectionHandler(void *arg)
{
    conn_t *conn = (conn_t *)arg;

    while(!stopConnection)
    {
        memset(conn->rxBuffer, 0x0, RX_BUFF_SIZE);
        if(packet_rcv(conn->connfd, conn->rxBuffer, RX_BUFF_SIZE) > 0)
        {
            conn->fsm->run(&conn->fsm->state,
                           conn->rxBuffer,
                           conn->fsm->q,
                           conn->connfd);
        }
        else
        {
            fprintf(stderr, "Connection closed\n");
            break;
        }
    }

    /*TODO: delete connection */
    closeConnection(conn);

    return NULL;
}

static void newConnection(int connfd, void *arg)
{
    server_t *server = NULL;
    int  i = 0;

    server = (server_t *)arg;

    pthread_mutex_lock(&server->data_lock);

    /* TODO: change to linked list to reduce time complexity */
    for (i = 0; i < server->conSlots; i++)
    {
        if (server->conns[i]->slotActive == FALSE)
        {
            server->conns[i]->connfd = connfd;
            server->conns[i]->fsm = getFsm();
            server->conns[i]->slotActive = TRUE;
            server->connections++;
            pthread_create(&server->conns[i]->threadId,
                           NULL, connectionHandler, server->conns[i]);
            break;
        }
    }
    pthread_mutex_unlock(&server->data_lock);

    return;
}

static void server_init(server_t *server, int connectionsSlots, float simSpeed)
{
    int i = 0;

    stopConnection = FALSE;
    server->connections = 0;
    server->simSpeed = simSpeed * MICRO_TO_SECONDS;
    server->conSlots = connectionsSlots;
    server->conns = (conn_t **)malloc(sizeof(conn_t*));
    for (i = 0; i < server->conSlots; i++) {
        server->conns[i] = (conn_t *)malloc(sizeof(conn_t));
        server->conns[i]->slotActive = FALSE;
    }

    pthread_mutex_init(&server->data_lock, NULL);

    return;
}

static void server_deinit(server_t *server)
{
    int i = 0;
    for (i = 0; i < server->conSlots; i++) {
        free(server->conns[i]);
    }
    free(server->conns);
}

static void *producerThread(void *arg)
{
    server_t *server = (server_t *)arg;
    int data = 0;
    int i = 0;

    while(!stopConnection)
    {
        data++;
        fprintf(stdout, SERVER_TAG"“Unit %d” produced\n", data);

        pthread_mutex_lock(&server->data_lock);
        /* If there is somebody listening lets send him the data */
        if (0 !=  server->connections)
        {
            for (i = 0; i < server->conSlots; i++)
            {
                if (server->conns[i]->slotActive)
                {
                    mt_queueSend(server->conns[i]->fsm->q, &data);
                }
            }
        }
        pthread_mutex_unlock(&server->data_lock);

        /* sleep to simulate some data aquistion */
        usleep(server->simSpeed);
    }
    server_deinit(server);

    return NULL;
}

static void server_connectionsStop()
{
    stopConnection = TRUE;
}

/******************************* INTERFACE FUNCTIONS ******************************/
int server_start(const char *sockFile, int connectionSlots, float simSpeed)
{
    server_t server;

    server_init(&server, connectionSlots, simSpeed);
    pthread_create(&server.producerId, NULL, producerThread, &server);

    fprintf(stdout, SERVER_TAG"Server Es\n");
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

void server_stop()
{
    /* Stop the server from accepting new connections */
    socket_serverStop();

    /* Stop the producer thread & deinit connections data */
    server_connectionsStop();
}