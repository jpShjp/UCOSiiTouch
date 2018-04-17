/******************** (C) COPYRIGHT 2014 YIYA ********************
* File Name          : control_struct.h
* Author             : yiya team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file contains all the functions prototypes for the
*                      control_struct firmware library.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_STRUCT_H
#define __CONTORL_STRUCT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
/* Exported types ------------------------------------------------------------*/

typedef struct{
	u8 CommChannel; //communication channal
	u16 DataCount; //received data length
	u16 DataLong; 
	u8 DataBuf[100];
	u8 FrameDelayMS;
	u8 OK;
}COMM_DATA;


/* Exported constants --------------------------------------------------------*/
/* GPIO Pin sources ----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#endif /* __CONTROL_STRUCT_H */

/******************* (C) COPYRIGHT YIYA TEAM *****END OF FILE****/
