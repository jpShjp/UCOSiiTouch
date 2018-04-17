/******************_** (C) COPYRIGHT 2014 YIYA ********************
* File Name          : control_dac.c
* Author             : YIYA Team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file provides all the user's DAC firmware functions.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "control_global.h"
#include "control_dac.h"
#include "stm32f10x_it.h" 
#include "stm32f10x_dac.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Global varibales -------------------------------------------------------------*/
u16 DAC_Value[4];
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void BSP_PWM_Init(u16 arr,u16 psc);
static void BSP_AO_GPIO_Config(void);
/* Private functions ---------------------------------------------------------*/
void BSP_DAC_Config(void);
void BSP_PWM_DutyCycle(u8 channelx,s16 dutycycle);
void BSP_AO_Select_Mode(u8 channelx,u8 mode);

/*******************************************************************************
* Function Name  : BSP_DAC_Config(void)
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BSP_DAC_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //使能PORTA通道时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //使能DAC通道时钟 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;				 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //模拟输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4)	;//PA.4 输出高
	GPIO_SetBits(GPIOA,GPIO_Pin_5)	;//PA.5 输出高
					
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1
	DAC_Init(DAC_Channel_2,&DAC_InitType);	 //初始化DAC通道2

	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC1
	DAC_Cmd(DAC_Channel_2, ENABLE);  //使能DAC2
  	
	// lc 111117
	ALL_DATA_RAM[DAC_OUTPUT] = 3735; // about 2.5mA (if r=500, the voltage is 1250mV)
	// AI ch1-4默认电压模式，any value but 3
	ALL_DATA_RAM[AI_MODE_CH1] = 1;
	ALL_DATA_RAM[AI_MODE_CH2] = 1;
	ALL_DATA_RAM[AI_MODE_CH3] = 1;
	ALL_DATA_RAM[AI_MODE_CH4] = 1;
	
	DAC_SetChannel1Data(DAC_Align_12b_R, ALL_DATA_RAM[DAC_OUTPUT]);  //12位右对齐数据格式设置DAC值
	DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
	
	BSP_PWM_Init(PWM_8K,0);
	BSP_AO_GPIO_Config();
}

void BSP_PWM_DutyCycle(u8 channelx,s16 dutycycle)
{
	switch(channelx)
	{
		case 1:
			TIM4->CCR4 = dutycycle;
		break;
		
		case 2:
			TIM4->CCR3 = dutycycle;
		break;
		
		case 3:
			TIM4->CCR2 = dutycycle;
		break;
		
		case 4:
			TIM4->CCR1 = dutycycle;
		break;
	}
}

void BSP_AO_Select_Mode(u8 channelx,u8 mode)
{
	if (!ALL_DATA_RAM[ADG1611_ACTIVE_LEVEL])
	{
		if (mode == AO_SELECT_MODE_1)
			mode = AO_SELECT_MODE_2;
		else
			mode = AO_SELECT_MODE_1;
	}

	switch(channelx)
	{
		case 1:
			if(mode == AO_SELECT_MODE_1)
			{
				GPIO_ResetBits(GPIOD,GPIO_Pin_11);
			}
			if(mode == AO_SELECT_MODE_2)
			{
				GPIO_SetBits(GPIOD,GPIO_Pin_11);
			}
		break;
		case 2:
			if(mode == AO_SELECT_MODE_1)
			{
				GPIO_ResetBits(GPIOD,GPIO_Pin_10);
			}
			if(mode == AO_SELECT_MODE_2)
			{
				GPIO_SetBits(GPIOD,GPIO_Pin_10);
			}
		break;
		case 3:
			if(mode == AO_SELECT_MODE_1)
			{
				GPIO_ResetBits(GPIOD,GPIO_Pin_9);
			}
			if(mode == AO_SELECT_MODE_2)
			{
				GPIO_SetBits(GPIOD,GPIO_Pin_9);
			}
		break;
		case 4:
			if(mode == AO_SELECT_MODE_1)
			{
				GPIO_ResetBits(GPIOD,GPIO_Pin_8);
			}
			if(mode == AO_SELECT_MODE_2)
			{
				GPIO_SetBits(GPIOD,GPIO_Pin_8);
			}
		break;	
	}
}

static void BSP_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
/*-------enable the TIM4's clock , GPIO clock and AF's clock--------------*/	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD  | RCC_APB2Periph_AFIO, ENABLE);
	
/*-------remap the TIM4's port -------------------------------------------*/		
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //Timer4
	
/*-------initialize TIM4's port -------------------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
/*-------initialize PWM's parameters -------------------------------------------*/
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); 
 
	TIM_Cmd(TIM4, ENABLE);
}

static void BSP_AO_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD ,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
