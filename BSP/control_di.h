/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : control_di.h
* Author             : yiya team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file contains all the functions prototypes for the
*                      control_di firmware library.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_DI_H
#define __CONTORL_DI_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* GPIO Pin sources ----------------------------------------------------------*/
#define DI_Channel_1            1U
#define DI_Channel_2            2U
#define DI_Channel_3            3U
#define DI_Channel_4            4U
#define DI_Channel_5            5U
#define DI_Channel_6            6U
#define DI_Channel_7            7U
#define DI_Channel_8            8U
#define DI_Channel_9            9U
#define DI_Channel_10           10U
#define DI_Channel_11           11U
#define DI_Channel_12           12U
#define DI_Channel_13           13U
#define DI_Channel_14           14U
#define DI_Channel_15           15U
#define DI_Channel_16           16U
/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
extern void Control_GPI_Config(void);
extern u16 DI_ReadBits(u8 DI_Channel_x);
#endif /* __CONTROL_DO_H */

/******************* (C) COPYRIGHT YIYA TEAM *****END OF FILE****/
