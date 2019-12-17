/*=============================================================================
 * Copyright (c) 2019, Ezequiel Urdaniz <urdanizezequiel@gmail.com>
 * All rights reserved.
 * Date: 2019/11/11
 * Version: 1.2
 *===========================================================================*/

/*==================[inlcusiones]============================================*/


#include "BLUE_USB_UART.h"
#include "LED_OUT.h"
#include "string.h"


/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

SemaphoreHandle_t Evento_Recibe, Evento_Save; 				//Semaphore de tareas

TickType_t tiempo_inic_ciclo;								//Variable para uso de espera.

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;  //Para configurar los mensajes por monitor


/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

	//----------- TASK CONTROL de OUT -----------------------
void ControlOut (void* taskParmPtr){

	tiempo_inic_ciclo = xTaskGetTickCount();	//Tiempo inicio de ciclo.

	while(TRUE){
	    if( control_Out == 1 ){
	    	uartWriteString(UART_PC, "r" );	//Comando de inicializacion de conexion.
	    	gpioWrite( LEDG, ON );			//Enciendo LED GREEN. Establecer conexion.
	    }
	    if( control_Out == 2 ){
	    	uartWriteString(UART_PC, "o" );	//Comando de apagado de conexion.
	    	gpioWrite( LEDG, OFF);	//Apago LED GREEN. Finalizar conexion.
	    }
	    if( control_Out == 3 ){
	    	uartWriteString(UART_PC, "D5" );	//Comando de solicitud de medicion.
	    	gpioWrite( LED1, ON );	//Enciendo LED1. Medir Campo.
	       	vTaskDelayUntil( &tiempo_inic_ciclo, 200 / portTICK_RATE_MS); // Espera 200ms
	    	gpioWrite( LED1, OFF);	//Apago LED1. Fin de operacion Campo.
	    }
	    if( control_Out == 4 ){
	    	uartWriteString(UART_PC, "i" );	//Comando de solicitud identificación.
	    	gpioWrite( LED2, ON );	//Enciendo LED2. Estatus del conexion.
	    	vTaskDelayUntil( &tiempo_inic_ciclo, 200 / portTICK_RATE_MS); // Espera 200ms
	    	gpioWrite( LED2, OFF);	//Apago LED2. Fin de operacion estatus.
	    }

	}
}


	//----------- TASK CONTROL de TECLA -----------------------
void ControlTecla(void* taskParmPtr){

	while(TRUE){
		if (!gpioRead( TEC1 )){ //(!false) tecla presionada (!true) no presionada.
			xSemaphoreTake(Evento_Save,portMAX_DELAY);
			control_Out = 1;	//save solicitud.
			xSemaphoreGive(Evento_Save);
			}

		if (!gpioRead( TEC2 )){ //(!false) tecla presionada (!true) no presionada.
			xSemaphoreTake(Evento_Save,portMAX_DELAY);
			control_Out = 2;	//save solicitud.
			xSemaphoreGive(Evento_Save);
		   }

		if (!gpioRead( TEC3 )) { //(!false) tecla presionada (!true) no presionada.
			xSemaphoreTake(Evento_Save,portMAX_DELAY);
			control_Out = 3;	//save solicitud.
			xSemaphoreGive(Evento_Save);
	       }

		if (!gpioRead( TEC4 )) { //(!false) tecla presionada (!true) no presionada.
			xSemaphoreTake(Evento_Save,portMAX_DELAY);
			control_Out = 4;	//save solicitud.
			xSemaphoreGive(Evento_Save);
		   }
	}
}


/*==================[fin del archivo]========================================*/
