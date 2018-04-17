/******************_** (C) COPYRIGHT 2014 YIYA ********************
* File Name          : control_adc.c
* Author             : YIYA Team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file provides all the user's ADC firmware functions.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "control_adc.h"
#include "stm32f10x_it.h" 
#include "control_global.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dac.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Global varibales -------------------------------------------------------------*/
u16 ADC_ValueAfterFilter[16];
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void BSP_AI_GPIO_Config(void);
/* Private functions ---------------------------------------------------------*/
void BSP_ADC_Config(void);
void BSP_AI_SelectChannel(u8 channelx);
void BSP_AI_Select_Mode(u8 mode);
void BSP_AI_Select_Gain(u8 channelx,u8 AI_SELECT_GAIN_x);
void BSP_MeasurResist_CurrentOut(u8 currentx);
void BSP_MeasurResist_CurrentOut_value(u16 value);
u16 Get_ADC1(u8 chx);
u16 Get_ADC3(u8 chx);
/*******************************************************************************
* Function Name  : BSP_ADC_Config(void)
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BSP_ADC_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOF | RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC3,ENABLE );
	
/*----initialize ADC1's channel-10,channel-11,channel-12 port respectively----*/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
/*----initialize ADC3's channel-4,channel-5,channel-6,channel-7,channel-8 port respectively----*/		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
/*----initialize ADC1's parameters----*/	
	ADC_DeInit(ADC1);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
	ADC_Init(ADC1, &ADC_InitStructure);	  
/*----initialize ADC1's regular groups----*/	
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_12,3,ADC_SampleTime_239Cycles5);
/*----enable ADC1 function----------*/	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);	
	while(ADC_GetResetCalibrationStatus(ADC1));	
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

/*----initialize ADC3's parameters----*/	
	ADC_DeInit(ADC3);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
	ADC_Init(ADC3, &ADC_InitStructure);	  
/*----initialize ADC3's regular groups----*/	
//	ADC_RegularChannelConfig(ADC3,ADC_Channel_4,1,ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC3,ADC_Channel_5,2,ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC3,ADC_Channel_6,3,ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC3,ADC_Channel_7,4,ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC3,ADC_Channel_8,5,ADC_SampleTime_239Cycles5);
/*----enable ADC1 function----------*/	
	ADC_Cmd(ADC3, ENABLE);
	
	ADC_ResetCalibration(ADC3);	
	while(ADC_GetResetCalibrationStatus(ADC3));	
	ADC_StartCalibration(ADC3);
	while(ADC_GetCalibrationStatus(ADC3));

	BSP_AI_GPIO_Config();
	
	ALL_DATA_RAM[ADG1611_ACTIVE_LEVEL] = 0; // default is active high
	
	BSP_AI_Select_Gain(1,1);
}

u16 Get_ADC1(u8 chx)
{
	ADC_RegularChannelConfig(ADC1, chx, 1, ADC_SampleTime_239Cycles5 );  
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	
	return ADC_GetConversionValue(ADC1);
}
u16 Get_ADC3(u8 chx)
{
	ADC_RegularChannelConfig(ADC3, chx, 1, ADC_SampleTime_239Cycles5 );  
  
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);
	 
	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
	
	return ADC_GetConversionValue(ADC3);
}

