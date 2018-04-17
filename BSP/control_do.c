/******************** (C) COPYRIGHT 2014 YIYA ********************
* File Name          : control_do.c
* Author             : YIYA Team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file provides all the user's DO firmware functions.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "control_do.h"
#include "stm32f10x_it.h" 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Control_GPIO_Config(void);
void Sys_Run_LED(u8 satatus);
/*******************************************************************************
* Function Name  : DO_SetBits(u8 DO_Channel_x)
* Description    : Set DO Channel x
* Input          : - DO_Channel_x: where x can be (1..16) to select the DO channel.
* Output         : None
* Return         : None
*******************************************************************************/
void DO_SetBits(u8 DO_Channel_x)
{
	/* Check the parameters */
	
	switch(DO_Channel_x)
	{
		case DO_Channel_1:
			GPIO_SetBits(DO_1_GPIO, DO_1_PIN);
			break;
		case DO_Channel_2:
			GPIO_SetBits(DO_2_GPIO, DO_2_PIN);
			break;
		case DO_Channel_3:
			GPIO_SetBits(DO_3_GPIO, DO_3_PIN);
			break;
		case DO_Channel_4:
			GPIO_SetBits(DO_4_GPIO, DO_4_PIN);
			break;
		case DO_Channel_5:
			GPIO_SetBits(DO_5_GPIO, DO_5_PIN);
			break;
		case DO_Channel_6:
			GPIO_SetBits(DO_6_GPIO, DO_6_PIN);
			break;
		case DO_Channel_7:
			GPIO_SetBits(DO_7_GPIO, DO_7_PIN);
			break;
		case DO_Channel_8:
			GPIO_SetBits(DO_8_GPIO, DO_8_PIN);
			break;
		case DO_Channel_9:
			GPIO_SetBits(DO_9_GPIO, DO_9_PIN);
			break;
		case DO_Channel_10:
			GPIO_SetBits(DO_10_GPIO, DO_10_PIN);
			break;
		case DO_Channel_11:
			GPIO_SetBits(DO_11_GPIO, DO_11_PIN);
			break;
		case DO_Channel_12:
			GPIO_SetBits(DO_12_GPIO, DO_12_PIN);
		break;
		case DO_Channel_13:
			GPIO_SetBits(DO_13_GPIO, DO_13_PIN);
			break;
		case DO_Channel_14:
			GPIO_SetBits(DO_14_GPIO, DO_14_PIN);
			break;
		case DO_Channel_15:
			GPIO_SetBits(DO_15_GPIO, DO_15_PIN);
			break;
		case DO_Channel_16:
			GPIO_SetBits(DO_16_GPIO, DO_16_PIN);
		break;
		
		default:
			break;	
	}
}

void Sys_Run_LED(u8 status)
{
	if(status == 1)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_6);
	}
	if(status == 0)
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_6);
	}
}
/*******************************************************************************
* Function Name  : DO_ResetBits(u8 DO_Channel_x)
* Description    : Reset DO Channel x
* Input          : - DO_Channel_x: where x can be (1..16) to select the DO channel.
* Output         : None
* Return         : None
*******************************************************************************/
void DO_ResetBits(u8 DO_Channel_x)
{
	/* Check the parameters */
	
	switch(DO_Channel_x)
	{
		case DO_Channel_1:
			GPIO_ResetBits(DO_1_GPIO, DO_1_PIN);
			break;
		case DO_Channel_2:
			GPIO_ResetBits(DO_2_GPIO, DO_2_PIN);
			break;
		case DO_Channel_3:
			GPIO_ResetBits(DO_3_GPIO, DO_3_PIN);
			break;
		case DO_Channel_4:
			GPIO_ResetBits(DO_4_GPIO, DO_4_PIN);
			break;
		case DO_Channel_5:
			GPIO_ResetBits(DO_5_GPIO, DO_5_PIN);
			break;
		case DO_Channel_6:
			GPIO_ResetBits(DO_6_GPIO, DO_6_PIN);
			break;
		case DO_Channel_7:
			GPIO_ResetBits(DO_7_GPIO, DO_7_PIN);
			break;
		case DO_Channel_8:
			GPIO_ResetBits(DO_8_GPIO, DO_8_PIN);
			break;
		case DO_Channel_9:
			GPIO_ResetBits(DO_9_GPIO, DO_9_PIN);
			break;
		case DO_Channel_10:
			GPIO_ResetBits(DO_10_GPIO, DO_10_PIN);
			break;
		case DO_Channel_11:
			GPIO_ResetBits(DO_11_GPIO, DO_11_PIN);
			break;
		case DO_Channel_12:
			GPIO_ResetBits(DO_12_GPIO, DO_12_PIN);
		break;
		case DO_Channel_13:
			GPIO_ResetBits(DO_13_GPIO, DO_13_PIN);
			break;
		case DO_Channel_14:
			GPIO_ResetBits(DO_14_GPIO, DO_14_PIN);
			break;
		case DO_Channel_15:
			GPIO_ResetBits(DO_15_GPIO, DO_15_PIN);
			break;
		case DO_Channel_16:
			GPIO_ResetBits(DO_16_GPIO, DO_16_PIN);
		break;
		
		default:
			break;	
	}
}

