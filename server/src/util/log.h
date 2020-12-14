/*******************************************************************************
 *******************************************************************************/
#ifndef SRC_UTIL_LOG_H_
#define SRC_UTIL_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************** INCLUDE FILES *******************************/

/*********************************** DEFINES **********************************/
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_RESET   "\x1b[0m"
#define SERVER_TAG COLOR_BLUE"SERVER "COLOR_RESET

/************************** INTERFACE DATA DEFINITIONS ************************/

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
void log_Setlevel(int level);

void log_info(char *msg);
void log_err(char *msg);
void log_debug(char *msg);
void log_v1(char *msg);
void log_v2(char *msg);


#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* SRC_UTIL_LOG_H_ */