void BSP_AI_SelectChannel(u8 channelx)
{
	u8 delay;
	switch (channelx)
	{
		case 0:
			GPIO_ResetBits(GPIOG,GPIO_Pin_3);		
		    GPIO_ResetBits(GPIOG,GPIO_Pin_2);
		break;
				
		case 1:
		case 9:
			GPIO_ResetBits(GPIOG,GPIO_Pin_3);		
		    GPIO_ResetBits(GPIOG,GPIO_Pin_2);
		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);
				
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
			GPIO_ResetBits(GPIOF,GPIO_Pin_11);	
				
		    GPIO_SetBits(GPIOG,GPIO_Pin_3);
			GPIO_SetBits(GPIOG,GPIO_Pin_2);	
		break;
				
		case 2:
		case 10:
			GPIO_ResetBits(GPIOG,GPIO_Pin_3);		
		    GPIO_ResetBits(GPIOG,GPIO_Pin_2);
		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_SetBits(GPIOA,GPIO_Pin_7);
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);
				
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
			GPIO_ResetBits(GPIOF,GPIO_Pin_11);	
				
		    GPIO_SetBits(GPIOG,GPIO_Pin_3);
			GPIO_SetBits(GPIOG,GPIO_Pin_2);	
		break;
		
		case 3:
		case 11:
			GPIO_ResetBits(GPIOG,GPIO_Pin_3);		
		    GPIO_ResetBits(GPIOG,GPIO_Pin_2);
		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);
			GPIO_SetBits(GPIOC,GPIO_Pin_4);
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);
				
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
			GPIO_ResetBits(GPIOF,GPIO_Pin_11);	
				
		    GPIO_SetBits(GPIOG,GPIO_Pin_3);
			GPIO_SetBits(GPIOG,GPIO_Pin_2);	
		break;
		
		case 4:
		case 12:
			GPIO_ResetBits(GPIOG,GPIO_Pin_3);		
		    GPIO_ResetBits(GPIOG,GPIO_Pin_2);
		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_SetBits(GPIOA,GPIO_Pin_7);
			GPIO_SetBits(GPIOC,GPIO_Pin_4);
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);
				
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
			GPIO_ResetBits(GPIOF,GPIO_Pin_11);	
				
		    GPIO_SetBits(GPIOG,GPIO_Pin_3);
			GPIO_SetBits(GPIOG,GPIO_Pin_2);	
		break;
		
		case 5:
		case 13:
			GPIO_ResetBits(GPIOG,GPIO_Pin_3);		
		    GPIO_ResetBits(GPIOG,GPIO_Pin_2);
		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
			GPIO_SetBits(GPIOC,GPIO_Pin_5);
				
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
			GPIO_SetBits(GPIOF,GPIO_Pin_11);	
				
		    GPIO_SetBits(GPIOG,GPIO_Pin_3);
			GPIO_SetBits(GPIOG,GPIO_Pin_2);
		break;
		
		case 6:
		case 14:
			GPIO_ResetBits(GPIOG,GPIO_Pin_3);		
		    GPIO_ResetBits(GPIOG,GPIO_Pin_2);
		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_SetBits(GPIOA,GPIO_Pin_7);
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
			GPIO_SetBits(GPIOC,GPIO_Pin_5);
				
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
			GPIO_SetBits(GPIOF,GPIO_Pin_11);	
				
		  GPIO_SetBits(GPIOG,GPIO_Pin_3);
			GPIO_SetBits(GPIOG,GPIO_Pin_2);
		break;
		
		case 7:
		case 15:
			GPIO_ResetBits(GPIOG,GPIO_Pin_3);		
		    GPIO_ResetBits(GPIOG,GPIO_Pin_2);
		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);
			GPIO_SetBits(GPIOC,GPIO_Pin_4);
			GPIO_SetBits(GPIOC,GPIO_Pin_5);
				
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
			GPIO_SetBits(GPIOF,GPIO_Pin_11);	
				
		    GPIO_SetBits(GPIOG,GPIO_Pin_3);
			GPIO_SetBits(GPIOG,GPIO_Pin_2);
		break;
		
		case 8:
		case 16:
			GPIO_ResetBits(GPIOG,GPIO_Pin_3);		
		    GPIO_ResetBits(GPIOG,GPIO_Pin_2);
		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_SetBits(GPIOA,GPIO_Pin_7);
			GPIO_SetBits(GPIOC,GPIO_Pin_4);
			GPIO_SetBits(GPIOC,GPIO_Pin_5);
				
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
			GPIO_SetBits(GPIOF,GPIO_Pin_11);	
				
		    GPIO_SetBits(GPIOG,GPIO_Pin_3);
			GPIO_SetBits(GPIOG,GPIO_Pin_2);
		break;	
		default:
			GPIO_ResetBits(GPIOG,GPIO_Pin_3);		
		    GPIO_ResetBits(GPIOG,GPIO_Pin_2);
		break;
	}
}

