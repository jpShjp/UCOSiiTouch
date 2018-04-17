/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : control_do.h
* Author             : yiya team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file contains all the functions prototypes for the
*                      control_do firmware library.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_DO_H
#define __CONTORL_DO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* GPIO Pin sources ----------------------------------------------------------*/
#define DO_Channel_1            1U
#define DO_Channel_2            2U
#define DO_Channel_3            3U
#define DO_Channel_4            4U
#define DO_Channel_5            5U
#define DO_Channel_6            6U
#define DO_Channel_7            7U
#define DO_Channel_8            8U
#define DO_Channel_9            9U
#define DO_Channel_10           10U
#define DO_Channel_11           11U
#define DO_Channel_12           12U
#define DO_Channel_13           13U
#define DO_Channel_14           14U
#define DO_Channel_15           15U
#define DO_Channel_16           16U
/* Exported macro ------------------------------------------------------------*/
/* The first digital-output channel define------------*/
#define DO_1_GPIO							GPIOB
#define DO_1_GPIO_CLK					RCC_APB2Periph_GPIOB
#define DO_1_PIN							GPIO_Pin_15

/* The second digital-output channel define------------*/
#define DO_2_GPIO							GPIOB
#define DO_2_GPIO_CLK					RCC_APB2Periph_GPIOB
#define DO_2_PIN							GPIO_Pin_14

/* The third digital-output channel define------------*/
#define DO_3_GPIO							GPIOB
#define DO_3_GPIO_CLK					RCC_APB2Periph_GPIOB
#define DO_3_PIN							GPIO_Pin_13

/* The fourth digital-output channel define------------*/
#define DO_4_GPIO							GPIOB
#define DO_4_GPIO_CLK					RCC_APB2Periph_GPIOB
#define DO_4_PIN							GPIO_Pin_12

/* The fifth digital-output channel define------------*/
#define DO_5_GPIO							GPIOE
#define DO_5_GPIO_CLK					RCC_APB2Periph_GPIOE
#define DO_5_PIN							GPIO_Pin_14

/* The sixth digital-output channel define------------*/
#define DO_6_GPIO							GPIOE
#define DO_6_GPIO_CLK					RCC_APB2Periph_GPIOE
#define DO_6_PIN							GPIO_Pin_13

/* The seventh digital-output channel define------------*/
#define DO_7_GPIO							GPIOE
#define DO_7_GPIO_CLK					RCC_APB2Periph_GPIOE
#define DO_7_PIN							GPIO_Pin_12

/* The eighth digital-output channel define------------*/
#define DO_8_GPIO							GPIOE
#define DO_8_GPIO_CLK					RCC_APB2Periph_GPIOE
#define DO_8_PIN							GPIO_Pin_11

/* The ninth digital-output channel define------------*/
#define DO_9_GPIO							GPIOE
#define DO_9_GPIO_CLK					RCC_APB2Periph_GPIOE
#define DO_9_PIN							GPIO_Pin_10

/* The tenth digital-output channel define------------*/
#define DO_10_GPIO						GPIOE
#define DO_10_GPIO_CLK				RCC_APB2Periph_GPIOE
#define DO_10_PIN							GPIO_Pin_9

/* The eleventh digital-output channel define------------*/
#define DO_11_GPIO						GPIOE
#define DO_11_GPIO_CLK				RCC_APB2Periph_GPIOE
#define DO_11_PIN							GPIO_Pin_8

/* The twelfth digital-output channel define------------*/
#define DO_12_GPIO						GPIOE
#define DO_12_GPIO_CLK				RCC_APB2Periph_GPIOE
#define DO_12_PIN							GPIO_Pin_7

/* The thirteenth digital-output channel define------------*/
#define DO_13_GPIO						GPIOF
#define DO_13_GPIO_CLK				RCC_APB2Periph_GPIOF
#define DO_13_PIN							GPIO_Pin_15

/* The fourteenth digital-output channel define------------*/
#define DO_14_GPIO						GPIOF
#define DO_14_GPIO_CLK				RCC_APB2Periph_GPIOF
#define DO_14_PIN							GPIO_Pin_14

/* The fifteenth digital-output channel define------------*/
#define DO_15_GPIO						GPIOF
#define DO_15_GPIO_CLK				RCC_APB2Periph_GPIOF
#define DO_15_PIN							GPIO_Pin_13

/* The sixteenth digital-output channel define------------*/
#define DO_16_GPIO						GPIOF
#define DO_16_GPIO_CLK				RCC_APB2Periph_GPIOF
#define DO_16_PIN							GPIO_Pin_12
/* Exported functions ------------------------------------------------------- */
extern void DO_SetBits(u8 DO_Channel_x);
extern void DO_ResetBits(u8 DO_Channel_x);
extern void Control_GPIO_Config(void);
extern void Sys_Run_LED(u8 satatus);
#endif /* __CONTROL_DO_H */

/******************* (C) COPYRIGHT YIYA TEAM *****END OF FILE****/
