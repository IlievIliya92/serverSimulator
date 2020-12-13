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
typedef void (*intCharPtrCharPtr_t)(char *, char *);
typedef void (*voidCharPtrCharPtrInt_t)(char *, char *, int *);
typedef void (*VoidInt_t)(int);
typedef int (*intVoid_t)(void);
typedef int (*constCharPtrSizeT_t)(const char*, size_t);
typedef int (*charPtrSizeT_t)(char*, size_t);
typedef int (*voidCharPtrInt_t)(char*, int);
typedef int (*voidCharPtrIntPtrInt_t)(char*, int *, int);

/** @} */

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* GENERIC_T_H_ */

