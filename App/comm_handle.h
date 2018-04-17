/******************** (C) COPYRIGHT 2014 YIYA ********************
* File Name          : comm_handle.h
* Author             : yiya team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file contains all the functions prototypes for the
*                      control_main firmware library.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMM_HANDLE_H
#define __COMM_HANDLE_H
#include "ucos_ii.h"
//include "stm32f10x_it.h"
//include "control_comm.h"
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* GPIO Pin sources ----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void P_Comm3_Handle(void);
extern void P_Comm2_Handle(void);
extern void P_Comm1_Handle(void);
extern void P_Comm1_Send_Handle (void);
extern void MMSenDFuncode03Frame(u8 SlaveAddress,u16 RegStar_Address,u16 RegLen);

#endif /* __comm_handle_H */

/******************* (C) COPYRIGHT YIYA TEAM *****END OF FILE****/
