/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : control_do.h
* Author             : yiya team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file contains all the functions prototypes for the
*                      control_do firmware library.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_ADC_H
#define __CONTORL_ADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* GPIO Pin sources ----------------------------------------------------------*/

#define AI_SELECT_GAIN_1   1
#define AI_SELECT_GAIN_2   2
#define AI_SELECT_GAIN_3   3
#define AI_SELECT_GAIN_4   4
#define AI_SELECT_GAIN_5   5

/* Exported macro ------------------------------------------------------------*/
#define ADC_FILITER_NUMBER	  10
#define ADC_AOFeedBack_1	  3
#define ADC_AOFeedBack_2	  4
#define ADC_AOFeedBack_3	  5
#define ADC_AOFeedBack_4	  6
#define ADC_AOFeedBack_5	  7
#define ADC_AOFeedBack_6	  0
#define ADC_AI_1						1
#define ADC_AI_2						2

extern u16 ADC_ValueAfterFilter[16];

/* Exported functions ------------------------------------------------------- */void DO_SetBits(u8 DO_Channel_x);
extern void BSP_ADC_Config(void);
extern void BSP_AI_SelectChannel(u8 channelx);
extern void BSP_AI_Select_Mode(u8 mode);
extern void BSP_AI_Select_Gain(u8 channelx,u8 AI_SELECT_GAIN_x);
extern void BSP_MeasurResist_CurrentOut(u8 currentx);
extern void BSP_MeasurResist_CurrentOut_value(u16 value);
extern u16 Get_ADC1(u8 chx);
extern u16 Get_ADC3(u8 chx);
#endif /* __CONTROL_ADC_H */

/******************* (C) COPYRIGHT YIYA TEAM *****END OF FILE****/
