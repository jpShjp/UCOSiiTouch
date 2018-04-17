/******************** (C) COPYRIGHT 2014 YIYA ********************
* File Name          : control_main.h
* Author             : yiya team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file contains all the functions prototypes for the
*                      control_main firmware library.
*******************************************************************************/
#include "ucos_ii.h"
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_MAIN_H
#define __CONTORL_MAIN_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define TASK_START_PRIO      		1

#define TASK_FUNCTION_PRIO   	    9
#define TASK_LOGIC_PRIO			 	8
#define	TASK_MMI_PRIO				7

#define	TASK_COMM1_RECE_PRIO		4
#define	TASK_COMM2_RECE_PRIO		5
#define	TASK_COMM3_RECE_PRIO		6
#define	TASK_COMM1_SEND_PRIO		11
//#define	TASK_COMM1_SEND_PRIO		11
//#define	TASK_COMM3_SEND_PRIO		12
//chunjie
//#define OS_TASK_TMR_PRIO           (OS_LOWEST_PRIO - 2)
//#define OS_TASK_TMR_PRIO           0
#define TASK_START_STK_SIZE         512

#define TASK_FUNCTION_STK_SIZE      1024
#define TASK_LOGIC_STK_SIZE			1024
#define	TASK_MMI_STK_SIZE	  		1024

#define	TASK_COMM1_RECE_STK_SIZE    2048
#define	TASK_COMM2_RECE_STK_SIZE    2048
#define	TASK_COMM3_RECE_STK_SIZE    2048
#define	TASK_COMM1_SEND_STK_SIZE    2048
extern OS_EVENT *Sem_Comm1Rece;
extern OS_EVENT *Sem_Comm2Rece;
extern OS_EVENT *Sem_Comm3Rece;
extern OS_EVENT *Sem_Comm1Send;
//chunjie

extern u8 comm1_master_wait_time_out;
extern u8 comm1_master_complete_send;
extern u8 comm1_master_complete_recv;
extern OS_TMR   *timer_100ms;
extern OS_TMR   *timer_50ms;
//extern OS_EVENT *Sem_Comm1Send;
//extern OS_EVENT *Sem_Comm2Send;
//extern OS_EVENT *Sem_Comm3Send;

/* GPIO Pin sources ----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#endif /* __CONTROL_MAIN_H */
/******************* (C) COPYRIGHT YIYA TEAM *****END OF FILE****/
