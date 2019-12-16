/*=============================================================================
 * Copyright (c) 2019, Ezequiel Urdaniz <urdanizezequiel@gmail.com>
 * All rights reserved.
 * Date: 2019/11/11
 * Version: 1.2
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

// Includes de FreeRTOS
#include "BLUE_USB_UART.h"
#include "LED_OUT.h"
#include "task.h"		//Api de control de tareas y temporización
#include "semphr.h"		//Api de sincronización (sem y mutex)

// sAPI header
#include "sapi.h"
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
	    	//control_Out =0;
	    }
	    if( control_Out == 2 ){
	    	uartWriteString(UART_PC, "o" );	//Comando de apagado de conexion.
	    	gpioWrite( LEDG, OFF);	//Apago LED GREEN. Finalizar conexion.
	    	//control_Out =0;
	    }
	    if( control_Out == 3 ){
	    	uartWriteString(UART_PC, "D5" );	//Comando de solicitud de medicion.
	    	gpioWrite( LED1, ON );	//Enciendo LED1. Medir Campo.
	    	vTaskDelayUntil( &tiempo_inic_ciclo, 200 / portTICK_RATE_MS); // Espera 200ms
	    	gpioWrite( LED1, OFF);	//Apago LED1. Fin de operacion Campo.
	    	//control_Out =0;
	  	}
	    if( control_Out == 4 ){
	    	uartWriteString(UART_PC, "i" );	//Comando de solicitud identificación.
	    	gpioWrite( LED2, ON );	//Enciendo LED2. Estatus del conexion.
	    	vTaskDelayUntil( &tiempo_inic_ciclo, 200 / portTICK_RATE_MS); // Espera 200ms
	    	gpioWrite( LED2, OFF);	//Apago LED2. Fin de operacion estatus.
	    	//control_Out =0;
	    }

	}
}


	//----------- TASK CONTROL de TECLA -----------------------
void ControlTecla(void* taskParmPtr){

	uint8_t estado = 0;	//variable estado de tecla.

	while(TRUE){

		if (!gpioRead( TEC1 )){ //(!false) tecla presionada (!true) no presionada.
			estado = 1;		//Solicitud de inicializacion de conexion.
			}

		if (!gpioRead( TEC2 )){ //(!false) tecla presionada (!true) no presionada.
			estado = 2;		//Solicitud de apagado de conexion.
		   }

		if (!gpioRead( TEC3 )) { //(!false) tecla presionada (!true) no presionada.
			estado = 3;		 //Solicitud de medicion.
	       }

		if (!gpioRead( TEC4 )) { //(!false) tecla presionada (!true) no presionada.
			estado = 4;		 //Solicitud identificación.
		   }

        xSemaphoreTake(Evento_Save,portMAX_DELAY);
        control_Out = estado;	//save solicitud.
		xSemaphoreGive(Evento_Save);

	}
}


/*==================[fin del archivo]========================================*/