void BSP_AI_Select_Mode(u8 mode)
{
	u8 delay;
	
	switch (mode)
	{
		case 0:
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);		
		break;
		
		case 1:
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_ResetBits(GPIOG,GPIO_Pin_7);
			GPIO_ResetBits(GPIOG,GPIO_Pin_6);
			GPIO_ResetBits(GPIOG,GPIO_Pin_5);
			GPIO_SetBits(GPIOG,GPIO_Pin_4);	
		break;
				
		case 2:
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_SetBits(GPIOG,GPIO_Pin_7);
			GPIO_ResetBits(GPIOG,GPIO_Pin_6);
			GPIO_ResetBits(GPIOG,GPIO_Pin_5);
			GPIO_SetBits(GPIOG,GPIO_Pin_4);	
		break;

		case 3:
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_ResetBits(GPIOG,GPIO_Pin_7);
			GPIO_SetBits(GPIOG,GPIO_Pin_6);
			GPIO_ResetBits(GPIOG,GPIO_Pin_5);
			GPIO_SetBits(GPIOG,GPIO_Pin_4);	
		break;
		
		case 4:
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_SetBits(GPIOG,GPIO_Pin_7);
			GPIO_SetBits(GPIOG,GPIO_Pin_6);
			GPIO_ResetBits(GPIOG,GPIO_Pin_5);
			GPIO_SetBits(GPIOG,GPIO_Pin_4);	
		break;
		
		case 5:
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_ResetBits(GPIOG,GPIO_Pin_7);
			GPIO_ResetBits(GPIOG,GPIO_Pin_6);
			GPIO_SetBits(GPIOG,GPIO_Pin_5);
			GPIO_SetBits(GPIOG,GPIO_Pin_4);	
		break;
		
		case 6:
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_SetBits(GPIOG,GPIO_Pin_7);
			GPIO_ResetBits(GPIOG,GPIO_Pin_6);
			GPIO_SetBits(GPIOG,GPIO_Pin_5);
			GPIO_SetBits(GPIOG,GPIO_Pin_4);	
		break;
		
		case 7:
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_ResetBits(GPIOG,GPIO_Pin_7);
			GPIO_SetBits(GPIOG,GPIO_Pin_6);
			GPIO_SetBits(GPIOG,GPIO_Pin_5);
			GPIO_SetBits(GPIOG,GPIO_Pin_4);	
		break;
		
		case 8:
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);		
			for(delay=0; delay<20; delay++)
			{;}
			GPIO_SetBits(GPIOG,GPIO_Pin_7);
			GPIO_SetBits(GPIOG,GPIO_Pin_6);
			GPIO_SetBits(GPIOG,GPIO_Pin_5);
			GPIO_SetBits(GPIOG,GPIO_Pin_4);	
		break;	
		default:
			GPIO_ResetBits(GPIOG,GPIO_Pin_4);		
		break;
	}
}

