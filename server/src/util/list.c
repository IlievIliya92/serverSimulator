/******************************** INCLUDE FILES *******************************/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "list.h"
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
        fprintf(stderr, "[%s] New node memory allocation failed!\n", __func__);
        return NULL;
    }
    newNode->data = 0;
    newNode->next = NULL;

    return newNode;
}

/******************************* INTERFACE FUNCTIONS ******************************/
int list_append(Node_t **headref, int data)
{
    if (*headref == NULL)
    {
        list_push(headref, data);
        return 0;
    }

    Node_t *newNode = NULL;
    Node_t *lastNode = NULL;

    newNode = list_newNode();
    newNode->data = data;

    lastNode = *headref;
    while(lastNode->next != NULL)
    {
        lastNode = lastNode->next;
    }
    lastNode->next = newNode;

    return 0;
}

int list_push(Node_t **headref, int data)
{
    Node_t *newNode = list_newNode();
    if (newNode == NULL)
        return -1;
    newNode->data = data;

    newNode->next = (*headref);
    (*headref) = newNode;

    return 0;
}

void list_print(Node_t *headref)
{
    while(headref != NULL)
    {
        fprintf(stdout, "%d\n", headref->data);
        headref = headref->next;
    }

    return;
}

Node_t *list_findData(Node_t *headref, int data)
{
    while(headref != NULL)
    {
        if (headref->data == data)
            return headref;

        headref = headref->next;
    }

    return NULL;
}
