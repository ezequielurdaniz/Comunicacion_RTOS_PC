/*=============================================================================
 * Copyright (c) 2019, Ezequiel Urdaniz <urdanizezequiel@gmail.com>
 * All rights reserved.
 * Date: 2019/11/11
 * Version: 1.2
 *===========================================================================*/

#ifndef MIS_PROGRAMAS_TP_SOLO_PWM_INC_PWM_H_
#define MIS_PROGRAMAS_TP_SOLO_PWM_INC_PWM_H_


/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/

void ControlOut (void* taskParmPtr);  // Prototipo de tarea control de salida.

void ControlTecla(void* taskParmPtr); // Prototipo de tarea control de tecla.


/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/





#endif /* MIS_PROGRAMAS_TP_SOLO_PWM_INC_PWM_H_ */
