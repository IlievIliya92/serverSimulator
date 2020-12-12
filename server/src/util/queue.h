#ifndef SRC_QUEUE_H_
#define SRC_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************** INCLUDE FILES *******************************/

/*********************************** DEFINES **********************************/

/************************** INTERFACE DATA DEFINITIONS ************************/
typedef struct _mt_queue_t mt_queue_t;

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
mt_queue_t * mt_queueNew(int capacity, int element_size);
void mt_queueDelete(mt_queue_t *queue);
int mt_queueSend(mt_queue_t *queue, const void * element);
int mt_queueReceive(mt_queue_t *queue, void * element_out);


#ifdef __cplusplus
}
#endif

#endif /* SRC_QUEUE_H_ */
