/*******************************************************************************
 *******************************************************************************/
#ifndef SRC_SERVER_LOG_H_
#define SRC_SERVER_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************** INCLUDE FILES *******************************/

/*********************************** DEFINES **********************************/

/************************** INTERFACE DATA DEFINITIONS ************************/

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
int server_start(const char *, int);
void server_stop(void);

/** @} */

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* SRC_SERVER_LOG_H_ */

