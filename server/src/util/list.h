#ifndef SRC_LIST_H_
#define SRC_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************** INCLUDE FILES *******************************/

#include <stdio.h>

/*********************************** DEFINES **********************************/


/************************** INTERFACE DATA DEFINITIONS ************************/
typedef struct Node {
    int data;
    struct Node *next;
} Node_t;

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
int list_push(Node_t **, int );
int list_append(Node_t **, int);
void list_print(Node_t *);
Node_t *list_findData(Node_t *, int );

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* SRC_LIST */

