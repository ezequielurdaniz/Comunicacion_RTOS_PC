/*=============================================================================
 * Copyright (c) 2019, Ezequiel Urdaniz <urdanizezequiel@gmail.com>
 * All rights reserved.
 * Date: 2019/11/11
 * Version: 1.2
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __COMUNICACION_RTOS_PC_H__
#define __COMUNICACION_RTOS_PC_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include "stdint.h"
#include "stddef.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

extern uint8_t control_Out = 0; 	//variable externa de control de salidas.
extern uint8_t control_data = 0;	//variable control de datos.

/*=====[Prototypes (declarations) of public functions]=======================*/

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __COMUNICACION_RTOS_PC_H__ */
