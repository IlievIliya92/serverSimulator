
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "queue.h"

/******************************** LOCAL DEFINES *******************************/

/******************************** TYPEDEFS ************************************/
struct _mt_queue_t {
  char *data;
  int  head; // Put in the head position and advance so it's the next free slot
  int  tail; // Take from tail position and advance. head==tail means empty
  int  elem_size;
  int  capacity;

  pthread_mutex_t   data_lock;
  sem_t   more_room;
  sem_t   more_data;
};
/******************************** GLOBALDATA *******************************/

/********************************* LOCAL DATA *********************************/

/******************************* INTERFACE DATA *******************************/

/************************ LOCAL FUNCTIONS PROTOTYPES***************************/

/***************************** INTERFACE FUNCTIONS ****************************/
mt_queue_t * mt_queueNew(int capacity, int element_size)
{
  mt_queue_t *q;

  q = malloc(sizeof(struct _mt_queue_t));

  // The ring-buffer must be able to hold one more element than capacity
  q->data = malloc((capacity + 1) * element_size);
  q->capacity = capacity + 1;
  q->elem_size = element_size;
  q->head = 0;
  q->tail = 0;

  pthread_mutex_init(&q->data_lock, NULL);
  sem_init(&q->more_data, 0, 0);
  sem_init(&q->more_room, 0, capacity);

  return q;
}

int mt_queueSend(mt_queue_t *q, const void * element)
{
    sem_wait(&q->more_room);
    int offset;

    pthread_mutex_lock(&q->data_lock);

    offset = q->elem_size * q->head;
    memcpy(&(q->data[offset]), element, q->elem_size);

    q->head += 1;
    if (q->head >= q->capacity)
    {
        q->head = 0;
    }

    if (q->head == q->tail)
    {
        fprintf(stderr, "Message queue overflow, should never happen!\n");
    }

    pthread_mutex_unlock(&q->data_lock);

    // And we must give to the data mutex, now that there is one more message
    sem_post(&q->more_data);

    return 0;
}

int mt_queueReceive(mt_queue_t *q, void * element_out)
{
        sem_wait(&q->more_data);
        pthread_mutex_lock(&q->data_lock);
        if (q->head != q->tail)
        {
            int offset;
            offset = q->elem_size * q->tail;
            memcpy(element_out, &(q->data[offset]), q->elem_size);

            q->tail += 1;
            if (q->tail >= q->capacity)
            {
                q->tail = 0;
            }
        }
        else
        {
            fprintf(stderr, "Message queue underflow, should never happen!\n");
        }
        pthread_mutex_unlock(&q->data_lock);

        // And we must give to the room mutex, now that there is room for one more message
        sem_post(&q->more_room);

        return 0;
}

void mt_queueDelete(mt_queue_t *q)
{
  pthread_mutex_destroy(&q->data_lock);
  sem_destroy(&q->more_data);
  sem_destroy(&q->more_room);

  free(q->data);
  free(q);
}