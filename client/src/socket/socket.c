/******************************************************************************/
/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <unistd.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>

#include "socket.h"

/******************************** LOCAL DEFINES *******************************/

/********************************* LOCAL DATA *********************************/

/******************************* INTERFACE DATA *******************************/


/******************************* LOCAL FUNCTIONS ******************************/


/***************************** INTERFACE FUNCTIONS ****************************/
int socket_unixTx(const char *socket_path, int retries)
{
    struct sockaddr_un addr;
    int sockfd;

    memset(&addr, 0, sizeof(addr));

    sockfd = socket(PF_UNIX, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        fprintf(stderr, "socket() failed\n");
        return -1;
    }

    /* start with a clean addr structure */
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    //snfprintf(stderr, addr.sun_path, UNIX_PATH_MAX, "./demo_socket");
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    while(retries > 0)
    {
        if(connect(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) != 0)
        {
            fprintf(stderr, "connect() failed\n");
            fprintf(stderr, "Try %d\n", retries);
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