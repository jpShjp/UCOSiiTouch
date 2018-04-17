/**************-*1***** (C) COPYRIGHT 2014 YIYA ********************
* File Name          : control_comm.c
* Author             : YIYA Team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file provides all the bsp firmware functions.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "ucos_ii.h"
#include "control_main.h"
#include "control_global.h"
#include "control_bsp.h"
#include "function_handle.h"
#include "control_comm.h"
#include "control_adc.h"
#include "stm32f10x_iwdg.h"
//应用添加--------------------
#include "TPK8CONTROL.h"
#include "myiic.h"
#include "delay.h"
#include "comm_handle.h"
#include "control_do.h"
//应用添加--------------------
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static OS_STK   TaskStartStk[TASK_START_STK_SIZE];
static OS_STK   TaskFunctionStk[TASK_FUNCTION_STK_SIZE];
static OS_STK    TaskLogicStk[TASK_LOGIC_STK_SIZE];
static OS_STK    TaskMMIStk[TASK_MMI_STK_SIZE];

static OS_STK    TaskComm1ReceStk[TASK_COMM1_RECE_STK_SIZE];
static OS_STK    TaskComm2ReceStk[TASK_COMM2_RECE_STK_SIZE];
static OS_STK    TaskComm3ReceStk[TASK_COMM3_RECE_STK_SIZE];
//static OS_STK    TaskComm1SendStk[TASK_COMM1_SEND_STK_SIZE];
static OS_STK    TaskComm1SendStk[TASK_COMM1_SEND_STK_SIZE];
//static OS_STK    TaskComm3SendStk[TASK_COMM3_SEND_STK_SIZE];

OS_EVENT *Sem_Comm1Rece;
OS_EVENT *Sem_Comm2Rece;
OS_EVENT *Sem_Comm3Rece;

//OS_EVENT *Sem_Comm1Send;
OS_EVENT *Sem_Comm1Send;
OS_EVENT *Sem_Com3Send;
//OS_EVENT *Sem_Comm3Send;

//chunjie
void timer_100ms_callback(OS_TMR *ptmr, void *p_arg);
OS_TMR   *timer_100ms;
OS_TMR   *timer_50ms;
u8      comm1_master_wait_time_out;
u8 		comm1_master_complete_send;
u8 		comm1_master_complete_recv;
u8      led_on = 0;

/* Private function ----------------------------------------------------------*/
static void TaskCreate(void *p_arg);

static void TaskFunction(void *p_arg);
static void TaskLogic(void *p_arg);
static void TaskMMI(void *p_arg);

static void TaskComm1Rece(void *p_arg);
static void TaskComm2Rece(void *p_arg);
static void TaskComm3Rece(void *p_arg);
//static void TaskComm1Send(void *p_arg);
static void TaskComm1Send(void *p_arg);
//static void TaskC
//static void TaskComm3Send(void *p_arg);

//Chunjie
//extern void tmr100ms_callback(OS_TMR *ptmr, void *p_arg);
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main(void)
* Description    : control system main function
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	OSInit();
	OSTaskCreate(TaskCreate,(void*)0,(OS_STK *)&TaskStartStk[TASK_START_STK_SIZE-1],TASK_START_PRIO);
	OSStart();
}

