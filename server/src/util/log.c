/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <string.h>

/******************************** LOCAL DEFINES *******************************/

/******************************* LOCAL TYPEDEFS ******************************/

/********************************* LOCAL DATA *********************************/
static int verbosityLevel = 0;

/******************************* LOCAL FUNCTIONS ******************************/

/******************************* INTERFACE FUNCTIONS ******************************/
void log_Setlevel(int level)
{
    verbosityLevel = level;

    return;
}

void log_info(char *msg)
{
    fprintf(stdout, "%s\n", msg);

    return;
}

void log_err(char *msg)
{
    fprintf(stderr, "%s\n", msg);

    return;
}

void log_debug(char *msg)
{
    if(verbosityLevel > 0)
    {
        fprintf(stdout, "%s\n", msg);
    }

    return;
}

void log_v1(char *msg)
{
    if(verbosityLevel > 1)
    {
        fprintf(stdout, "%s\n", msg);
    }
    return;
}

void log_v2(char *msg)
{
    if(verbosityLevel > 2)
    {
        fprintf(stdout, "%s\n", msg);
    }
    return;
}

