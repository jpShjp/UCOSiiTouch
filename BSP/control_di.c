/******************** (C) COPYRIGHT 2014 YIYA ********************
* File Name          : control_do.c
* Author             : YIYA Team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file provides all the user's DO firmware functions.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "control_di.h"
#include "stm32f10x_it.h" 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Control_GPI_Config(void);
u16 DI_ReadBits(u8 DI_Channel_x);
/*******************************************************************************
* Function Name  : DI_ReadBits(u8 DI_Channel_x)
* Description    : Read DO Channel x
* Input          : - DI_Channel_x: where x can be (1..16) to select the DI channel.
* Output         : None
* Return         : None
*******************************************************************************/
u16 DI_ReadBits(u8 DI_Channel_x)
{
	/* Check the parameters */
	u8 di_value;
	
	switch(DI_Channel_x)
	{
		case DI_Channel_1:
			di_value = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9);
			break;
		case DI_Channel_2:
			di_value = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_10);
			break;
		case DI_Channel_3:
			di_value = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11);
			break;
		case DI_Channel_4:
			di_value = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_12);
			break;
		case DI_Channel_5:
			di_value = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13);
			break;
		case DI_Channel_6:
			di_value = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14);
			break;
		case DI_Channel_7:
			di_value = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_15);
			break;
		case DI_Channel_8:
			di_value = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
			break;
		case DI_Channel_9:
			di_value = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
			break;
		case DI_Channel_10:
			di_value = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
			break;
		case DI_Channel_11:
			di_value = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
			break;
		case DI_Channel_12:
			di_value = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1);
		  break;
		case DI_Channel_13:
			di_value = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2);
			break;
		case DI_Channel_14:
			di_value = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3);
			break;
		case DI_Channel_15:
			di_value = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4);
			break;
		case DI_Channel_16:
			di_value = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5);
		break;
		default:
			di_value = 0U;
			break;	
	}
	return(di_value);
}


void Control_GPI_Config(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStruct; //claim general GPIO structure
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOB | 
												 RCC_APB2Periph_GPIOE, ENABLE); //enable gpio clk
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | 
													   GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOG,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
													   GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
}
