/*=============================================================================
 * Copyright (c) 2019, Ezequiel Urdaniz <urdanizezequiel@gmail.com>
 * All rights reserved.
 * Date: 2019/11/11
 * Version: 1.2
 *===========================================================================*/

#ifndef PROGRAMS_COMUNICACION_RTOS_PC_INC_BLUE_USB_UART_H_
#define PROGRAMS_COMUNICACION_RTOS_PC_INC_BLUE_USB_UART_H_


#include "Main_RTOS_PC.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif


/*=====[Definition macros of public constants]===============================*/

#define BAUDIO	9600
#define UART_BLUETOOTH UART_232
#define UART_PC        UART_USB

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

extern uint8_t control_Out;				//variable externa de control de salidas.
extern uint8_t control_data;			//variable control de datos.


/*=====[Prototypes (declarations) of public functions]=======================*/

bool_t hm10bleTest( int32_t uart ); // Prototito de función para testear HM10.

void init_UART_USB_BLE (void); 		// Prototipo de función inicializar Comunicacion.

void Recibe_PC (void* taskParmPtr); // Prototipo de tarea recibir datos PC.

void Recibe_BLE (void* taskParmPtr); // Prototipo de tarea recibir datos BLE.

void ControlDataBLE (void* taskParmPtr); // Prototipo de tarea control de data BLE.

void ControlDataPC (void* taskParmPtr); // Prototipo de tarea control de data PC.


/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/



#endif /* PROGRAMS_COMUNICACION_RTOS_PC_INC_BLUE_USB_UART_H_ */
