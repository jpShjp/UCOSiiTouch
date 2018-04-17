#ifndef _BSP_H_
#define _BSP_H_
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_map.h"

#ifdef  BSP_GLOBALS
#define BSP_EXT
#else
#define BSP_EXT  extern
#endif
/*
//DO port define//
#define SYS_RUN_LED	      GPIO_Pin_6 //GPIOE
#define DO_LED_1          GPIO_Pin_15	//GPIOB
#define	DO_LED_2		  GPIO_Pin_14	//GPIOB
#define	DO_LED_3		  GPIO_Pin_13	//GPIOB
#define DO_LED_4		  GPIO_Pin_12	//GPIOB
#define	DO_LED_5		  GPIO_Pin_14	//GPIOE
#define	DO_LED_6	      GPIO_Pin_13	//GPIOE
#define DO_LED_7		  GPIO_Pin_12	//GPIOE
#define	DO_LED_8		  GPIO_Pin_11	//GPIOE
#define	DO_LED_9		  GPIO_Pin_10	//GPIOE
#define DO_LED_10		  GPIO_Pin_9	//GPIOE
#define	DO_LED_11		  GPIO_Pin_8	//GPIOE
#define	DO_LED_12		  GPIO_Pin_7	//GPIOE
#define	DO_LED_13		  GPIO_Pin_15	//GPIOF
#define DO_LED_14		  GPIO_Pin_14	//GPIOF
#define	DO_LED_15		  GPIO_Pin_13	//GPIOF
#define	DO_LED_16		  GPIO_Pin_12	//GPIOF


#define COMM3_GPIO		  GPIOB
#define COMM3_CLK		  RCC_APB1Periph_USART3
#define COMM3_GPIO_CLK	  RCC_APB2Periph_GPIOB
#define	COMM3_RxPin		  GPIO_Pin_11
#define	COMM3_TxPin		  GPIO_Pin_10
#define COMM3_Ctrl_GPIO	  GPIOE
#define COMM3_Ctrl_CLK    RCC_APB2Periph_GPIOE
#define COMM3_CtrlPin	  GPIO_Pin_15
#define COMM3_Stat_GPIO	  GPIOF
#define COMM3_Stat_CLK    RCC_APB2Periph_GPIOF
#define COMM3_RxLEDPin	  GPIO_Pin_5
#define	COMM3_TxLEDPin	  GPIO_Pin_4

#define COMM2_GPIO		  GPIOD
#define COMM2_CLK		  RCC_APB1Periph_USART2
#define COMM2_GPIO_CLK	  RCC_APB2Periph_GPIOD
#define	COMM2_RxPin		  GPIO_Pin_6
#define	COMM2_TxPin		  GPIO_Pin_5
#define COMM2_Ctrl_GPIO	  GPIOD
#define COMM2_Ctrl_CLK    RCC_APB2Periph_GPIOD
#define COMM2_CtrlPin	  GPIO_Pin_4
#define COMM2_Stat_GPIO	  GPIOF
#define COMM2_Stat_CLK    RCC_APB2Periph_GPIOF
#define COMM2_RxLEDPin	  GPIO_Pin_3
#define	COMM2_TxLEDPin	  GPIO_Pin_2

#define COMM1_GPIO		  GPIOA
#define COMM1_CLK		  RCC_APB2Periph_USART1
#define COMM1_GPIO_CLK	  RCC_APB2Periph_GPIOA
#define	COMM1_RxPin		  GPIO_Pin_10
#define	COMM1_TxPin		  GPIO_Pin_9
#define COMM1_Ctrl_GPIO	  GPIOA
#define COMM1_Ctrl_CLK    RCC_APB2Periph_GPIOA
#define COMM1_CtrlPin	  GPIO_Pin_11
#define COMM1_Stat_GPIO	  GPIOF
#define COMM1_Stat_CLK    RCC_APB2Periph_GPIOF
#define COMM1_RxLEDPin	  GPIO_Pin_1
#define	COMM1_TxLEDPin	  GPIO_Pin_0


extern void COMM3_RX_Mode(void);
extern void COMM3_TX_Mode(void);
extern void COMM3_IDLE(void);
extern void COMM2_RX_Mode(void);
extern void COMM2_TX_Mode(void);
extern void COMM2_IDLE(void);
extern void COMM1_RX_Mode(void);
extern void COMM1_TX_Mode(void);
extern void COMM1_IDLE(void);
extern void DMA_Enabale(DMA_Channel_TypeDef *DMA_CHx);
extern u16 Get_ADC(u8 ch);
*/
/************************************************************************************/
extern void BSP_Init(void);

#endif


