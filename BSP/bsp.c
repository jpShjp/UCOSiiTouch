#define BSP_GLOBALS
#include "includes.h"


static volatile ErrorStatus HSEStartUpStatus = SUCCESS;

extern u8 SendBuff[200];

static void SysTick_Config(void);
void GPIO_Config(void);
void NVIC_Config(void);
void Interrupt_Config(void);
void COMM_Config(void);
void DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
void TIM4_PWM_Init(u16 arr,u16 psc);
void ADC_Config(void);
u16 Get_ADC(u8 ch);
void DAC_Config(void);


/**************************************************************************/
void BSP_Init(void)
{
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
      {;}

      RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
      
      while(RCC_GetSYSCLKSource() != 0x08)
      {;}    
     }
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	 //根据使用的引脚修改

    GPIO_Config();
    Interrupt_Config();
    SysTick_Config();
	COMM_Config();
	DMA_Config(DMA1_Channel2,(u32)&USART3->DR,(u32)SendBuff,50U);
	TIM4_PWM_Init(899,0);	 //不分频。PWM频率=72000/900=8Khz
	ADC_Config();
	DAC_Config();
}

static void SysTick_Config(void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    u32  cnts;

    RCC_GetClocksFreq(&rcc_clocks);
    
    cnts = (u32)rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC;

    SysTick_SetReload(cnts);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick_CounterCmd(SysTick_Counter_Enable);
    SysTick_ITConfig(ENABLE);
}

void GPIO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);	 //释放JTAG脚作为普通IO口
    //config the output's port which is the control of relay//		
   	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
    GPIO_InitStruct.GPIO_Pin = DO_LED_1|DO_LED_2|DO_LED_3|DO_LED_4;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);				
	
   	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);	
    GPIO_InitStruct.GPIO_Pin = SYS_RUN_LED|DO_LED_5|DO_LED_6|DO_LED_7|DO_LED_8|DO_LED_9|DO_LED_10|DO_LED_11|DO_LED_12;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE,&GPIO_InitStruct);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
    GPIO_InitStruct.GPIO_Pin = DO_LED_12|DO_LED_13|DO_LED_14|DO_LED_15|DO_LED_16;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOF,&GPIO_InitStruct);
		
    //config the ADG508's control port which is the mode selection of measure resistance
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);	 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5 |GPIO_Pin_6|GPIO_Pin_7;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG,&GPIO_InitStruct);
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 ;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStruct);
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);	 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11 ;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOF,&GPIO_InitStruct);
		
	//config the WDI's port//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);	 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 ;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG,&GPIO_InitStruct);
		
	//config the amplifier's gain which is the selection of mode//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStruct);
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_3 | GPIO_Pin_6;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);	 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG,&GPIO_InitStruct);
		
	//config the INPUT's collection port which is the show of value
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);	 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12 |GPIO_Pin_13|GPIO_Pin_14 | GPIO_Pin_15;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOG,&GPIO_InitStruct);
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);	 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE,&GPIO_InitStruct);
		
	//config the INPUT's button which is the randam input//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);	 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD,&GPIO_InitStruct);
}

void Interrupt_Config(void)
{
    //NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick,0,0);
}

