/*=============================================================================
 * Copyright (c) 2019, Ezequiel Urdaniz <urdanizezequiel@gmail.com>
 * All rights reserved.
 * Date: 2019/12/18
 * Version: 1.0
 *===========================================================================*/


/*==================[inlcusiones]============================================*/


#include "Main_RTOS_PC.h"
#include "TECLA.h"


/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

portTickType TiempoTec1,TiempoTec2,TiempoTec3,TiempoTec4,xLastWakeTime;	 //variables tiempos.
uint8_t EstadoTec1 = UP;		//Variables estado tecla.
uint8_t EstadoTec2 = UP;		//Variables estado tecla.
uint8_t EstadoTec3 = UP;		//Variables estado tecla.
uint8_t EstadoTec4 = UP;		//Variables estado tecla.

/*==================[definiciones de datos externos]=========================*/
SemaphoreHandle_t Mutex_t_pulsado;			//Semaphore tarea
uint8_t control_Out;						//Variable de control externa.

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

	//-------------TASK TECLA ANTIREBOTE --------------
void Tecla( void* taskParmPtr ){
	while( TRUE ){

		xLastWakeTime = xTaskGetTickCount();  //Registro inicio de ciclo.

		fsmButtonUpdate( TEC1, 1, &EstadoTec1, &TiempoTec1);  //Estado TEC1.
	 	vTaskDelay( 1 / portTICK_RATE_MS );

	 	fsmButtonUpdate( TEC2, 2, &EstadoTec2, &TiempoTec2);  //Estado TEC2.
	 	vTaskDelay( 1 / portTICK_RATE_MS );

	 	fsmButtonUpdate( TEC3, 3, &EstadoTec3, &TiempoTec3);  //Estado TEC3.
	 	vTaskDelay( 1 / portTICK_RATE_MS );

	 	fsmButtonUpdate( TEC4, 4, &EstadoTec4, &TiempoTec4);  //Estado TEC4.
	 	vTaskDelay( 1 / portTICK_RATE_MS );

	 	// Envio la tarea al estado bloqueado.
	 	vTaskDelayUntil( &xLastWakeTime, (80/ portTICK_RATE_MS) );
	}
}

	// ------------FUNCIÓN UPDATE ESTADO DE TECLA -------------------
void fsmButtonUpdate(gpioMap_t tecla, uint8_t Control, uint8_t* Estadotec, portTickType* TiempoTec){

		switch (*Estadotec){					//Switch estado de tecla
		 case UP:
	  		  if(0 == gpioRead(tecla)){				//Read tecla
	  			*Estadotec = FALLING;				//Guardo estado tecla.
	  			*TiempoTec = xTaskGetTickCount();   //Registro tiempo.
	  		  }
	  		  break;

	  	 case FALLING:
	  		 if(0  == gpioRead(tecla)){				//Read tecla
	  			 if (((xTaskGetTickCount()-*TiempoTec)*portTICK_RATE_MS)>=20){ //Tiempo antirebote.
	  				*Estadotec = DOWN;				//Guardo estado tecla.
	  			 }
	  		 }
	  		 else {
	  			*Estadotec = UP;					//Guardo estado tecla.
	  		 }
	  	break;

	  	 case DOWN:
	  		if(1 == gpioRead(tecla)){				//Read tecla
	  			*Estadotec = RISING;			  	//Guardo estado tecla.
	  			*TiempoTec = xTaskGetTickCount();	//Registro tiempo.
	  		}
	  		else {				 								//(Evento DOWN)
	  			xSemaphoreTake(Mutex_t_pulsado,portMAX_DELAY); //Inicio zona critica.
	  			control_Out = Control;							//Variable de control.
	  			xSemaphoreGive(Mutex_t_pulsado);				//Fin de zona critica.
	  		}
	  	 break;

	  	 case RISING:
	  		 if(1  == gpioRead(tecla)){				//Read tecla
				 if (((xTaskGetTickCount()-*TiempoTec)*portTICK_RATE_MS)>=20){ //Tiempo antirebote.
					 *Estadotec = UP;				//Guardo estado tecla.
				 }
			}
			 else {
				 *Estadotec = DOWN; 			//Guardo estado tecla.
			}
		 break;

	  	 default:								//Case default.
	  		*Estadotec = UP;					//Reseteo estado tecla.
	  	 break;
	  }
}


/*==================[fin del archivo]========================================*/
