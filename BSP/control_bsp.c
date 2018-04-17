/**************-****** (C) COPYRIGHT 2014 YIYA ********************
* File Name          : control_bsp.c
* Author             : YIYA Team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file provides all the bsp firmware functions.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "control_bsp.h"
#include "stm32f10x_it.h" 
#include "ucos_ii.h"
#include "control_do.h"
#include "control_di.h"
#include "control_comm.h"
#include "control_adc.h"
#include "control_dac.h"
#include "stm32f10x_flash.h"
#include "delay.h"
#include "misc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
	/* The system run led define------------*/
#define SYSRUN_LED_GPIO				GPIOE
#define SYSRUN_LED_GPIO_CLK		RCC_APB2Periph_GPIOE
#define SYSRUN_LED_PIN				GPIO_Pin_6

	/* The system function button define------------*/
#define SYSFUN_BUTTON_GPIO				GPIOD
#define SYSFUN_BUTTON_GPIO_CLK		RCC_APB2Periph_GPIOD
#define SYSFUN_BUTTON_PIN					GPIO_Pin_0

	/* The system WDI define------------*/
#define SYSFUN_WDI_GPIO				GPIOG
#define SYSFUN_WDI_GPIO_CLK		RCC_APB2Periph_GPIOG
#define SYSFUN_WDI_PIN				GPIO_Pin_8


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void BSP_SysClkInit(void);
static void BSP_SysGenFunInit(void);
static void BSP_SysTick_Config(void);
static void BSP_Interrupt_Config(void);
/* Private functions ---------------------------------------------------------*/
void BSP_Config(void);

/*******************************************************************************
* Function Name  : BSP_Config(void)
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BSP_Config(void)
{
	BSP_SysClkInit();
	
  BSP_Comm_Config();
	BSP_SysGenFunInit();
	Control_GPIO_Config();
	Control_GPI_Config();
	BSP_ADC_Config();
	BSP_DAC_Config();
}

/*******************************************************************************
* Function Name  : BSP_SysclkInit(void)
* Description    : Set system clock
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void BSP_SysClkInit(void)
{
	ErrorStatus HSEStartUpStatus = SUCCESS;
	
	RCC_DeInit();
	
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		FLASH_SetLatency(FLASH_Latency_2);
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);

		RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
		{ ; }

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	
		while(RCC_GetSYSCLKSource() != 0x08)
		{ ; }		
	 }
	 
	 BSP_Interrupt_Config();  //config the system interrupt
	 BSP_SysTick_Config();    //config the system tick time
}

static void BSP_SysGenFunInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct; //claim general GPIO structure
	
	/* The system run led config,output------------*/
	RCC_APB2PeriphClockCmd(SYSRUN_LED_GPIO_CLK, ENABLE); //enable gpio clk
	GPIO_InitStruct.GPIO_Pin = SYSRUN_LED_PIN;	//select the pin
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  //select the mode
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  //select the speed para
	GPIO_Init(SYSRUN_LED_GPIO,&GPIO_InitStruct);
	
	/* The system function button config,input------------*/
	RCC_APB2PeriphClockCmd(SYSFUN_BUTTON_GPIO_CLK, ENABLE);//enable gpio clk
	GPIO_InitStruct.GPIO_Pin = SYSFUN_BUTTON_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(SYSFUN_BUTTON_GPIO,&GPIO_InitStruct);
	
	/* The system function WDI config,output------------*/
	RCC_APB2PeriphClockCmd(SYSFUN_WDI_GPIO_CLK, ENABLE);
	GPIO_InitStruct.GPIO_Pin = SYSFUN_WDI_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SYSFUN_WDI_GPIO,&GPIO_InitStruct);
}

static void BSP_Interrupt_Config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* set system tick interrupt priority as the first
	(preemption is 0; subpriority is 0)--------------------*/
 //   NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick,0,0);
}

static void BSP_SysTick_Config(void)
{
//	RCC_ClocksTypeDef  rcc_clocks;
//	u32  cnts;

//	RCC_GetClocksFreq(&rcc_clocks);
//	
//	cnts = (u32)rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC;

//	SysTick_SetReload(cnts);
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
//	SysTick_CounterCmd(SysTick_Counter_Enable);
//	SysTick_ITConfig(ENABLE);
}
