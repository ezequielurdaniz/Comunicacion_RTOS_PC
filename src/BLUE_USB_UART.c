/*=============================================================================
 * Copyright (c) 2019, Ezequiel Urdaniz <urdanizezequiel@gmail.com>
 * All rights reserved.
 * Date: 2019/11/11
 * Version: 1.2
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "BLUE_USB_UART.h"
#include "string.h"


/*==================[definiciones y macros]==================================*/


//*==================[definiciones de datos internos]==========================*/
SemaphoreHandle_t Mutex_Save, Mutex_Recibe, Mutex_Read; 	//Semaphore de tareas

uint8_t data_in = 0; 			//variable de dato recibido.
uint8_t data_ex_pc = 0;			//variable de dato PC.
uint8_t data_ex_bl = 0;			//variable de dato BLE.
uint8_t control_Out = 0;			//variable de control de salidas.
uint8_t control_OutD = 0;			//variable de control de indices.

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;  //Para configurar los mensajes por monitor

/*==================[funcion principal]======================================*/

// ------------------- FUNCIÓN de CONFIG de UART  -------------------

void init_UART_USB_BLE (void){

	 // Inicializar UART_USB para conectar a la PC
	 uartConfig( UART_PC, BAUDIO );
	 uartWriteString( UART_PC, "UART_PC configurada.\r\n" );

	 // Inicializar UART_232 para conectar a BLUETOOTH
	 uartConfig( UART_BLUETOOTH, BAUDIO );
	 uartWriteString( UART_PC, "UART_BLUETOOTH configurada.\r\n" );

	 //Testet del modulo BLE HM10
	 if( hm10bleTest( UART_BLUETOOTH)){ //hm10bleTest( UART_BLUETOOTH)
		 uartWriteString( UART_PC, "Modulo BLE HM10 conectado correctamente.\r\n" );
	   }
	   else{
	     uartWriteString( UART_PC, "No funciona.\r\n" );
	   }

}

	// ------------FUNCIÓN TESTEO HM10 -------------------
bool_t hm10bleTest( int32_t uart ){
   uartWriteString( uart, "AT\r\n" ); // Enviar comandos AT\r\n a BLE HM10
   return waitForReceiveStringOrTimeoutBlocking( uart,"OK\r\n",strlen("OK\r\n"),1000); //1000 Espero un OK\r\n o Timeout
  }

	//-------------TASK RECIBIR de UART PC --------------
void Recibe_PC (void* taskParmPtr){
	while(TRUE){
		xSemaphoreTake(Mutex_Recibe,portMAX_DELAY);		//Inicio zona critica. Recibe
 		if( uartReadByte(UART_PC, &data_in)){				//Read dato en UART_PC.
			data_ex_pc = data_in;							//Save dato.
		}
		xSemaphoreGive(Mutex_Recibe);					//Salida de zona critica.
 		vTaskDelay( 5/ portTICK_RATE_MS ); 		//Estado Blocked para liberacion de recurso.
	}
}
	//----------- TASK RECIBIR de UART BLE --------------
void Recibe_BLE (void* taskParmPtr){
	while(TRUE){
		xSemaphoreTake(Mutex_Recibe,portMAX_DELAY); 	//Inicio zona critica. Recibe
		if( uartReadByte(UART_BLUETOOTH, &data_in)){		//Read dato en UART_BLE.
			if(data_in != '\0'){							// Dato recibido distino de <break>
				data_ex_bl = data_in;						//Save dato.
			}
		}
		xSemaphoreGive(Mutex_Recibe);					//Salida de zona critica.
		vTaskDelay( 5/ portTICK_RATE_MS ); 			//Estado Blocked para liberacion de recurso.
	}
}

	//----------- TASK CONTROL de DATA de BLE------------------
void ControlDataBLE (void* taskParmPtr){
	while(TRUE){
		if( data_ex_bl == 'c'){
			xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Save
			control_Out= 1;						//Solicitud de inicializacion de sonda.
			data_ex_bl=0;						//Reseteo de variable
			xSemaphoreGive(Mutex_Save);				//Salida de zona critica.
	      }

		if( data_ex_bl == 'a'){
			xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Save
			control_Out = 2;				//Solicitud de apagado de sonda.
			data_ex_bl=0;					//Reseteo de variable
			xSemaphoreGive(Mutex_Save);				//Salida de zona critica.
	      }

		if( data_ex_bl == 'b'){
			xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Save
	      	control_Out= 3;					//Solicitud de medicion campo electrico.
	      	control_OutD= 3;				//Solicitud de envio de dato.
	      	data_ex_bl=0;					//Reseteo de variable
	      	data_ex_pc=0;					//Reseteo de variable
	      	xSemaphoreGive(Mutex_Save);				//Salida de zona critica.
	      }
		if( data_ex_bl == 'd'){
			xSemaphoreTake(Mutex_Save,portMAX_DELAY); //Inicio zona critica. Save
	      	control_Out= 4;			 		//Solicitud identificación ID.
	        control_OutD= 4;				//Solicitud de envio de ID.
	    	data_ex_bl=0;					//Reseteo de variable
	    	data_ex_pc=0;					//Reseteo de variable
	    	xSemaphoreGive(Mutex_Save);			//Salida de zona critica.

	      }
    	vTaskDelay( 5 / portTICK_RATE_MS ); //Estado Blocked para liberacion de recurso.
	}
}

//----------- TASK CONTROL de DATA de PC ------------------
void ControlDataPC (void* taskParmPtr){
	while(TRUE){
		if(data_ex_pc !=0){
		switch (control_OutD){				//Switch estado de variable control_outD
				case 3:
						uartWriteString(UART_BLUETOOTH, "Campo:" ); //String de identificación de medición.
						uartWriteByte( UART_BLUETOOTH, data_ex_pc );//Envio medición de campo a UART_BLE
						xSemaphoreTake(Mutex_Save,portMAX_DELAY); 	//Inicio zona critica. Read_Enviar
						control_OutD=0;								//Reseteo de variable de control.
						data_ex_pc =0;							//Reseteo de variable de control.
						xSemaphoreGive(Mutex_Save);					//Salida de zona critica.
						gpioWrite(LED2, OFF);						//Apago LED2. Fin de operacion Campo.
			    	break;

				case 4:
						uartWriteString(UART_BLUETOOTH, "ID:" );	//String de identificación de ID.
						uartWriteByte( UART_BLUETOOTH, data_ex_pc ); //Envio numero  de identificación a UART_BLE
						xSemaphoreTake(Mutex_Save,portMAX_DELAY); 	//Inicio zona critica. Read_Enviar
						control_OutD=0;								//Reseteo de variable de control.
						data_ex_pc =0;
						xSemaphoreGive(Mutex_Save);					//Salida de zona critica.
						gpioWrite(LED3, OFF);						//Apago LED3. Fin de operacion estatus.
					break;

			  	default:
			  		 //Sin Acción.
			  		break;
		}
	}

		vTaskDelay( 10 / portTICK_RATE_MS ); 			//Estado Blocked para liberacion de recurso.
  }
}

/*==================[fin del archivo]========================================*/

