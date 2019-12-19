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

SemaphoreHandle_t Mutex_Save, Mutex_Recibe, Mutex_Read; 	//Semaphore de tareas

TickType_t tiempo_inic_ciclo;								//Variable para uso de espera.

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;  //Para configurar los mensajes por monitor


/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

	//----------- TASK CONTROL de OUT -----------------------
void ControlOut (void* taskParmPtr){

	while(TRUE){

	    if( control_Out == 1 ){
		    xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Read
	    	gpioWrite( LEDG, ON );					//Enciendo LED GREEN. Establecer conexion.
	    	control_Out =0;								//Reseteo de variable de control.
		    xSemaphoreGive(Mutex_Save); 			//Fin zona critica.
	    }
	    if( control_Out == 2 ){
		    xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Read
	    	gpioWrite( LEDG, OFF);					//Apago LED GREEN. Finalizar conexion.
	    	control_Out =0;								//Reseteo de variable de control.
		    xSemaphoreGive(Mutex_Save); 			//Fin zona critica.
	    }
	    if( control_Out == 3 ){
		    xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Read
		    gpioWrite( LED2, ON );					//Enciendo LED2. Medir Campo.
	    	control_Out =0;								//Reseteo de variable de control.
		    xSemaphoreGive(Mutex_Save); 			//Fin zona critica.
	    }
	    if( control_Out == 4 ){
		    xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Read
		    gpioWrite( LED3, ON );					//Enciendo LED3. Estatus del conexion.
	    	control_Out =0;								//Reseteo de variable de control.
		    xSemaphoreGive(Mutex_Save);				 //Fin zona critica.
	    }

	    vTaskDelay( 10/ portTICK_RATE_MS ); 			//Estado Blocked para liberacion de recurso.

	}
}


	//----------- TASK CONTROL de TECLA -----------------------
void ControlTecla(void* taskParmPtr){

	while(TRUE){
		if (!gpioRead( TEC1 )){ //(!false) tecla presionada.
			xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Save
			control_Out = 1;						//Solicitud de inicializacion de conexion.
	      	data_ex_pc = 0;					//Reseteo de variable
			xSemaphoreGive(Mutex_Save);				//Salida de zona critica.
			vTaskDelay( 40 / portTICK_RATE_MS ); 			//Estado Blocked para liberacion de recurso.
			}

		if (!gpioRead( TEC2 )){ //(!false) tecla presionada.
			xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Save
			control_Out = 2;				//Solicitud de apagado de conexion.
	      	data_ex_pc=0;					//Reseteo de variable
			xSemaphoreGive(Mutex_Save);				//Salida de zona critica.
			vTaskDelay( 40 / portTICK_RATE_MS ); 			//Estado Blocked para liberacion de recurso.
		   }


		if (!gpioRead( TEC3 )) { //(!false) tecla presionada.
			xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Save
	      	control_Out= 3;					//Solicitud de medicion.
	      	control_OutD= 3;				//Solicitud de envio de dato.
	      	data_ex_pc=0;					//Reseteo de variable
	      	xSemaphoreGive(Mutex_Save);				//Salida de zona critica.
	      	vTaskDelay( 40 / portTICK_RATE_MS ); 	//Estado Blocked para liberacion de recurso.
	       }

		if (!gpioRead( TEC4 )) { //(!false) tecla presionada.
			xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Save
	        control_Out= 4;			 		//Solicitud identificación.
	        control_OutD= 4;				//Solicitud de envio de ID.
	      	data_ex_pc=0;					//Reseteo de variable
			xSemaphoreGive(Mutex_Save);				//Salida de zona critica.
			vTaskDelay( 40 / portTICK_RATE_MS ); 			//Estado Blocked para liberacion de recurso.
		   }
	}
}


/*==================[fin del archivo]========================================*/
