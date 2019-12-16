/*=============================================================================
 * Copyright (c) 2019, Ezequiel Urdaniz <urdanizezequiel@gmail.com>
 * All rights reserved.
 * Date: 2019/11/11
 * Version: 1.2
 *===========================================================================*/



/*==================[inlcusiones]============================================*/

#include "BLUE_USB_UART.h"
#include "semphr.h"			//(sem y mutex)
#include "string.h"


/*==================[definiciones y macros]==================================*/


//*==================[definiciones de datos internos]==========================*/
SemaphoreHandle_t Evento_Recibe, Evento_Save; 			//Semaphore de tareas

uint8_t data_in = 0; 			//variable de dato recibido.
uint8_t data_ex_pc = 0;			//variable de dato PC.
uint8_t data_ex_bl = 0;			//variable de dato BLE.
bool_t  test = FALSE;
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
	 if( hm10bleTest( UART_BLUETOOTH )){ //
		 uartWriteString( UART_PC, "Modulo BLE HM10 conectado correctamente.\r\n" );
	   }
	   else{
	     uartWriteString( UART_PC, "No funciona.\r\n" );
	   }

}
	// ------------FUNCIÓN TESTEO HM10 -------------------
bool_t hm10bleTest( int32_t uart ){
   //uartWriteString( uart, "AT\r\n" ); // Enviar comandos AT\r\n a BLE HM10
   //return waitForReceiveStringOrTimeoutBlocking( uart,"OK\r\n", strlen("OK\r\n"),500); //1000 Espero un OK\r\n o Timeout
   return TRUE;
}

	//-------------TASK RECIBIR de UART PC --------------
void Recibe_PC (void* taskParmPtr){

	while(TRUE){
		if( uartReadByte(UART_PC, &data_in)){			//Ready dato en UART_PC.

			xSemaphoreTake(Evento_Recibe,portMAX_DELAY);
			data_ex_pc = data_in;						//Save dato.
			xSemaphoreGive(Evento_Recibe);
			}
	}
}
	//----------- TASK RECIBIR de UART BLE --------------
void Recibe_BLE (void* taskParmPtr){

	while(TRUE){
		if( uartReadByte(UART_BLUETOOTH, &data_in)){	//Ready dato en UART_BLE.

			xSemaphoreTake(Evento_Recibe,portMAX_DELAY);
			data_ex_bl = data_in;						//Save dato.
			xSemaphoreGive(Evento_Recibe);

			}
	}
}

	//----------- TASK CONTROL de DATA de BLE------------------
void ControlDataBLE (void* taskParmPtr){

	while(TRUE){
		if( data_ex_bl == 'C' ){
			xSemaphoreTake(Evento_Save,portMAX_DELAY);
			control_Out= 1;			//Solicitud de inicializacion de conexion.
	        xSemaphoreGive(Evento_Save);
	      }
		if( data_ex_bl == 'A' ){
			xSemaphoreTake(Evento_Save,portMAX_DELAY);
			control_Out = 2;		//Solicitud de apagado de conexion.
	        xSemaphoreGive(Evento_Save);
	      }
		if( data_ex_bl == 'B' ){
	      	xSemaphoreTake(Evento_Save,portMAX_DELAY);
	      	control_Out= 3;			//Solicitud de medicion.
	        xSemaphoreGive(Evento_Save);
	      }
		if( data_ex_bl == 'D' ){
	        xSemaphoreTake(Evento_Save,portMAX_DELAY);
	        control_Out= 4;			 //Solicitud identificación.
	        xSemaphoreGive(Evento_Save);
	      }
	}
}

	//----------- TASK CONTROL de DATA de PC ------------------
void ControlDataPC (void* taskParmPtr){

	while(TRUE){

		if( data_ex_pc != 0 ){

			switch (control_Out){			//Switch estado de control_out
				case 3:
					uartWriteString(UART_BLUETOOTH, "Campo:" ); //Envio a BLE la medición.
					uartWriteByte( UART_BLUETOOTH, data_ex_pc );
					break;

				case 4:
					uartWriteString(UART_BLUETOOTH, "ID:" );
					uartWriteByte( UART_BLUETOOTH, data_ex_pc ); //Envio a BLE la identificación..
					break;

			  	default:
			  		 //Sin Acción.
			  		break;
			}
		}
	}
}


/*==================[fin del archivo]========================================*/

