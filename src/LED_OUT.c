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

SemaphoreHandle_t Mutex_Save; 	//Semaphore de tareas

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;  //Para configurar los mensajes por monitor


/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

//----------- TASK CONTROL de OUT -----------------------
void ControlOut (void* taskParmPtr){
	while(TRUE){

	    if( control_Out == 1 ){						//Establecer conexion con sonda.
		    gpioWrite( LEDG, ON );					//Enciendo LED GREEN.
	    	uartWriteString(UART_PC, "r\r\n" );		//Comando de SONDA ON.
	    	xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Save
	    	control_Out =0;								//Reseteo de variable de control.
		    xSemaphoreGive(Mutex_Save); 			//Fin zona critica.
	    }
	    if( control_Out == 2 ){						//Finalizar conexion con sonda.
		    gpioWrite( LEDG, OFF);					//Apago LED GREEN.
		    xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Read
	    	uartWriteString(UART_PC, "o\r\n" ); 	//Comando de SONDA OFF.
	    	control_Out =0;								//Reseteo de variable de control.
		    xSemaphoreGive(Mutex_Save); 			//Fin zona critica.
	    }
	    if( control_Out == 3 ){						//Medir Campo.
		    gpioWrite( LED2, ON );					//Enciendo LED2.
		    uartWriteString(UART_PC, "D5\r\n" ); //Comando de Medición de Campo Electrico Sonda.
		    xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Read
	      	control_OutD= 3;							//Solicitud de envio de dato.
	      	data_ex_pc=0;								//Reseteo de variable
	    	control_Out =0;								//Reseteo de variable de control.
		    xSemaphoreGive(Mutex_Save); 			//Fin zona critica.
	    }
	    if( control_Out == 4 ){						//Identificacion de sonda.
		    gpioWrite( LED3, ON );						//Enciendo LED3.
		    uartWriteString(UART_PC, "i\r\n" ); 		//Comando de ID de Sonda.
		    xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Read
	    	control_Out =0;								//Reseteo de variable de control.
	        control_OutD= 4;							//Solicitud de envio de ID.
	      	data_ex_pc=0;								//Reseteo de variable
		    xSemaphoreGive(Mutex_Save);				 //Fin zona critica.
	    }

	    vTaskDelay( 10/ portTICK_RATE_MS ); 		//Estado Blocked para liberacion de recurso.

	}
}

/*==============================[fin del archivo]========================================*/
