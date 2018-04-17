/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : control_comm.h
* Author             : yiya team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file contains all the functions prototypes for the
*                      control_comm firmware library.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_COMM_H
#define __CONTORL_COMM_H

/* Includes ------------------------------------------------------------------*/
#include "control_struct.h"
#include "stm32f10x_it.h" 
//#include "stm32f10x_nvic.h"
#include "stm32f10x_rcc.h"
/* Exported defines ------------------------------------------------------------*/

/* The comm3 define---------------------*/
#define COMM3_GPIO					GPIOB 
#define COMM3_GPIO_CLK				RCC_APB2Periph_GPIOB
#define COMM3_GPIO_RxPin			GPIO_Pin_11
#define COMM3_GPIO_TxPin			GPIO_Pin_10
#define COMM3_Ctrl_GPIO				GPIOE
#define COMM3_Ctrl_GPIO_CLK		    RCC_APB2Periph_GPIOE
#define COMM3_Ctrl_Pin				GPIO_Pin_15

 /* The comm2 define--------------------*/
#define COMM2_GPIO					GPIOD 
#define COMM2_GPIO_CLK				RCC_APB2Periph_GPIOD
#define COMM2_GPIO_RxPin			GPIO_Pin_6
#define COMM2_GPIO_TxPin			GPIO_Pin_5
#define COMM2_Ctrl_GPIO				GPIOD
#define COMM2_Ctrl_GPIO_CLK		    RCC_APB2Periph_GPIOD
#define COMM2_Ctrl_Pin				GPIO_Pin_4

 /* The comm1 define--------------------*/
#define COMM1_GPIO					GPIOA 
#define COMM1_GPIO_CLK				RCC_APB2Periph_GPIOA
#define COMM1_GPIO_RxPin			GPIO_Pin_10
#define COMM1_GPIO_TxPin			GPIO_Pin_9
#define COMM1_Ctrl_GPIO				GPIOA
#define COMM1_Ctrl_GPIO_CLK		    RCC_APB2Periph_GPIOA
#define COMM1_Ctrl_Pin				GPIO_Pin_11

 /* The comm state led define------------*/
#define COMM_State_LED_GPIO			GPIOF
#define COMM_State_LED_GPIO_CLK 	RCC_APB2Periph_GPIOF
#define COMM3_Rx_LED				GPIO_Pin_5
#define COMM3_Tx_LED				GPIO_Pin_4
#define COMM2_Rx_LED				GPIO_Pin_3
#define COMM2_Tx_LED				GPIO_Pin_2
#define COMM1_Rx_LED				GPIO_Pin_1
#define COMM1_Tx_LED				GPIO_Pin_0

/* Exported constants --------------------------------------------------------*/
extern COMM_DATA COMM1_ReceBuf;
extern COMM_DATA COMM1_SendBuf;
extern COMM_DATA COMM2_ReceBuf;
extern COMM_DATA COMM2_SendBuf;
extern COMM_DATA COMM3_ReceBuf;
extern COMM_DATA COMM3_SendBuf;
/* GPIO Pin sources ----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void BSP_Comm_Config(void);
extern u16 Get_CRC(u8 *puchMsg, u16 usDataLen);
extern u16 BSP_COMM_Rece(u8 commx);
extern void BSP_COMM_Send(COMM_DATA pdatastruct);
extern void BSP_Comm_Rx_Mode(u8 commx);
extern void BSP_Comm_Tx_Mode(u8 commx);
extern void BSP_Comm_Idle_Mode(u8 commx);
#endif /* __CONTROL_COMM_H */

/******************* (C) COPYRIGHT YIYA TEAM *****END OF FILE****/
