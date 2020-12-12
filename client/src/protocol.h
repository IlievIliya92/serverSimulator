#ifndef PROTOCOL_H
#define PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************** BRIEF *******************************/
/*
 * We assume that both server and clients are running
 * on the same machine and there will be no need to
 * change endiannes through the network or worry
 * about struct padding missmatches.
 * Fixed sized payload buffers are used to aboid serilialization
 *
 *
 *  PROTOCOL:
 *
 *  |                   HEADER              | MSG DATA |
 *  | COOKIE | COMMAND ID | PAYLOAD LEN     | DATA     |
 *  | 1 BYTE | 4 bytes    | 4 bytes         | 40 bytes |
 */
/******************************** INCLUDE FILES *******************************/

/*********************************** DEFINES **********************************/
#define COOKIE '*'

typedef enum command_t {
    START_EXCHG = 0,
    ACK,
    GET_REQUEST,
} command_t;

/************************** INTERFACE DATA DEFINITIONS ************************/
typedef struct msgHeader_t {
    char cookie;
    command_t command;
    int payloadLen;
} __attribute__((packed, aligned(1))) msgHeader_t;

typedef struct msg_t {
    msgHeader_t header;
    char payload[40];
} __attribute__((packed, aligned(1))) msg_t;

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* PROTOCOL_H */