void Control_GPIO_Config(void)
{
	u8 i = 1;
	GPIO_TypeDef *select_gpio_tempreg;
	u32 select_gpio_clk_tempreg;
	u16 select_pin_tempreg;
	
	GPIO_InitTypeDef  GPIO_InitStruct; //claim general GPIO structure
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); //release the ports of JTAG as GPIO
	
	for(i=1; i<=DO_Channel_16; i++)
	{
		switch(i)
		{
		case DO_Channel_1:
			select_gpio_tempreg = DO_1_GPIO;
			select_gpio_clk_tempreg = DO_1_GPIO_CLK;
			select_pin_tempreg = DO_1_PIN;		
			break;
		case DO_Channel_2:
			select_gpio_tempreg = DO_2_GPIO;
			select_gpio_clk_tempreg = DO_2_GPIO_CLK;
			select_pin_tempreg = DO_2_PIN;
			break;
		case DO_Channel_3:
			select_gpio_tempreg = DO_3_GPIO;
			select_gpio_clk_tempreg = DO_3_GPIO_CLK;
			select_pin_tempreg = DO_3_PIN;
			break;
		case DO_Channel_4:
			select_gpio_tempreg = DO_4_GPIO;
			select_gpio_clk_tempreg = DO_4_GPIO_CLK;
			select_pin_tempreg = DO_4_PIN;
			break;
		case DO_Channel_5:
			select_gpio_tempreg = DO_5_GPIO;
			select_gpio_clk_tempreg = DO_5_GPIO_CLK;
			select_pin_tempreg = DO_5_PIN;
			break;
		case DO_Channel_6:
			select_gpio_tempreg = DO_6_GPIO;
			select_gpio_clk_tempreg = DO_6_GPIO_CLK;
			select_pin_tempreg = DO_6_PIN;
			break;
		case DO_Channel_7:
			select_gpio_tempreg = DO_7_GPIO;
			select_gpio_clk_tempreg = DO_7_GPIO_CLK;
			select_pin_tempreg = DO_7_PIN;
			break;
		case DO_Channel_8:
			select_gpio_tempreg = DO_8_GPIO;
			select_gpio_clk_tempreg = DO_8_GPIO_CLK;
			select_pin_tempreg = DO_8_PIN;
			break;
		case DO_Channel_9:
			select_gpio_tempreg = DO_9_GPIO;
			select_gpio_clk_tempreg = DO_9_GPIO_CLK;
			select_pin_tempreg = DO_9_PIN;
			break;
		case DO_Channel_10:
			select_gpio_tempreg = DO_10_GPIO;
			select_gpio_clk_tempreg = DO_10_GPIO_CLK;
			select_pin_tempreg = DO_10_PIN;
			break;
		case DO_Channel_11:
			select_gpio_tempreg = DO_11_GPIO;
			select_gpio_clk_tempreg = DO_11_GPIO_CLK;
			select_pin_tempreg = DO_11_PIN;
			break;
		case DO_Channel_12:
			select_gpio_tempreg = DO_12_GPIO;
			select_gpio_clk_tempreg = DO_12_GPIO_CLK;
			select_pin_tempreg = DO_12_PIN;
		break;
		case DO_Channel_13:
			select_gpio_tempreg = DO_13_GPIO;
			select_gpio_clk_tempreg = DO_13_GPIO_CLK;
			select_pin_tempreg = DO_13_PIN;
			break;
		case DO_Channel_14:
			select_gpio_tempreg = DO_14_GPIO;
			select_gpio_clk_tempreg = DO_14_GPIO_CLK;
			select_pin_tempreg = DO_14_PIN;
			break;
		case DO_Channel_15:
			select_gpio_tempreg = DO_15_GPIO;
			select_gpio_clk_tempreg = DO_15_GPIO_CLK;
			select_pin_tempreg = DO_15_PIN;
			break;
		case DO_Channel_16:
			select_gpio_tempreg = DO_16_GPIO;
			select_gpio_clk_tempreg = DO_16_GPIO_CLK;
			select_pin_tempreg = DO_16_PIN;
			break;
		
		default:
			break;	
		}
		RCC_APB2PeriphClockCmd(select_gpio_clk_tempreg, ENABLE); //enable gpio clk
		GPIO_InitStruct.GPIO_Pin = select_pin_tempreg;	//select the pin
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  //select the mode
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  //select the speed para
    GPIO_Init(select_gpio_tempreg,&GPIO_InitStruct);
	}
}
