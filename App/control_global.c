/**************-*1***** (C) COPYRIGHT 2014 YIYA ********************
* File Name          : control_global.c
* Author             : YIYA Team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : .
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "control_global.h"
#include "stm32f10x_it.h" 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
u8 System_Status[9];

u8 Parameter_Set_Type[18];
float Parameter_Set_Rang_Max[18];
float Parameter_Set_Rang_Min[18];
float Parameter_Set_Adjust[18];
float Parameter_Set_Gain[18];

u16 Function_Analog_Origin[20];
float Function_Analog_FINAL[20];

u8 Function_Digital[32];
//所有数据内存区
s16 ALL_DATA_RAM[512]; 

u32 TimeBug;
/* Private functions ---------------------------------------------------------*/
extern void Control_Global_Init(void);
/*******************************************************************************
* Function Name  : Control_Global_Init(void)
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Control_Global_Init(void)
{
	u8 i = 0;
  u32 TimeBug = 0;
	
	ALL_DATA_RAM[SYS_RUNORSTOP_BASE] 			= 1;     //run -- '0'; stop -- '1'
	ALL_DATA_RAM[SYS_NUMBER_BASE]         		= 1;
	ALL_DATA_RAM[TRANSVERT1_ERROR_BASE] 		= 0;     //normal -- '0' ; error -- '1'
	ALL_DATA_RAM[TRANSVERT2_ERROR_BASE]	 		= 0;
	ALL_DATA_RAM[TRANSVERT3_ERROR_BASE] 		= 0;
	ALL_DATA_RAM[TRANSVERT4_ERROR_BASE] 		= 0;
	ALL_DATA_RAM[FRE_IMBALANCE_ERROR_BASE] 	    = 0;
	ALL_DATA_RAM[DIF_PRESSURE_ERROR_BASE]		= 0;
	ALL_DATA_RAM[SENSOR_ERROR_BASE] 			= 0;
	ALL_DATA_RAM[PARAMETER_ERROR_BASE]		 	= 0;
	
	ALL_DATA_RAM[PRESSURE_SENSOR1_TYPE_BASE] 	  = CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[FIR_WATERPRETRANS_RANG_MAX_BASE] = 1600;	//max water pretrans is 1.6*1000(0.001mpa)
	ALL_DATA_RAM[FIR_WATERPRETRANS_RANG_MIN_BASE] = 0; 		//min water pretrans is 0*1000(0.001mpa)
	ALL_DATA_RAM[FIR_WATERPRETRANS_ADJUST_BASE]   = 0; 		//adjust offset is  0*1000 [0 0.3]0*1000(0.001mpa)
	ALL_DATA_RAM[FIR_WATERPRETRANS_GAIN_BASE] 	  = 1000; //adjust gain is  1*1000 [0.5 2]
	ALL_DATA_RAM[PRESSURE_SENSOR2_TYPE_BASE] 	  = CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[SEC_WATERPRETRANS_RANG_MAX_BASE] = 1600;	//max water pretrans is 1.6*1000(0.001mpa)
	ALL_DATA_RAM[SEC_WATERPRETRANS_RANG_MIN_BASE] = 0; 		//min water pretrans is 0*1000(0.001mpa)
	ALL_DATA_RAM[SEC_WATERPRETRANS_ADJUST_BASE]   = 0; 		//adjust offset is  0*1000 [0 0.3]0*1000(0.001mpa)
	ALL_DATA_RAM[SEC_WATERPRETRANS_GAIN_BASE] 	  = 1000; //adjust gain is  1*100 [0.5 2]
	ALL_DATA_RAM[PRESSURE_SENSOR3_TYPE_BASE] 	  = CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[THI_WATERPRETRANS_RANG_MAX_BASE] = 1600;	//max water pretrans is 1.6*1000(0.001mpa)
	ALL_DATA_RAM[THI_WATERPRETRANS_RANG_MIN_BASE] = 0; 		//min water pretrans is 0*1000(0.001mpa)
	ALL_DATA_RAM[THI_WATERPRETRANS_ADJUST_BASE]   = 0; 		//adjust offset is  0*1000 [0 0.3]0*1000(0.001mpa)
	ALL_DATA_RAM[THI_WATERPRETRANS_GAIN_BASE] 	  = 1000; 	//adjust gain is  1*1000 [0.5 2]
	ALL_DATA_RAM[PRESSURE_SENSOR4_TYPE_BASE] 	  = CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[FOU_WATERPRETRANS_RANG_MAX_BASE] = 1600;	//max water pretrans is 1.6*1000(0.001mpa)
	ALL_DATA_RAM[FOU_WATERPRETRANS_RANG_MIN_BASE] = 0; 		//min water pretrans is 0*1000(0.001mpa)
	ALL_DATA_RAM[FOU_WATERPRETRANS_ADJUST_BASE]   = 0; 		//adjust offset is  0*1000 [0 0.3]0*1000(0.001mpa)
	ALL_DATA_RAM[FOU_WATERPRETRANS_GAIN_BASE] 	  = 1000; 	//adjust gain is  1*1000 [0.5 2]
	
	ALL_DATA_RAM[TEMPERATURE_SENSOR1_TYPE_BASE]   = CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[FIR_WATERTEMTRANS_RANG_MAX_BASE] = 26000;	//max water pretrans is 260*100(0.01degree)
	ALL_DATA_RAM[FIR_WATERTEMTRANS_RANG_MIN_BASE] = -4000; 	//min water pretrans is -40*100(0.01degree)
	ALL_DATA_RAM[FIR_WATERTEMTRANS_ADJUST_BASE]   = 0; 		//adjust offset is  0*100 [-40 40](0.01degree)
	ALL_DATA_RAM[FIR_WATERTEMTRANS_GAIN_BASE] 	  = 100; 	//adjust gain is  1*100 [0.5 2]
	ALL_DATA_RAM[TEMPERATURE_SENSOR2_TYPE_BASE]   = CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[SEC_WATERTEMTRANS_RANG_MAX_BASE] = 26000;	//max water pretrans is 260*100(0.01degree)
	ALL_DATA_RAM[SEC_WATERTEMTRANS_RANG_MIN_BASE] = -4000; 	//min water pretrans is -40*100(0.01degree)
	ALL_DATA_RAM[SEC_WATERTEMTRANS_ADJUST_BASE]   = 0; 		//adjust offset is  0*100 [-40 40](0.01degree)
	ALL_DATA_RAM[SEC_WATERTEMTRANS_GAIN_BASE] 	  = 100; 	//adjust gain is  1*100 [0.5 2]
	ALL_DATA_RAM[TEMPERATURE_SENSOR3_TYPE_BASE]   = CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[THI_WATERTEMTRANS_RANG_MAX_BASE] = 26000;	//max water pretrans is 260*100(0.01degree)
	ALL_DATA_RAM[THI_WATERTEMTRANS_RANG_MIN_BASE] = -4000; 	//min water pretrans is -40*100(0.01degree)
	ALL_DATA_RAM[THI_WATERTEMTRANS_ADJUST_BASE]   = 0; 		//adjust offset is  0*100 [-40 40](0.01degree)
	ALL_DATA_RAM[THI_WATERTEMTRANS_GAIN_BASE] 	  = 100; 	//adjust gain is  1*100 [0.5 2]
	ALL_DATA_RAM[TEMPERATURE_SENSOR4_TYPE_BASE]   = CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[FOU_WATERTEMTRANS_RANG_MAX_BASE] = 26000;	//max water pretrans is 260*100 (0.01degree)
	ALL_DATA_RAM[FOU_WATERTEMTRANS_RANG_MIN_BASE] = -4000; 	//min water pretrans is -40*100(0.01degree)
	ALL_DATA_RAM[FOU_WATERTEMTRANS_ADJUST_BASE]   = 0; 		//adjust offset is  0*100 [-40 40](0.01degree)
	ALL_DATA_RAM[FOU_WATERTEMTRANS_GAIN_BASE] 	  = 100; 	//adjust gain is  1*100 [0.5 2]
	
	ALL_DATA_RAM[FREQUENCY_SENSOR1_TYPE_BASE] 	  = CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[FIR_WATERFRETRANS_RANG_MAX_BASE] = 6000;		//max water pretrans is 60*100(0.01hz)
	ALL_DATA_RAM[FIR_WATERFRETRANS_RANG_MIN_BASE] = 0; 		//min water pretrans is 0*100(0.01hz)
	ALL_DATA_RAM[FIR_WATERFRETRANS_ADJUST_BASE]   = 0; 		//adjust offset is  0*100(0.01hz)  [-10 10] 
	ALL_DATA_RAM[FIR_WATERFRETRANS_GAIN_BASE] 	  = 100; 	//adjust gain is  1*100  [0.5 2]
	ALL_DATA_RAM[FREQUENCY_SENSOR2_TYPE_BASE] 	  = CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[SEC_WATERFRETRANS_RANG_MAX_BASE] = 6000;		//max water pretrans is 60*100(0.01hz)
	ALL_DATA_RAM[SEC_WATERFRETRANS_RANG_MIN_BASE] = 0; 		//min water pretrans is 0*100(0.01hz)
	ALL_DATA_RAM[SEC_WATERFRETRANS_ADJUST_BASE]   = 0; 		//adjust offset is  0*100(0.01hz  [-10 10]) 
	ALL_DATA_RAM[SEC_WATERFRETRANS_GAIN_BASE] 	  = 100; 	//adjust gain is  1*100  [0.5 2]
	ALL_DATA_RAM[FREQUENCY_SENSOR3_TYPE_BASE] 	  = CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[THI_WATERFRETRANS_RANG_MAX_BASE] = 6000;		//max water pretrans is 60*100(0.01hz)
	ALL_DATA_RAM[THI_WATERFRETRANS_RANG_MIN_BASE] = 0; 		//min water pretrans is 0*100(0.01hz)
	ALL_DATA_RAM[THI_WATERFRETRANS_ADJUST_BASE]   = 0; 		//adjust offset is  0*100(0.01hz)  [-10 10] 
	ALL_DATA_RAM[THI_WATERFRETRANS_GAIN_BASE] 	  = 100; 	//adjust gain is  1*100  [0.5 2]
	ALL_DATA_RAM[FREQUENCY_SENSOR4_TYPE_BASE] 	  = CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[FOU_WATERFRETRANS_RANG_MAX_BASE] = 6000;		//max water pretrans is 60*100(0.01hz)
	ALL_DATA_RAM[FOU_WATERFRETRANS_RANG_MIN_BASE] = 0; 		//min water pretrans is 0*100(0.01hz)
	ALL_DATA_RAM[FOU_WATERFRETRANS_ADJUST_BASE]   = 0; 		//adjust offset is  0*100 (0.01hz) [-10 10] 
	ALL_DATA_RAM[FOU_WATERFRETRANS_GAIN_BASE] 	  = 100; 	//adjust gain is  1*100  [0.5 2]
	
	ALL_DATA_RAM[VALVE_FEEDBACK1_TYPE_BASE] 		= CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[FIR_VALVEFB_RANG_MAX_BASE] 		= 100;	//max water pretrans is 1*100
	ALL_DATA_RAM[FIR_VALVEFB_RANG_MIN_BASE] 		= 0; 		//min water pretrans is 0*100
	ALL_DATA_RAM[FIR_VALVEFB_ADJUST_BASE] 			= 0; 		//adjust offset is  0*100 [-0.2 0.2] 
	ALL_DATA_RAM[FIR_VALVEFB_GAIN_BASE] 					= 100; 	//adjust gain is  1*100  [0.8 1.2]
	ALL_DATA_RAM[VALVE_FEEDBACK2_TYPE_BASE] 			= CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[SEC_VALVEFB_RANG_MAX_BASE] 			= 100;	//max water pretrans is 1*100
	ALL_DATA_RAM[SEC_VALVEFB_RANG_MIN_BASE] 			= 0; 		//min water pretrans is 0*100
	ALL_DATA_RAM[SEC_VALVEFB_ADJUST_BASE] 				= 0; 		//adjust offset is  0*100 [-0.2 0.2] 
	ALL_DATA_RAM[SEC_VALVEFB_GAIN_BASE] 					= 100; 	//adjust gain is  1*100  [0.8 1.2]
	ALL_DATA_RAM[VALVE_FEEDBACK3_TYPE_BASE] 			= CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[THI_VALVEFB_RANG_MAX_BASE] 			= 100;	//max water pretrans is 1*100
	ALL_DATA_RAM[THI_VALVEFB_RANG_MIN_BASE] 			= 0; 		//min water pretrans is 0*100
	ALL_DATA_RAM[THI_VALVEFB_ADJUST_BASE] 				= 0; 		//adjust offset is  0*100 [-0.2 0.2] 
	ALL_DATA_RAM[THI_VALVEFB_GAIN_BASE] 					= 100; 	//adjust gain is  1*100  [0.8 1.2]
	ALL_DATA_RAM[VALVE_FEEDBACK4_TYPE_BASE] 			= CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[FOU_VALVEFB_RANG_MAX_BASE] 			= 100;	//max water pretrans is 1*100
	ALL_DATA_RAM[FOU_VALVEFB_RANG_MIN_BASE] 			= 0; 		//min water pretrans is 0*100
	ALL_DATA_RAM[FOU_VALVEFB_ADJUST_BASE] 				= 0; 		//adjust offset is  0*100 [-0.2 0.2] 
	ALL_DATA_RAM[FOU_VALVEFB_GAIN_BASE] 					= 100; 	//adjust gain is  1*100  [0.8 1.2]
	
	ALL_DATA_RAM[DIF_PRESSURE_TYPE_BASE] 					= CURRENT_COLLECT_MODE1;
	ALL_DATA_RAM[DIF_PRESSURE_RANG_MAX_BASE] 			= 600;		//max water pretrans is 0.6*1000 (0.001mpa)
	ALL_DATA_RAM[DIF_PRESSURE_RANG_MIN_BASE] 			= 0; 		//min water pretrans is 0*1000(0.001mpa)
	ALL_DATA_RAM[DIF_PRESSURE_ADJUST_BASE] 				= 0; 		//adjust offset is  0*1000(0.001mpa) [-0.2 0.2] 
	ALL_DATA_RAM[DIF_PRESSURE_GAIN_BASE] 					= 1000; 	//adjust gain is  1*1000  [0.8 1.2]
	
//	ALL_DATA_RAM[OUT_TEMPERATURE_TYPE_BASE] 			= CURRENT_COLLECT_MODE1;
//	ALL_DATA_RAM[OUT_TEMTRANS_RANG_MAX_BASE] 			= 5000;		//max water pretrans is 50*100(0.01degree)
//	ALL_DATA_RAM[OUT_TEMTRANS_RANG_MIN_BASE] 			= -5000; 		//min water pretrans is -50*100(0.01degree)
//	ALL_DATA_RAM[OUT_TEMTRANS_ADJUST_BASE] 				= 0; 		//adjust offset is  0*100(0.01degree) [-20 20] 
//	ALL_DATA_RAM[OUT_TEMTRANS_GAIN_BASE] 					= 100; 	//adjust gain is  1*100  [0.4 1.2]
	
	ALL_DATA_RAM[OUT1_TYPE_BASE] = 2;
	ALL_DATA_RAM[OUT2_TYPE_BASE] = 2;
	ALL_DATA_RAM[OUT3_TYPE_BASE] = 2;
	ALL_DATA_RAM[OUT4_TYPE_BASE] = 2;
	
	ALL_DATA_RAM[AI_GROUP1_TYPE_BASE] = 1; 
	ALL_DATA_RAM[AI_GROUP2_TYPE_BASE] = 1;

	ALL_DATA_RAM[WINTER_OUTDOOR_TEMPERATURE_SET_BASE] = -1000;//coefficient is 100,rang is [-35 50],dimension is (0.01degree centigrade)
	ALL_DATA_RAM[WINTER_INDOOR_TEMPERATURE_SET_BASE]  = 1800;//coefficient is 100,rang is [16  28],dimension is (0.01degree centigrade)
	ALL_DATA_RAM[WINTER_DIF_PRESSURE_MAX_SET_BASE]    = 200;//coefficient is 1000,rang is [0.05 0.8],dimension is (.001MPa(megapascal))
	ALL_DATA_RAM[WINTER_DIF_PRESSURE_MIN_SET_BASE]    = 50;//coefficient is 1000,rang is [0.05 0.8],dimension is (.001MPa(megapascal))   
	ALL_DATA_RAM[WINTER_FREQUENCY_MAX_SET_BASE]       = 5000;//coefficient is 100,rang is [0 50],dimension is (0.01Hz)
	ALL_DATA_RAM[WINTER_FREQUENCY_MIN_SET_BASE]       = 2500;//coefficient is 100,rang is [0 50],dimension is (0.01Hz)
	ALL_DATA_RAM[WINTER_STARTPOINT_OUTDOOR_TEMPERATURE_SET_BASE] = 500;//coefficient is 100,rang is [0 10],dimension is (0.01degree centigrade)
	ALL_DATA_RAM[WINTER_WATER_SUPPLY_MAX_TEMPERATURE_SET_BASE]   = 5500;//coefficient is 100,rang is [0 160],dimension is (0.01degree centigrade
	ALL_DATA_RAM[WINTER_WATER_SUPPLY_MIN_TEMPERATURE_SET_BASE]   = 4500;//coefficient is 100,rang is [0 160],dimension is (0.01degree centigrade

	ALL_DATA_RAM[SUMMER_OUTDOOR_TEMPERATURE_SET_BASE] = 3300;//coefficient is 100,rang is [20 40],dimension is degree centigrade
	ALL_DATA_RAM[SUMMER_INDOOR_TEMPERATURE_SET_BASE]  = 2600;//coefficient is 100,rang is [20  33],dimension is degree centigrade
	ALL_DATA_RAM[SUMMER_DIF_PRESSURE_MAX_SET_BASE]    = 200;//coefficient is 1000,rang is [0.05 0.8],dimension is MPa(megapascal)
	ALL_DATA_RAM[SUMMER_DIF_PRESSURE_MIN_SET_BASE]    = 50;//coefficient is 1000,rang is [0.05 0.8],dimension is MPa(megapascal)   
	ALL_DATA_RAM[SUMMER_FREQUENCY_MAX_SET_BASE] = 5000;//coefficient is 100,rang is [0 50],dimension is Hz
	ALL_DATA_RAM[SUMMER_FREQUENCY_MIN_SET_BASE] = 2500;//coefficient is 100,rang is [0 50],dimension is Hz
	ALL_DATA_RAM[SUMMER_STARTPOINT_OUTDOOR_TEMPERATURE_SET_BASE] = 2800;//coefficient is 100,rang is [20 35],dimension is degree centigrade
	ALL_DATA_RAM[SUMMER_WATER_SUPPLY_MAX_TEMPERATURE_SET_BASE] = 1000;//coefficient is 100,rang is [5 20],dimension is degree centigrade
	ALL_DATA_RAM[SUMMER_WATER_SUPPLY_MIN_TEMPERATURE_SET_BASE] = 500;//coefficient is 100,rang is [5 20],dimension is degree centigrade

	ALL_DATA_RAM[SEASON_WORK_CONDITION_SET_BASE] = 1;//1--work in winter; 0-- work in summer
	ALL_DATA_RAM[ACCELERATION_TIME_OUT_FREQUENCY_SET_BASE] = 20;//coefficient is 1;rang is [0 999],dimension is second
	ALL_DATA_RAM[DECELERATION_TIME_OUT_FREQUENCY_SET_BASE] = 20;//coefficient is 1;rang is [0 999],dimension is second
	ALL_DATA_RAM[FILTER_TIME_IN_SIGNAL_SET_BASE] = 5;//coefficient is 1;rang is [0 60],dimension is second
	ALL_DATA_RAM[METHOD_STOPPING_PUMP_SET_BASE] = 1;//1--stop at anytime; 0--stop fellow integral time
	ALL_DATA_RAM[INCREASE_PUMP_DELAY_TIME_SET_BASE] = 20;//coefficient is 1;rang is [0 900],dimension is second
	ALL_DATA_RAM[DECREASE_PUMP_DELAY_TIME_SET_BASE] = 20;//coefficient is 1;rang is [0 900],dimension is second
	ALL_DATA_RAM[INCREASE_PUMP_PLUS_DIF_PRESSURE_SET_BASE] = 20;//coefficient is 1000;rang is [0 0.1],dimension is MPa
	ALL_DATA_RAM[DECREASE_PUMP_MINUS_DIF_PRESSURE_SET_BASE] = 20;//coefficient is 1000;rang is [0 0.1],dimension is MPa
	ALL_DATA_RAM[SWITCH_PUMP_PLUS_DIF_PRESSURE_SET_BASE] = 20;//coefficient is 1000;rang is [0 0.1],dimension is MPa
    ALL_DATA_RAM[SWITCH_PUMP_MINUS_DIF_PRESSURE_SET_BASE] = 20;//coefficient is 1000;rang is [0 0.1],dimension is MPa
	ALL_DATA_RAM[MIN_FREQUENCY_STOP_PUMP_SET_BASE] = 1;//1-- permit stop at min-frequency; 0-- not permit
	
	ALL_DATA_RAM[CONTROL_CYCLE_SET_BASE] = 10;//coefficient is 1;rang is [1 200];dimension is second
	ALL_DATA_RAM[CONTROL_METHOD_SET_BASE] = 1;//0-- calefaction; 1-- refrigeration
	ALL_DATA_RAM[PID_GENERAL_PROPORTION_BASE] = 3;//coefficient is 1;rang is [0 8]
	ALL_DATA_RAM[PID_GENERAL_INTEGRAL_TIME_BASE] = 60;//coefficient is 1;rang is [1 800]
	ALL_DATA_RAM[PID_TEMPERATURE_CONTROL_PROPORTION_BASE] = 3;//coefficient is 1;rang is [0 8]
	ALL_DATA_RAM[PID_TEMPERATURE_CONTROL_INTEGRAL_TIME_BASE] = 60;//coefficient is 1;rang is [1 800]
	ALL_DATA_RAM[PID_PRESSURE_CONTROL_PROPORTION_BASE] = 3;//coefficient is 1;rang is [0 8]
	ALL_DATA_RAM[PID_PRESSURE_CONTROL_INTEGRAL_TIME_BASE] = 60;//coefficient is 1;rang is [1 800]
		
	for(i=0; i<18; i++)
	{
		Parameter_Set_Type[i] = VOLTAGE_COLLECT_MODE1;
	}	
	TimeBug = 0;
}