void COMM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//configure comm3's gpios//
	RCC_APB2PeriphClockCmd(COMM3_GPIO_CLK | COMM3_Ctrl_CLK | COMM3_Stat_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(COMM3_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = COMM3_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(COMM3_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = COMM3_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(COMM3_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = COMM3_CtrlPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(COMM3_Ctrl_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = COMM3_RxLEDPin | COMM3_TxLEDPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(COMM3_Stat_GPIO,&GPIO_InitStructure);
	
    //configure comm2's gpios//
	RCC_APB2PeriphClockCmd(COMM2_GPIO_CLK | COMM2_Ctrl_CLK | COMM2_Stat_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(COMM2_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = COMM2_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(COMM2_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = COMM2_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(COMM2_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = COMM2_CtrlPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(COMM2_Ctrl_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = COMM2_RxLEDPin | COMM2_TxLEDPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(COMM2_Stat_GPIO,&GPIO_InitStructure);

	//configure comm1's gpios//
	RCC_APB2PeriphClockCmd(COMM1_GPIO_CLK | COMM1_Ctrl_CLK | COMM1_Stat_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(COMM1_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = COMM1_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(COMM1_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = COMM1_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(COMM1_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = COMM1_CtrlPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(COMM1_Ctrl_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = COMM1_RxLEDPin | COMM1_TxLEDPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(COMM1_Stat_GPIO,&GPIO_InitStructure);
	
	//configure the nested vectored interrupt controller//	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQChannel | USART2_IRQChannel | USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//configure para//
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	//configure comm3's para//
	USART_Init(USART3,&USART_InitStructure);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART3, ENABLE);
	
	//configure comm2's para//
	USART_Init(USART2,&USART_InitStructure);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2, ENABLE);

	//configure comm1's para//
	USART_Init(USART1,&USART_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1, ENABLE);
}

//comm3
void COMM3_RX_Mode(void)
{
	GPIO_ResetBits(COMM3_Ctrl_GPIO,COMM3_CtrlPin);
	GPIO_ResetBits(COMM3_Stat_GPIO,COMM3_RxLEDPin);
}

void COMM3_TX_Mode(void)
{
	GPIO_SetBits(COMM3_Ctrl_GPIO,COMM3_CtrlPin);
	GPIO_ResetBits(COMM3_Stat_GPIO,COMM3_TxLEDPin);
}

void COMM3_IDLE(void)
{
	GPIO_ResetBits(COMM3_Ctrl_GPIO,COMM3_CtrlPin);
	GPIO_SetBits(COMM3_Stat_GPIO,COMM3_RxLEDPin);
	GPIO_SetBits(COMM3_Stat_GPIO,COMM3_TxLEDPin);
}

//comm2
void COMM2_RX_Mode(void)
{
	GPIO_ResetBits(COMM2_Ctrl_GPIO,COMM2_CtrlPin);
	GPIO_ResetBits(COMM2_Stat_GPIO,COMM2_RxLEDPin);
}

void COMM2_TX_Mode(void)
{
	GPIO_SetBits(COMM2_Ctrl_GPIO,COMM2_CtrlPin);
	GPIO_ResetBits(COMM2_Stat_GPIO,COMM2_TxLEDPin);
}

void COMM2_IDLE(void)
{
	GPIO_ResetBits(COMM2_Ctrl_GPIO,COMM2_CtrlPin);
	GPIO_SetBits(COMM2_Stat_GPIO,COMM2_RxLEDPin);
	GPIO_SetBits(COMM2_Stat_GPIO,COMM2_TxLEDPin);
}

//comm1
void COMM1_RX_Mode(void)
{
	GPIO_ResetBits(COMM1_Ctrl_GPIO,COMM1_CtrlPin);
	GPIO_ResetBits(COMM1_Stat_GPIO,COMM1_RxLEDPin);
}

void COMM1_TX_Mode(void)
{
	GPIO_SetBits(COMM1_Ctrl_GPIO,COMM1_CtrlPin);
	GPIO_ResetBits(COMM1_Stat_GPIO,COMM1_TxLEDPin);
}

void COMM1_IDLE(void)
{
	GPIO_ResetBits(COMM1_Ctrl_GPIO,COMM1_CtrlPin);
	GPIO_SetBits(COMM1_Stat_GPIO,COMM1_RxLEDPin);
	GPIO_SetBits(COMM1_Stat_GPIO,COMM1_TxLEDPin);
}

u16 DMA1_MEM_LEN;//保存DMA每次数据传送的长度 
void DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA_CHx);
	DMA1_MEM_LEN = cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr =cpar;
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = cndtr;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA_CHx,&DMA_InitStructure);
}

void DMA_Enabale(DMA_Channel_TypeDef *DMA_CHx)
{
	DMA_Cmd(DMA_CHx, DISABLE);
	DMA_CHx->CNDTR = DMA1_MEM_LEN;
	DMA_Cmd(DMA_CHx, ENABLE);
}

void TIM4_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //Timer4
	
	 //设置该引脚为复用输出功能,输出TIM4 CH1 CH2 CH3 CH4的PWM脉冲波形	GPIOD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM4 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC1
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC2
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC3
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC4

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR3上的预装载寄存器
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR4上的预装载寄存器
 
	TIM_Cmd(TIM4, ENABLE);  //使能TIM3
}

void ADC_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1	, ENABLE );	

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	

	//PC0 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);		
	
	ADC_DeInit(ADC1);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器  
	
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准   
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	ADC_StartCalibration(ADC1);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}

u16 Get_ADC(u8 ch)
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}
void DAC_Config(void)
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
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
	DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
}

