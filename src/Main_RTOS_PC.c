/*=============================================================================
 * Copyright (c) 2019, Ezequiel Urdaniz <urdanizezequiel@gmail.com>
 * All rights reserved.
 * Date: 2019/11/11
 * Version: 1.2
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "Main_RTOS_PC.h"
#include "BLUE_USB_UART.h"
#include "LED_OUT.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

DEBUG_PRINT_ENABLE;  //Configurar mensajes de consola

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

SemaphoreHandle_t Evento_Recibe, Evento_Save; 	//Semaphore de tareas

/*=====[Main function, program entry point after power on or reset]==========*/



int main( void ){

	// ---------- CONFIGURACIONES ------------------------------
	boardConfig();			// Init & config CIAA

	init_UART_USB_BLE();	// Init UART_USB & UART 232 Modulo BLUETOOTH

	// -----------CONFIGURACIÓN de SEMAPHORE -------------------
	uint8_t Error_state = 0;	// inicializo flag de error.

	if (NULL == (Evento_Recibe = xSemaphoreCreateBinary())){   // Semaphore Recibe.
		Error_state =1;
	}

	if (NULL == (Evento_Save = xSemaphoreCreateBinary())){   // Semaphore Save.
		Error_state =1;
	}


	// --------------CREACIÓN de TAREAS --------------------------
	xTaskCreate(
		Recibe_PC,                     	// Funcion de la tarea a ejecutar
		(const char *)"Recibe_PC",     	// Nombre de la tarea como String
		configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
		0,                          	// Parametros de tarea
	    tskIDLE_PRIORITY+3,         	// Prioridad de la tarea
	    0                           	// Puntero a la tarea creada en el sistema
	);

	xTaskCreate(
		Recibe_BLE,                     	// Funcion de la tarea a ejecutar
		(const char *)"Recibe_BLE",     	// Nombre de la tarea como String
		configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
		0,                          	// Parametros de tarea
	    tskIDLE_PRIORITY+3,         	// Prioridad de la tarea
	    0                           	// Puntero a la tarea creada en el sistema
	);

	xTaskCreate(
		ControlDataBLE,                     // Funcion de la tarea a ejecutar
		(const char *)"ControlDataBLE",     // Nombre de la tarea como String
		configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
		0,                          	// Parametros de tarea
	    tskIDLE_PRIORITY+2,         	// Prioridad de la tarea
	    0                           	// Puntero a la tarea creada en el sistema
	);

	xTaskCreate(
		ControlDataPC ,                   // Funcion de la tarea a ejecutar
		(const char *)"ControlDataPC",     // Nombre de la tarea como String
		configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
		0,                          	// Parametros de tarea
	    tskIDLE_PRIORITY+2,         	// Prioridad de la tarea
	    0                           	// Puntero a la tarea creada en el sistema
	);

	xTaskCreate(
		ControlOut,                     // Funcion de la tarea a ejecutar
		(const char *)"ControlOut",     // Nombre de la tarea como String
		configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
		0,                          	// Parametros de tarea
	    tskIDLE_PRIORITY+1,         	// Prioridad de la tarea
	    0                           	// Puntero a la tarea creada en el sistema
	);

	xTaskCreate(
		ControlTecla,                   // Funcion de la tarea a ejecutar
		(const char *)"ControlTecla",     // Nombre de la tarea como String
		configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
		0,                          	// Parametros de tarea
	    tskIDLE_PRIORITY+3,         	// Prioridad de la tarea
	    0                           	// Puntero a la tarea creada en el sistema
	);




   //-------------- INCIAR SCHEDDULER -------------------------
   if (0 == Error_state){
	   vTaskStartScheduler();	//Inicializo el SO.

   } else{
	   gpioWrite( LEDR, ON );  // No pudo iniciar el scheduler.
   }

   // ------------- REPETIR POR SIEMPRE --------------------------
   while( TRUE ){
	   //Sin Accion.
   }

   return 0;
}
