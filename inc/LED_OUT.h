/*=============================================================================
 * Copyright (c) 2019, Ezequiel Urdaniz <urdanizezequiel@gmail.com>
 * All rights reserved.
 * Date: 2019/11/11
 * Version: 1.2
 *===========================================================================*/

#ifndef MIS_PROGRAMAS_TP_SOLO_PWM_INC_PWM_H_
#define MIS_PROGRAMAS_TP_SOLO_PWM_INC_PWM_H_


#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"


/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

extern uint8_t control_Out; 			//variable externa de control de salidas.
extern uint8_t control_data;			//variable externa control de datos.

/*=====[Prototypes (declarations) of public functions]=======================*/

void ControlOut (void* taskParmPtr);  // Prototipo de tarea control de salida.
void ControlTecla(void* taskParmPtr); // Prototipo de tarea control de tecla.


/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/





#endif /* MIS_PROGRAMAS_TP_SOLO_PWM_INC_PWM_H_ */
