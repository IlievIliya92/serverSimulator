#ifndef GENERIC_T_H_
#define GENERIC_T_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************** INCLUDE FILES *******************************/

/*********************************** DEFINES **********************************/
/* just used for coloring probably not the best place to define it */
#define ANSI_COLOR_YELLOW    "\x1b[33m"
#define COLOR_RESET          "\x1b[0m"
#define CLIENT_TAG(x)        ANSI_COLOR_YELLOW"CLIENT%d "COLOR_RESET, x

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