/*******************************************************************************
* Function Name  : TaskCreat(void)
* Description    : this is the first task of operation system, which major purpose
*									is to creat task
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void TaskCreate(void *p_arg)
{
    u8 err;
    u8 p[12] = "timer_100ms";
  (void)p_arg;

	BSP_Config();
	Control_Global_Init();

  #if (OS_TASK_STAT_EN > 0)
	OSStatInit();
  #endif
  
    timer_100ms=OSTmrCreate(10,10,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)timer_100ms_callback,0,p,&err);

 
  OSTaskCreate(TaskFunction,(void*)0,&TaskFunctionStk[TASK_FUNCTION_STK_SIZE-1],TASK_FUNCTION_PRIO);
  OSTaskCreate(TaskLogic,(void*)0,&TaskLogicStk[TASK_LOGIC_STK_SIZE-1],TASK_LOGIC_PRIO); // PID control
  OSTaskCreate(TaskMMI,(void*)0,&TaskMMIStk[TASK_MMI_STK_SIZE-1],TASK_MMI_PRIO);
//2016.1.24
  OSTaskCreate(TaskComm1Rece,(void*)0,&TaskComm1ReceStk[TASK_COMM1_RECE_STK_SIZE-1],TASK_COMM1_RECE_PRIO);
  OSTaskCreate(TaskComm2Rece,(void*)0,&TaskComm2ReceStk[TASK_COMM2_RECE_STK_SIZE-1],TASK_COMM2_RECE_PRIO);
//2016.1.24
  OSTaskCreate(TaskComm3Rece,(void*)0,&TaskComm3ReceStk[TASK_COMM3_RECE_STK_SIZE-1],TASK_COMM3_RECE_PRIO);
//2016.1.24
  //OSTaskCreate(TaskComm1Send,(void*)0,&TaskComm1SendStk[TASK_COMM1_SEND_STK_SIZE-1],TASK_COMM1_SEND_PRIO);
 // OSTaskCreate(TaskComm1Send,(void*)0,&TaskComm1SendStk[TASK_COMM1_SEND_STK_SIZE-1],TASK_COMM1_SEND_PRIO);
//2016.1.24
//  OSTaskCreate(TaskComm3Send,(void*)0,&TaskComm3SendStk[TASK_COMM3_SEND_STK_SIZE-1],TASK_COMM3_SEND_PRIO);


	//Sem_Comm1Send = OSSemCreate(0);
	Sem_Comm1Rece = OSSemCreate(0);
//	Sem_Comm1Send = OSSemCreate(0);
	Sem_Comm2Rece = OSSemCreate(0);
//	Sem_Comm3Send = OSSemCreate(0);
	Sem_Comm3Rece = OSSemCreate(0);
//OSTmrStart(timer_100ms, &err); //Start timer
    while(1)
    {
       OSTaskSuspend(OS_PRIO_SELF);
    }
}

void IWDG_Configuration(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	IWDG_SetPrescaler(IWDG_Prescaler_256);
IWDG_SetReload(315); 	// 2s
//IWDG_SetReload(188); // 1.2s
//IWDG_SetReload(125); // 0.8s
	IWDG_ReloadCounter();
	IWDG_Enable();
}


/*******************************************************************************
* Function Name  : TaskLogic(void)
* Description    : this is the user task of control system
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void TaskLogic(void *p_arg)
{

	static u8 init_flag;
	static u8 auto_flagz,auto_flagl,bpqerr_flagz,bpqerr_flagl;
	static s16 out_plz,out_pll;
	static u8 control_cycle_temp = 0;
	(void)p_arg;

	OSTimeDlyHMSM(0,0,1,0);
	//TPK8control_init();
	PID_control_parameter_init();

	init_flag = 1;

	while(init_flag == 1)
	{
		control_cycle_temp = (u8)ALL_DATA_RAM[PID_CONTROL_CYCLE];

		OSTimeDlyHMSM(0,0,control_cycle_temp,0);

    // lc 10142017 mask original control algrithm
		//TPK8control();
	  pid_control_top();
	}

}

void ADC_reg_init()
{
	float cal_coef[2], cal_offset[2];
	u16 *p, *p_des;

	cal_coef[0] = cal_coef[1] = 1.0;
	cal_offset[0] = cal_offset[1] = 0.0;


	////////////////////// ADC 1///////////////////
	//coef
	p = (u16 *)&cal_coef[0];
	p_des = (u16 *)&ALL_DATA_RAM[COEF_HI_ADC1];

	//lc 111917 swap high/low word as PLC does
	*p_des = *(p+1);
	*(p_des+1) = *p;

	// offset
	p = (u16 *)&cal_offset[0];
	p_des = (u16 *)&ALL_DATA_RAM[OFFSET_HI_ADC1];

	*p_des = *(p+1);
	*(p_des+1) = *p;

	////////////////////// ADC 2///////////////////
	//coef
	p = (u16 *)&cal_coef[1];
	p_des = (u16 *)&ALL_DATA_RAM[COEF_HI_ADC2];

	*p_des = *(p+1);
	*(p_des+1) = *p;

	// offset
	p = (u16 *)&cal_offset[0];
	p_des = (u16 *)&ALL_DATA_RAM[OFFSET_HI_ADC2];

	*p_des = *(p+1);
	*(p_des+1) = *p;
}

void AO_reg_init()
{
	float cal_start_th[4], cal_coef[4], cal_offset[4];
	u16 *p, *p_des;

	cal_start_th[0] = cal_start_th[1] = cal_start_th[2] = cal_start_th[3] = 4000; // 4v
	cal_coef[0] = cal_coef[1] = cal_coef[2] = cal_coef[3] = 0.9718;
	cal_offset[0] = cal_offset[1] = cal_offset[2] = cal_offset[3] = -1.0937;


	////////////////////// channel 1///////////////////
	ALL_DATA_RAM[CAL_START_TH_AO1] = cal_start_th[0];

	//coef
	p = (u16 *)&cal_coef[0];
	p_des = (u16 *)&ALL_DATA_RAM[COEF_HI_AO1];

	*p_des = *(p+1);
	*(p_des+1) = *p;

	// offset
	p = (u16 *)&cal_offset[0];
	p_des = (u16 *)&ALL_DATA_RAM[OFFSET_HI_AO1];

	*p_des = *(p+1);
	*(p_des+1) = *p;

	////////////////////// channel 2///////////////////
	ALL_DATA_RAM[CAL_START_TH_AO2] = cal_start_th[1];

	//coef
	p = (u16 *)&cal_coef[1];
	p_des = (u16 *)&ALL_DATA_RAM[COEF_HI_AO2];

	*p_des = *(p+1);
	*(p_des+1) = *p;

	// offset
	p = (u16 *)&cal_offset[0];
	p_des = (u16 *)&ALL_DATA_RAM[OFFSET_HI_AO2];

	*p_des = *(p+1);
	*(p_des+1) = *p;

	////////////////////// channel 3///////////////////
	ALL_DATA_RAM[CAL_START_TH_AO3] = cal_start_th[1];

	//coef
	p = (u16 *)&cal_coef[1];
	p_des = (u16 *)&ALL_DATA_RAM[COEF_HI_AO3];

	*p_des = *(p+1);
	*(p_des+1) = *p;

	// offset
	p = (u16 *)&cal_offset[0];
	p_des = (u16 *)&ALL_DATA_RAM[OFFSET_HI_AO3];

	*p_des = *(p+1);
	*(p_des+1) = *p;

	////////////////////// channel 4///////////////////
	ALL_DATA_RAM[CAL_START_TH_AO4] = cal_start_th[1];

	//coef
	p = (u16 *)&cal_coef[1];
	p_des = (u16 *)&ALL_DATA_RAM[COEF_HI_AO4];

	*p_des = *(p+1);
	*(p_des+1) = *p;

	// offset
	p = (u16 *)&cal_offset[0];
	p_des = (u16 *)&ALL_DATA_RAM[OFFSET_HI_AO4];

	*p_des = *(p+1);
	*(p_des+1) = *p;
}


/*******************************************************************************
* Function Name  : TaskFunction(void)
* Description    : this is the peripheral function of control system
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void TaskFunction(void *p_arg)
{
	(void)p_arg;


	IWDG_Configuration();
	ADC_reg_init();
	AO_reg_init();
 I2C_GPIO_Init();
	DO_reload();

	while(1)
	{
		IWDG_ReloadCounter();

		if(ALL_DATA_RAM[SYS_RUNORSTOP_BASE] == ENABLE)
		{
			P_Function_DO();
			P_Function_DI();
			P_Function_AO();
			P_Function_AI();
			OSTimeDlyHMSM(0,0,0,5);
		}
	}
}

static void TaskComm3Rece(void *p_arg)
{
	u8 err;

	(void)p_arg;

	while(1)
	{
		OSSemPend(Sem_Comm3Rece,0,&err);
		P_Comm3_Handle();
	}
}

static void TaskComm2Rece(void *p_arg)
{
	u8 err;

	(void)p_arg;


	while(1)
	{
		OSSemPend(Sem_Comm2Rece,0,&err);
		P_Comm2_Handle();
	}
}

//chunjie

static void TaskComm1Send(void *p_arg)
{
	u8 err;

	(void)p_arg;


	while(1)
	{
		OSSemPend(Sem_Comm1Send,0,&err);
		P_Comm1_Send_Handle();
        OSTimeDlyHMSM(0,0,5,0);
	}
}

static void TaskComm1Rece(void *p_arg)
{
	u8 err;

	(void)p_arg;

	while(1)
	{

		OSSemPend(Sem_Comm1Rece,0,&err);
		P_Comm1_Handle();
        // delay 5s
        //OSTimeDlyHMSM(0,0,5,0);
	}
}

static void TaskMMI(void *p_arg)
{
	(void)p_arg;
    
	while(1)
	{
		//P_Function_RunStatus(500);
        OSTimeDlyHMSM(0,0,5,0);
         // Read AI
        //MMSenDFuncode03Frame(0x1,0xc8,1);
        // Read all DO
        MMSenDFuncode03Frame(0x1,0x12c,0x00);
    
            

#if 0
		COMM2_SendBuf.DataBuf[0] = 0x55;
		COMM2_SendBuf.DataBuf[1] = 2;
		COMM2_SendBuf.DataBuf[2] = 2;
		COMM2_SendBuf.DataBuf[3] = 2;

		COMM2_SendBuf.DataLong    = 4;
		COMM2_SendBuf.CommChannel = 2;
		COMM2_SendBuf.DataCount   = 0;
		COMM2_SendBuf.OK          = 1;

		BSP_COMM_Send(COMM2_SendBuf);

		COMM3_SendBuf.DataBuf[0] = 0x55;
		COMM3_SendBuf.DataBuf[1] = 3;
		COMM3_SendBuf.DataBuf[2] = 3;
		COMM3_SendBuf.DataBuf[3] = 3;

		COMM3_SendBuf.DataLong    = 4;
		COMM3_SendBuf.CommChannel = 3;
		COMM3_SendBuf.DataCount   = 0;
		COMM3_SendBuf.OK          = 1;

		BSP_COMM_Send(COMM3_SendBuf);

		COMM1_SendBuf.DataBuf[0] = 0x55;
		COMM1_SendBuf.DataBuf[1] = 1;
		COMM1_SendBuf.DataBuf[2] = 1;
		COMM1_SendBuf.DataBuf[3] = 1;

		COMM1_SendBuf.DataLong    = 4;
		COMM1_SendBuf.CommChannel = 1;
		COMM1_SendBuf.DataCount   = 0;
		COMM1_SendBuf.OK          = 1;

		BSP_COMM_Send(COMM1_SendBuf);
#endif
	}
}

//chunjie

//------------ Timer --------------------------------

void timer_100ms_callback(OS_TMR *ptmr, void *p_arg) {
    u8 err;
    comm1_master_wait_time_out = 1;
   Sys_Run_LED(led_on & 0x01);
    led_on = ~led_on;
    // OSTmrStart(timer_100ms, &err); //Start timer
}