//lc 10142017 the actual active level of the switch ADG1611 is 1,
// which is opposite to datasheet!
// input£º1 switch on; input: 0 switch off
//lc 11112017 new product is acitive low and is coherent with spec. add a register to set
void BSP_AI_Select_Gain(u8 channelx,u8 AI_SELECT_GAIN_x)
{
if (!ALL_DATA_RAM[ADG1611_ACTIVE_LEVEL]){
	if(0 < channelx <= 8)
	{
		switch (AI_SELECT_GAIN_x)
		{
			case AI_SELECT_GAIN_1:
				GPIO_SetBits(GPIOC,GPIO_Pin_3);		
				GPIO_SetBits(GPIOA,GPIO_Pin_0);
				GPIO_SetBits(GPIOA,GPIO_Pin_1);
				GPIO_SetBits(GPIOA,GPIO_Pin_2);	
			break;
					
			case AI_SELECT_GAIN_2:
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);		
				GPIO_SetBits(GPIOA,GPIO_Pin_0);
				GPIO_SetBits(GPIOA,GPIO_Pin_1);
				GPIO_SetBits(GPIOA,GPIO_Pin_2);	
			break;

			case 3:
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);		
				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
				GPIO_SetBits(GPIOA,GPIO_Pin_1);
				GPIO_SetBits(GPIOA,GPIO_Pin_2);	
			break;
			
			case 4:
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);		
				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				GPIO_SetBits(GPIOA,GPIO_Pin_2);	
			break;
			
			case 5:
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);		
				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
			break;

			default:
				GPIO_SetBits(GPIOC,GPIO_Pin_3);		
				GPIO_SetBits(GPIOA,GPIO_Pin_0);
				GPIO_SetBits(GPIOA,GPIO_Pin_1);
				GPIO_SetBits(GPIOA,GPIO_Pin_2);	
			break;
		}
	}
	
	if(8 < channelx <=16)
	{
		switch (AI_SELECT_GAIN_x)
		{
			case AI_SELECT_GAIN_1:
				GPIO_SetBits(GPIOA,GPIO_Pin_3);		
				GPIO_SetBits(GPIOA,GPIO_Pin_6);
				GPIO_SetBits(GPIOG,GPIO_Pin_0);
				GPIO_SetBits(GPIOG,GPIO_Pin_1);	
			break;
					
			case AI_SELECT_GAIN_2:
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);		
				GPIO_SetBits(GPIOA,GPIO_Pin_6);
				GPIO_SetBits(GPIOG,GPIO_Pin_0);
				GPIO_SetBits(GPIOG,GPIO_Pin_1);	
			break;

			case AI_SELECT_GAIN_3:
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);		
				GPIO_ResetBits(GPIOA,GPIO_Pin_6);
				GPIO_SetBits(GPIOG,GPIO_Pin_0);
				GPIO_SetBits(GPIOG,GPIO_Pin_1);	
			break;
			
			case AI_SELECT_GAIN_4:
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);		
				GPIO_ResetBits(GPIOA,GPIO_Pin_6);
				GPIO_ResetBits(GPIOG,GPIO_Pin_0);
				GPIO_SetBits(GPIOG,GPIO_Pin_1);	
			break;
			
			case AI_SELECT_GAIN_5:
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);		
				GPIO_ResetBits(GPIOA,GPIO_Pin_6);
				GPIO_ResetBits(GPIOG,GPIO_Pin_0);
				GPIO_ResetBits(GPIOG,GPIO_Pin_1);	
			break;

			default:
				GPIO_SetBits(GPIOA,GPIO_Pin_3);		
				GPIO_SetBits(GPIOA,GPIO_Pin_6);
				GPIO_SetBits(GPIOG,GPIO_Pin_0);
				GPIO_SetBits(GPIOG,GPIO_Pin_1);	
			break;
		}
	}
}else{
	if(0 < channelx <= 8)
	{
		switch (AI_SELECT_GAIN_x)
		{
			case AI_SELECT_GAIN_1:
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);		
				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
			break;
					
			case AI_SELECT_GAIN_2:
				GPIO_SetBits(GPIOC,GPIO_Pin_3);		
				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
			break;

			case 3:
				GPIO_SetBits(GPIOC,GPIO_Pin_3);		
				GPIO_SetBits(GPIOA,GPIO_Pin_0);
				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
			break;
			
			case 4:
				GPIO_SetBits(GPIOC,GPIO_Pin_3);		
				GPIO_SetBits(GPIOA,GPIO_Pin_0);
				GPIO_SetBits(GPIOA,GPIO_Pin_1);
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
			break;
			
			case 5:
				GPIO_SetBits(GPIOC,GPIO_Pin_3);		
				GPIO_SetBits(GPIOA,GPIO_Pin_0);
				GPIO_SetBits(GPIOA,GPIO_Pin_1);
				GPIO_SetBits(GPIOA,GPIO_Pin_2);	
			break;

			default:
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);		
				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
			break;
		}
	}
	
	if(8 < channelx <=16)
	{
		switch (AI_SELECT_GAIN_x)
		{
			case AI_SELECT_GAIN_1:
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);		
				GPIO_ResetBits(GPIOA,GPIO_Pin_6);
				GPIO_ResetBits(GPIOG,GPIO_Pin_0);
				GPIO_ResetBits(GPIOG,GPIO_Pin_1);	
			break;
					
			case AI_SELECT_GAIN_2:
				GPIO_SetBits(GPIOA,GPIO_Pin_3);		
				GPIO_ResetBits(GPIOA,GPIO_Pin_6);
				GPIO_ResetBits(GPIOG,GPIO_Pin_0);
				GPIO_ResetBits(GPIOG,GPIO_Pin_1);	
			break;

			case AI_SELECT_GAIN_3:
				GPIO_SetBits(GPIOA,GPIO_Pin_3);		
				GPIO_SetBits(GPIOA,GPIO_Pin_6);
				GPIO_ResetBits(GPIOG,GPIO_Pin_0);
				GPIO_ResetBits(GPIOG,GPIO_Pin_1);	
			break;
			
			case AI_SELECT_GAIN_4:
				GPIO_SetBits(GPIOA,GPIO_Pin_3);		
				GPIO_SetBits(GPIOA,GPIO_Pin_6);
				GPIO_SetBits(GPIOG,GPIO_Pin_0);
				GPIO_ResetBits(GPIOG,GPIO_Pin_1);	
			break;
			
			case AI_SELECT_GAIN_5:
				GPIO_SetBits(GPIOA,GPIO_Pin_3);		
				GPIO_SetBits(GPIOA,GPIO_Pin_6);
				GPIO_SetBits(GPIOG,GPIO_Pin_0);
				GPIO_SetBits(GPIOG,GPIO_Pin_1);	
			break;

			default:
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);		
				GPIO_ResetBits(GPIOA,GPIO_Pin_6);
				GPIO_ResetBits(GPIOG,GPIO_Pin_0);
				GPIO_ResetBits(GPIOG,GPIO_Pin_1);	
			break;
		}
	}
	}
}

void BSP_MeasurResist_CurrentOut(u8 currentx)
{
	switch(currentx)
	{
		case 1:
			DAC_SetChannel1Data(DAC_Align_12b_R, 1000);  
			DAC_SetChannel2Data(DAC_Align_12b_R, 1000);
		break;
		case 2:
			DAC_SetChannel1Data(DAC_Align_12b_R, 1000);  
			DAC_SetChannel2Data(DAC_Align_12b_R, 1000);
		break;
	}
}

void BSP_MeasurResist_CurrentOut_value(u16 value)
{
	DAC_SetChannel1Data(DAC_Align_12b_R, value);  
}

static void BSP_AI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
						   RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOF ,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
								  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
								  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}
