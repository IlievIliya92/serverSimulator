#ifndef GENERIC_T_H_
#define GENERIC_T_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************** INCLUDE FILES *******************************/

/*********************************** DEFINES **********************************/

/****************************** GENERIC TYPEDEFS *****************************/
typedef void (*voidVoid_t)(void);
typedef void (*voidCharPtrCharPtr_t)(char *, char *);
typedef int (*intVoid_t)(void);
typedef int (*constCharPtrSizeT_t)(const char*, size_t);
typedef int (*charPtrSizeT_t)(char*, size_t);


/** @} */

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* GENERIC_T_H_ */

