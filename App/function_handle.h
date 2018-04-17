/******************** (C) COPYRIGHT 2014 YIYA ********************
* File Name          : function_handle.h
* Author             : yiya team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file contains all the functions prototypes for the
*                      control_main firmware library.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FUNCTION_HANDLE_H
#define __FUNCTION_HANDLE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* GPIO Pin sources ----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define Filter_num		200
/* Exported functions ------------------------------------------------------- */
extern void P_Function_DO(void);
extern void P_Function_DI(void);
extern void P_Function_AO(void);
extern void P_Function_AI(void);
extern void P_Function_RunStatus(u16 blinktimeMS);

extern void TPK8control_init(void);
extern void TPK8control(void);
extern void DO_reload(void);
extern void DO_store_lo(void);
extern void DO_store_hi(void);
extern void DO_store(void);
#endif /* __function_handle_H */

/******************* (C) COPYRIGHT YIYA TEAM *****END OF FILE****/
