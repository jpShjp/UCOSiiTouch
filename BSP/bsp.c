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
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	 //����ʹ�õ������޸�

    GPIO_Config();
    Interrupt_Config();
    SysTick_Config();
	COMM_Config();
	DMA_Config(DMA1_Channel2,(u32)&USART3->DR,(u32)SendBuff,50U);
	TIM4_PWM_Init(899,0);	 //����Ƶ��PWMƵ��=72000/900=8Khz
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

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);	 //�ͷ�JTAG����Ϊ��ͨIO��
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

u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 
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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //Timer4
	
	 //���ø�����Ϊ�����������,���TIM4 CH1 CH2 CH3 CH4��PWM���岨��	GPIOD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM4 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC1
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC2
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC3
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC4

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR3�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR4�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM3
}

void ADC_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1	, ENABLE );	

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	

	//PC0 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);		
	
	ADC_DeInit(ADC1);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���  
	
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼   
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}

u16 Get_ADC(u8 ch)
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	
	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}
void DAC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //ʹ��PORTAͨ��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //ʹ��DACͨ��ʱ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //ģ������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4)	;//PA.4 �����
	GPIO_SetBits(GPIOA,GPIO_Pin_5)	;//PA.5 �����
					
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1
	DAC_Init(DAC_Channel_2,&DAC_InitType);	 //��ʼ��DACͨ��2

	DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DAC1
	DAC_Cmd(DAC_Channel_2, ENABLE);  //ʹ��DAC2
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
	DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
}

