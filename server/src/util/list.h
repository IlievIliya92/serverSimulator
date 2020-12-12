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
    void *data;
    struct Node *next;
} Node_t;

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
int list_push(Node_t **, void *, int );
int list_append(Node_t **, void *, int);

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* SRC_LIST */

