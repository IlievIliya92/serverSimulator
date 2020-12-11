/******************************************************************************/
/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <unistd.h>

#include <sys/un.h>
#include <sys/stat.h> /* for chmod */
#include <sys/socket.h>

#include "socket.h"
#include "util/plf.h"

/******************************** LOCAL DEFINES *******************************/
#define SERVER_SOCK_FILE_MODE  0777
#define SOCKET_TIMEOUT_SEC     5

/********************************* LOCAL DATA *********************************/

static volatile bool_t AbortServer;

/******************************* INTERFACE DATA *******************************/


/******************************* LOCAL FUNCTIONS ******************************/


/***************************** INTERFACE FUNCTIONS ****************************/
void socket_serverStop(void)
{
    AbortServer = TRUE;
}

void socket_serverStart(const char *socket_file, socket_new_t cb, void *arg)
{
    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_un serv_addr;

    mode_t mode = SERVER_SOCK_FILE_MODE;

    if ((listenfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Failed to create the server socket!\n");
        return;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sun_family = AF_UNIX;
    strncpy(serv_addr.sun_path, socket_file, sizeof(serv_addr.sun_path));
    unlink(socket_file);

    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0)
    {
        fprintf(stderr, "bind failed\n");
        return;
    }

    if (listen(listenfd, 10) != 0)
    {
        fprintf(stderr, "listen failed\n");
        return;
    }

    if ((chmod(socket_file, mode)) != 0)
    {
        fprintf(stderr, "Failed to change the mode of the socket!\n");
        return;
    }

    while(!AbortServer)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        if (connfd != -1)
        {
            struct timeval timeout;
            timeout.tv_sec = SOCKET_TIMEOUT_SEC;
            timeout.tv_usec = 0;

            if (setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
            {
                fprintf(stderr, "setsockopt1 failed\n");
            }

            cb(connfd, arg);
        }
    }

    return;
}
