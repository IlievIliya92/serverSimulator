/******************************** INCLUDE FILES *******************************/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "list.h"

#include "log.h"
/******************************** LOCAL DEFINES *******************************/

/******************************* LOCAL TYPEDEFS ******************************/

/********************************* LOCAL DATA *********************************/

/******************************* LOCAL FUNCTIONS ******************************/
static Node_t *list_newNode(void)
{
    Node_t *newNode = NULL;
    newNode = (Node_t *)malloc(sizeof(Node_t));
    if (newNode == NULL)
    {
        log_err("New node memory allocation failed!\n");
        return NULL;
    }
    newNode->data = NULL;
    newNode->next = NULL;

    return newNode;
}

/******************************* INTERFACE FUNCTIONS ******************************/
int list_append(Node_t **headref, void *data, int dataLen)
{
    if (*headref == NULL)
    {
        list_push(headref, data, dataLen);
        return 0;
    }

    Node_t *newNode = NULL;
    Node_t *lastNode = NULL;

    newNode = list_newNode();
    int offset = 0;
    newNode->data = malloc(dataLen);
    for (offset = 0; offset < dataLen; offset++)
    {
        *((uint8_t *)(newNode->data + offset)) =  *((uint8_t *)(data + offset));
    }

    lastNode = *headref;
    while(lastNode->next != NULL)
    {
        lastNode = lastNode->next;
    }
    lastNode->next = newNode;

    return 0;
}

int list_push(Node_t **headref, void *data, int dataLen)
{
    Node_t *newNode = list_newNode();
    if (newNode == NULL)
        return -1;

    int offset = 0;
    newNode->data = malloc(dataLen);
    for (offset = 0; offset < dataLen; offset++)
    {
        *((uint8_t *)(newNode->data + offset)) =  *((uint8_t *)(data + offset));
    }

    newNode->next = (*headref);
    (*headref) = newNode;

    return 0;
}

Node_t * list_New(void)
{
  Node_t *ll = NULL;

  ll = malloc(sizeof(Node_t));
  ll->next = NULL;
  ll->data = NULL;

  return ll;
}