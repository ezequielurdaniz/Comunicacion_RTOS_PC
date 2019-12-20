/*=============================================================================
 * Copyright (c) 2019, Ezequiel Urdaniz <urdanizezequiel@gmail.com>
 * All rights reserved.
 * Date: 2019/12/18
 * Version: 1.0
 *===========================================================================*/

#ifndef PROGRAMS_COMUNICACION_RTOS_PC_INC_TECLA_H_
#define PROGRAMS_COMUNICACION_RTOS_PC_INC_TECLA_H_

#include "Main_RTOS_PC.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif


/*=====[Definition macros of public constants]===============================*/

//---- Definición de estado de TECLA --------------
#define UP      1
#define FALLING 2
#define DOWN	3
#define RISING  4

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/


void fsmButtonUpdate(gpioMap_t tecla,			// Prototipo de función estado de tecla.
					uint8_t Control,
					uint8_t* Estadotec,
					portTickType* TiempoTec);

void Tecla( void* taskParmPtr ); 				// Prototipo de tarea tecla antirebote.


/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/


#endif /* PROGRAMS_COMUNICACION_RTOS_PC_INC_TECLA_H_ */
