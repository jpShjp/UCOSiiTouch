/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : control_dac.h
* Author             : yiya team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : .
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_DAC_H
#define __CONTORL_DAC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* GPIO Pin sources ----------------------------------------------------------*/
#define AO_SELECT_MODE_1   1 //voltage output
#define AO_SELECT_MODE_2   2 //current output

/* Exported macro ------------------------------------------------------------*/
#define PWM_8K						899  //72000/900 = 8K?
#define PWM_10K           719

/* Exported functions ------------------------------------------------------- */void DO_SetBits(u8 DO_Channel_x);
extern void BSP_DAC_Config(void);
extern void BSP_PWM_DutyCycle(u8 channelx,s16 dutycycle);
extern void BSP_AO_Select_Mode(u8 channelx,u8 mode);

#endif /* __CONTROL_DAC_H */

/******************* (C) COPYRIGHT YIYA TEAM *****END OF FILE****/
