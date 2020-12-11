/******************************** INCLUDE FILES *******************************/
#include <argp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/******************************** LOCAL DEFINES *******************************/

/******************************* LOCAL TYPEDEFS ******************************/
typedef struct server_t {
    int sockfd;
} server_t;


/********************************* LOCAL DATA *********************************/

/******************************* LOCAL FUNCTIONS ******************************/
static void server_init(const char *sockFile) {


}


/******************************* INTERFACE FUNCTIONS ******************************/
int server_start(const char *sockFile)
{

    server_init(sockFile);

    fprintf(stdout, "%s\n", sockFile);
    return 0;
}