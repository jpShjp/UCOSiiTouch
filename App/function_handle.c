/**************-*1***** (C) COPYRIGHT 2014 YIYA ********************
* File Name          : function_handle.c
* Author             : YIYA Team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file provides all the bsp firmware functions.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "function_handle.h"
#include "ucos_ii.h"
#include "control_do.h"
#include "control_di.h"
#include "control_dac.h"
#include "control_adc.h"
#include "control_global.h"
#include "stm32f10x_adc.h"
#include "24cxx.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void P_Function_DO(void);
void P_Function_DI(void);
void P_Function_AO(void);
void P_Function_AI(void);
void P_Function_RunStatus(u16 blinktimeMS);
float to_float(u16 org_hi, u16 org_lo);

s16 DO_realtime[16];

/*******************************************************************************
* Function Name  : BSP_Comm_Config(void)
* Description    : Config comm1, comm2, comm3
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void P_Function_AI(void)
{
	OS_CPU_SR  cpu_sr;
	static float adc_value_filter_stack_1[Filter_num] 		= {0};
	static float adc_value_filter_stack_9[Filter_num] 		= {0};
	static float adc_value_filter_stack_2[Filter_num] 		= {0};
	static float adc_value_filter_stack_10[Filter_num] 		= {0};
	static float adc_value_filter_stack_3[Filter_num] 		= {0};
	static float adc_value_filter_stack_11[Filter_num] 		= {0};
	static float adc_value_filter_stack_4[Filter_num] 		= {0};
	static float adc_value_filter_stack_12[Filter_num] 		= {0};
	static float adc_value_filter_stack_5[Filter_num] 		= {0};
	static float adc_value_filter_stack_13[Filter_num] 		= {0};
	static float adc_value_filter_stack_6[Filter_num] 		= {0};
	static float adc_value_filter_stack_14[Filter_num]		= {0};
	static float adc_value_filter_stack_7[Filter_num] 		= {0};
	static float adc_value_filter_stack_15[Filter_num] 		= {0};
	static float adc_value_filter_stack_8[Filter_num] 		= {0};
	static float adc_value_filter_stack_16[Filter_num] 		= {0};
	
	static float ao_feedback_stack_1[Filter_num] = {0};
	static float ao_feedback_stack_2[Filter_num] = {0};
	static float ao_feedback_stack_3[Filter_num] = {0};
	static float ao_feedback_stack_4[Filter_num] = {0};
	static float ao_feedback_stack_5[Filter_num] = {0};
	static float ao_feedback_stack_6[Filter_num] = {0};
		
	static u16 adc_original_value_temp;
	//static u16 adc_type_temp1, adc_type_temp2;
	u16 AI_mode_ch[4];
	u16 DAC_out;
	static u8 i = 0;
	
	static float adc_value_temp;
	static float temp_f1,temp_f2,temp_f3,temp_f4;
	static s16 adc_value_temp_s;
	//static float adc_ad2mv_f = 1.46484375; //ref(3000mv)*gain(2)/maxrang(4096)
//	static float adc_ad2mv_f   = 2.67; //ref(2500mv)*gain(4.31221719457)/maxrang(4096)

  // lc 10142017 change the coef of adc raw data to voltage
  // the division resistors are 22.1k and 73.2k respectively
	# define PROPORTION (73.2+22.1)/22.1
	//# define PROPORTION 1
  # define COEF 2500.0*1/4096 * PROPORTION
	static float adc_ad2mv_f = COEF; //ref(2500mv)*gain(1)/maxrang(4096)

	static float adc_4ma_zero_f = 1;//0.004a*250ohm
//	static float adc_4ma_zero_f = 2;//0.004a*500ohm
	static float zero_off = -0;
//	static float zero_off = -35;
	float cala_temp_f = 0;
	
	s16 cal_coef_hi[2], cal_coef_lo[2], cal_offset_hi[2], cal_offset_lo[2];
	float	cal_coef[2], cal_offset[2];
	
	// lc 111117 deselect resistor mode
	BSP_AI_Select_Mode(0);

	OS_ENTER_CRITICAL();
	//adc_type_temp1 = ALL_DATA_RAM[AI_GROUP1_TYPE_BASE];
	//adc_type_temp2 = ALL_DATA_RAM[AI_GROUP2_TYPE_BASE];
	AI_mode_ch[0] = ALL_DATA_RAM[AI_MODE_CH1];
	AI_mode_ch[1] = ALL_DATA_RAM[AI_MODE_CH2];
	AI_mode_ch[2] = ALL_DATA_RAM[AI_MODE_CH3];
	AI_mode_ch[3] = ALL_DATA_RAM[AI_MODE_CH4];
	DAC_out = ALL_DATA_RAM[DAC_OUTPUT];

	// lc 111917 notice the high part is actually the low part. swap them in to_float
	cal_coef_hi[0] = ALL_DATA_RAM[COEF_HI_ADC1];
	cal_coef_lo[0] = ALL_DATA_RAM[COEF_LO_ADC1];
	cal_offset_hi[0] = ALL_DATA_RAM[OFFSET_HI_ADC1];
	cal_offset_lo[0] = ALL_DATA_RAM[OFFSET_LO_ADC1];

	cal_coef_hi[1] = ALL_DATA_RAM[COEF_HI_ADC2];
	cal_coef_lo[1] = ALL_DATA_RAM[COEF_LO_ADC2];
	cal_offset_hi[1] = ALL_DATA_RAM[OFFSET_HI_ADC2];
	cal_offset_lo[1] = ALL_DATA_RAM[OFFSET_LO_ADC2];

	OS_EXIT_CRITICAL();
	
	//lc 111117 update current output
	BSP_MeasurResist_CurrentOut_value(DAC_out);
	
	cal_coef[0] = to_float(cal_coef_hi[0], cal_coef_lo[0]);
	cal_offset[0] = to_float(cal_offset_hi[0], cal_offset_lo[0]);
	cal_coef[1] = to_float(cal_coef_hi[1], cal_coef_lo[1]);
	cal_offset[1] = to_float(cal_offset_hi[1], cal_offset_lo[1]);

	////////////////chanal -1 and -9/////////////////////	
	//if(adc_type_temp1 == 3) //采集电阻
	if (AI_mode_ch[0] == 3)
	{
		BSP_AI_Select_Mode(1);
	}	
	BSP_AI_SelectChannel(1); //压力传感器1*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_11);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[0] + cal_offset[0];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off; //原始值转化为MV
	// lc 111917 FIXME should calibrate the original ADC samping value?
	adc_value_temp = adc_value_temp*cal_coef[0] + cal_offset[0];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_1[i] =  adc_value_filter_stack_1[i+1];
	}
	adc_value_filter_stack_1[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_1[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC1_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[FIR_WATERPRETRANS_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[FIR_WATERPRETRANS_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[FIR_WATERPRETRANS_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[FIR_WATERPRETRANS_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f); //和4ma的差压V
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 4000; //采集设备范围(1-5V)*物理量缩放倍数(1000)
	adc_value_temp = adc_value_temp*((temp_f1 - temp_f2)/cala_temp_f) + temp_f2/1000;	
	adc_value_temp = adc_value_temp + (temp_f3/1000);
	adc_value_temp = adc_value_temp * (temp_f4/1000);
	adc_value_temp = adc_value_temp * 1000;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC1_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();
	//
	//BSP_AI_SelectChannel(9); //频率传感器1*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_12);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[1] + cal_offset[1];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[1] + cal_offset[1];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_9[i] =  adc_value_filter_stack_9[i+1];
	}
	adc_value_filter_stack_9[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_9[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC9_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[FIR_WATERFRETRANS_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[FIR_WATERFRETRANS_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[FIR_WATERFRETRANS_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[FIR_WATERFRETRANS_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f);//和4ma的压差
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 400; //采集设备范围(1-5V)*物理量缩放倍数(100)
	adc_value_temp = adc_value_temp*((temp_f1 - temp_f2)/cala_temp_f) + temp_f2/100;	
	adc_value_temp = adc_value_temp + (temp_f3/100);
	adc_value_temp = adc_value_temp * (temp_f4/100);
	adc_value_temp = adc_value_temp * 100;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC9_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();
	
	///////////////channal -2 and -10///////////////////////////	
	if (AI_mode_ch[1] == 3) //接热敏电阻
	{
		BSP_AI_Select_Mode(2);
	}	
	BSP_AI_SelectChannel(2);//压力传感器2*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_11);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[0] + cal_offset[0];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[0] + cal_offset[0];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_2[i] =  adc_value_filter_stack_2[i+1];
	}
	adc_value_filter_stack_2[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_2[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC2_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[SEC_WATERPRETRANS_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[SEC_WATERPRETRANS_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[SEC_WATERPRETRANS_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[SEC_WATERPRETRANS_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f);//和4ma的压差V
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 4000; //采集设备范围(1-5V)*物理量缩放倍数(1000)
	adc_value_temp = adc_value_temp*((temp_f1 - temp_f2)/cala_temp_f) + temp_f2/1000;	
	adc_value_temp = adc_value_temp + (temp_f3/1000);
	adc_value_temp = adc_value_temp * (temp_f4/1000);
	adc_value_temp = adc_value_temp * 1000;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////	
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC2_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();
	//
	//BSP_AI_SelectChannel(10);//频率传感器2*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_12);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[1] + cal_offset[1];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[1] + cal_offset[1];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_10[i] =  adc_value_filter_stack_10[i+1];
	}
	adc_value_filter_stack_10[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_10[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC10_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[SEC_WATERFRETRANS_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[SEC_WATERFRETRANS_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[SEC_WATERFRETRANS_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[SEC_WATERFRETRANS_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f);//和4ma的压差
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 400; //采集设备范围(1-5V)*物理量缩放倍数(100)
	adc_value_temp = adc_value_temp*((temp_f1 - temp_f2)/cala_temp_f) + temp_f2/100;	
	adc_value_temp = adc_value_temp + (temp_f3/100);
	adc_value_temp = adc_value_temp * (temp_f4/100);
	adc_value_temp = adc_value_temp * 100;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC10_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();
	
	///////////////channal -3 and -11///////////////////////////	
	if (AI_mode_ch[2] == 3) //接热敏电阻
	{
		BSP_AI_Select_Mode(3);
	}	
	BSP_AI_SelectChannel(3);//压力传感器3*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_11);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[0] + cal_offset[0];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[0] + cal_offset[0];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_3[i] =  adc_value_filter_stack_3[i+1];
	}
	adc_value_filter_stack_3[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_3[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC3_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[THI_WATERPRETRANS_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[THI_WATERPRETRANS_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[THI_WATERPRETRANS_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[THI_WATERPRETRANS_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f);//和4ma的压差V
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 4000; //采集设备范围(1-5V)*物理量缩放倍数(1000)
	adc_value_temp = adc_value_temp*((temp_f1 - temp_f2)/cala_temp_f) + temp_f2/1000;	
	adc_value_temp = adc_value_temp + (temp_f3/1000);
	adc_value_temp = adc_value_temp * (temp_f4/1000);
	adc_value_temp = adc_value_temp * 1000;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////	
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC3_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();
	//
	//BSP_AI_SelectChannel(11);//频率传感器3*********************************
	OSTimeDly(2);
	adc_original_value_temp = Get_ADC1(ADC_Channel_12);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[1] + cal_offset[1];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[1] + cal_offset[1];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_11[i] =  adc_value_filter_stack_11[i+1];
	}
	adc_value_filter_stack_11[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_11[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC11_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[THI_WATERFRETRANS_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[THI_WATERFRETRANS_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[THI_WATERFRETRANS_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[THI_WATERFRETRANS_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f);//和4ma的压差
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 400; //采集设备范围(1-5V)*物理量缩放倍数(100)
	adc_value_temp = adc_value_temp*((temp_f1 - temp_f2)/cala_temp_f) + temp_f2/100;	
	adc_value_temp = adc_value_temp + (temp_f3/100);
	adc_value_temp = adc_value_temp * (temp_f4/100);
	adc_value_temp = adc_value_temp * 100;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC11_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();
	
		///////////////channal -4 and -12///////////////////////////	
	if (AI_mode_ch[3] == 3) //接热敏电阻
	{
		BSP_AI_Select_Mode(4);
	}	
	BSP_AI_SelectChannel(4);//压力传感器4*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_11);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[0] + cal_offset[0];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[0] + cal_offset[0];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_4[i] =  adc_value_filter_stack_4[i+1];
	}
	adc_value_filter_stack_4[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_4[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC4_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[FOU_WATERPRETRANS_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[FOU_WATERPRETRANS_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[FOU_WATERPRETRANS_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[FOU_WATERPRETRANS_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f);//和4ma的压差V
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 8000; //采集设备范围(2-10V)*物理量缩放倍数(1000)
	adc_value_temp = adc_value_temp*((temp_f1 - temp_f2)/cala_temp_f) + temp_f2/1000;	
	adc_value_temp = adc_value_temp + (temp_f3/1000);
	adc_value_temp = adc_value_temp * (temp_f4/1000);
	adc_value_temp = adc_value_temp * 1000;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC4_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();
	//
	//BSP_AI_SelectChannel(12);//频率传感器4*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_12);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[1] + cal_offset[1];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[1] + cal_offset[1];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_12[i] =  adc_value_filter_stack_12[i+1];
	}
	adc_value_filter_stack_12[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_12[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC12_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[FOU_WATERFRETRANS_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[FOU_WATERFRETRANS_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[FOU_WATERFRETRANS_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[FOU_WATERFRETRANS_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f);//和4ma的压差
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 400; //采集设备范围(1-5V)*物理量缩放倍数(100)
	adc_value_temp = adc_value_temp*((temp_f1 - temp_f2)/cala_temp_f) + temp_f2/100;	
	adc_value_temp = adc_value_temp + (temp_f3/100);
	adc_value_temp = adc_value_temp * (temp_f4/100);
	adc_value_temp = adc_value_temp * 100;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC12_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();
	
		///////////////channal -5 and -13///////////////////////////	
	/*if(adc_type_temp1 == 3) //采集电阻
	{
		BSP_AI_Select_Mode(5);
	}*/	
	BSP_AI_SelectChannel(5);//温度传感器1*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_11);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[0] + cal_offset[0];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[0] + cal_offset[0];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_5[i] =  adc_value_filter_stack_5[i+1];
	}
	adc_value_filter_stack_5[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_5[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC5_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[FIR_WATERTEMTRANS_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[FIR_WATERTEMTRANS_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[FIR_WATERTEMTRANS_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[FIR_WATERTEMTRANS_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f); //采集电压差
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 400; //采集设备范围(1-5V)*物理量缩放倍数(100)
	adc_value_temp = adc_value_temp * ((temp_f1 - temp_f2)/cala_temp_f) + temp_f2/100;//(采集压差*（量程温差/量程压差）+最小量程）	
	adc_value_temp = adc_value_temp + (temp_f3/100);
	adc_value_temp = adc_value_temp * (temp_f4/100);
	adc_value_temp = adc_value_temp * 100;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC5_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();
	//
	//BSP_AI_SelectChannel(13);//调节阀反馈1*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_12);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[1] + cal_offset[1];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[1] + cal_offset[1];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_13[i] =  adc_value_filter_stack_13[i+1];
	}
	adc_value_filter_stack_13[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_13[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC13_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[FIR_VALVEFB_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[FIR_VALVEFB_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[FIR_VALVEFB_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[FIR_VALVEFB_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f); //采集电压差
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 400; //采集设备范围(1-5V)*物理量缩放倍数(100)
	adc_value_temp = adc_value_temp*((temp_f1 - temp_f2)/cala_temp_f) + temp_f2/100;	
	adc_value_temp = adc_value_temp + (temp_f3/100);
	adc_value_temp = adc_value_temp * (temp_f4/100);
	adc_value_temp = adc_value_temp * 100;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC13_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();
	
	///////////////channal -6 and -14///////////////////////////	
	/*if(adc_type_temp1 == 3) //采集电阻
	{
		BSP_AI_Select_Mode(6);
	}*/	
	BSP_AI_SelectChannel(6);//温度传感器2*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_11);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[0] + cal_offset[0];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[0] + cal_offset[0];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_6[i] =  adc_value_filter_stack_6[i+1];
	}
	adc_value_filter_stack_6[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_6[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC6_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[SEC_WATERTEMTRANS_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[SEC_WATERTEMTRANS_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[SEC_WATERTEMTRANS_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[SEC_WATERTEMTRANS_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f); //采集电压差
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 400; //采集设备范围(1-5V)*物理量缩放倍数(100)
	adc_value_temp = adc_value_temp * ((temp_f1	- temp_f2)/cala_temp_f) + temp_f2/100;	
	adc_value_temp = adc_value_temp + (temp_f3/100);
	adc_value_temp = adc_value_temp * (temp_f4/100);
	adc_value_temp = adc_value_temp * 100;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC6_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();
	//
	//BSP_AI_SelectChannel(14);//调节阀反馈2*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_12);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[1] + cal_offset[1];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[1] + cal_offset[1];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_14[i] =  adc_value_filter_stack_14[i+1];
	}
	adc_value_filter_stack_14[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_14[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC14_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[SEC_VALVEFB_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[SEC_VALVEFB_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[SEC_VALVEFB_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[SEC_VALVEFB_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_ad2mv_f);//原始值转化为mv
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 400; //采集设备范围(1-5V)*物理量缩放倍数(100)
	adc_value_temp = adc_value_temp*((temp_f1 - temp_f2)/cala_temp_f) + temp_f2/100;	
	adc_value_temp = adc_value_temp + (temp_f3/100);
	adc_value_temp = adc_value_temp * (temp_f4/100);
	adc_value_temp = adc_value_temp * 100;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC14_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();
	
	///////////////channal -7 and -15///////////////////////////	
	/*if(adc_type_temp1 == 3) //采集电阻
	{
		BSP_AI_Select_Mode(7);
	}*/
	BSP_AI_SelectChannel(7);//温度传感器3*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_11);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[0] + cal_offset[0];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[0] + cal_offset[0];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_7[i] =  adc_value_filter_stack_7[i+1];
	}
	adc_value_filter_stack_7[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_7[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC7_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[THI_WATERTEMTRANS_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[THI_WATERTEMTRANS_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[THI_WATERTEMTRANS_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[THI_WATERTEMTRANS_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f); //采集电压差V
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 400; //采集设备范围(1-5V)*物理量缩放倍数(100)
	adc_value_temp = adc_value_temp * ((temp_f1	- temp_f2)/cala_temp_f) + temp_f2/100;	
	adc_value_temp = adc_value_temp + (temp_f3/100);
	adc_value_temp = adc_value_temp * (temp_f4/100);
	adc_value_temp = adc_value_temp * 100;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC7_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();

	//BSP_AI_SelectChannel(15);//压差传感器*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_12);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[1] + cal_offset[1];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[1] + cal_offset[1];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_15[i] =  adc_value_filter_stack_15[i+1];
	}
	adc_value_filter_stack_15[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_15[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC15_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[DIF_PRESSURE_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[DIF_PRESSURE_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[DIF_PRESSURE_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[DIF_PRESSURE_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f); //采集电压差V
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 4000; //采集设备范围(1-5V)*物理量缩放倍数(1000)
	adc_value_temp = adc_value_temp * ((temp_f1 - temp_f2)/cala_temp_f) + temp_f2/1000;	
	adc_value_temp = adc_value_temp + (temp_f3/1000);
	adc_value_temp = adc_value_temp * (temp_f4/1000);
	adc_value_temp = adc_value_temp * 1000;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC15_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();
	
	///////////////channal -8 and -16///////////////////////////	
	/*if(adc_type_temp1 == 3) //采集电阻
	{
		BSP_AI_Select_Mode(8);
	}*/
	BSP_AI_SelectChannel(8);//温度传感器4*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_11);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[0] + cal_offset[0];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[0] + cal_offset[0];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_8[i] =  adc_value_filter_stack_8[i+1];
	}
	adc_value_filter_stack_8[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_8[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC8_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[FOU_WATERTEMTRANS_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[FOU_WATERTEMTRANS_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[FOU_WATERTEMTRANS_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[FOU_WATERTEMTRANS_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f); //采集电压差V
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 400; //采集设备范围(1-5V)*物理量缩放倍数(100)
	adc_value_temp = adc_value_temp * ((temp_f1	- temp_f2)/cala_temp_f) + temp_f2/100;	
	adc_value_temp = adc_value_temp + (temp_f3/100);
	adc_value_temp = adc_value_temp * (temp_f4/100);
	adc_value_temp = adc_value_temp * 100;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC8_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();

	//BSP_AI_SelectChannel(16);//外温传感器*********************************
	OSTimeDly(1);
	adc_original_value_temp = Get_ADC1(ADC_Channel_12);
	//adc_original_value_temp = adc_original_value_temp*cal_coef[1] + cal_offset[1];
	adc_value_temp = (float)adc_original_value_temp * adc_ad2mv_f+zero_off;//原始值转化为mv
	adc_value_temp = adc_value_temp*cal_coef[1] + cal_offset[1];
	
	for(i=0; i<(Filter_num-1); i++)
	{
		adc_value_filter_stack_16[i] =  adc_value_filter_stack_16[i+1];
	}
	adc_value_filter_stack_16[(Filter_num-1)] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<Filter_num; i++)
	{
		adc_value_temp = adc_value_temp + adc_value_filter_stack_16[i];
	}
	adc_value_temp = adc_value_temp / Filter_num;
	//原始电压值//
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC16_ORIGIN_BASE] = (s16)adc_value_temp;
	temp_f1 = (float)ALL_DATA_RAM[OUT_TEMTRANS_RANG_MAX_BASE];
	temp_f2 = (float)ALL_DATA_RAM[OUT_TEMTRANS_RANG_MIN_BASE];
	temp_f3 = (float)ALL_DATA_RAM[OUT_TEMTRANS_ADJUST_BASE];
	temp_f4 = (float)ALL_DATA_RAM[OUT_TEMTRANS_GAIN_BASE];
	OS_EXIT_CRITICAL();
	//根据配置参数进行计算//
	adc_value_temp = (adc_value_temp/1000 - adc_4ma_zero_f); //采集电压差V
	if (adc_value_temp < 0){adc_value_temp = 0;}
	cala_temp_f = 400; //采集设备范围(1-5V)*物理量缩放倍数(100)
	adc_value_temp = adc_value_temp * ((temp_f1 - temp_f2)/cala_temp_f) + temp_f2/100;	
	adc_value_temp = adc_value_temp + (temp_f3/100);
	adc_value_temp = adc_value_temp * (temp_f4/100);
	adc_value_temp = adc_value_temp * 100;
	adc_value_temp_s = (s16)adc_value_temp;
	////////////////////////
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[ADC16_FINAL_BASE] = adc_value_temp_s;
	OS_EXIT_CRITICAL();	
	
	/*
	///////////////AO FEEDBACK-1///////////////////////////	
	OSTimeDly(1);
	
	adc_original_value_temp = Get_ADC3(ADC_Channel_4);
	adc_value_temp = (float)adc_original_value_temp * 2.5 * 2 / 4.096;
	
	for(i=0; i<4; i++)
	{
		ao_feedback_stack_1[i] =  ao_feedback_stack_1[i+1];
	}
	ao_feedback_stack_1[4] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<5; i++)
	{
		adc_value_temp = adc_value_temp + ao_feedback_stack_1[i];
	}
	adc_value_temp = adc_value_temp / 5;
	
	adc_original_value_temp = (u16)adc_value_temp;
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[AO_FEEDBACK1_BASE] = adc_original_value_temp;
	OS_EXIT_CRITICAL();
	//
	OSTimeDly(2);
	adc_original_value_temp = Get_ADC3(ADC_Channel_5);
	adc_value_temp = (float)adc_original_value_temp * 2.5 * 2 / 4.096;
	
	for(i=0; i<4; i++)
	{
		ao_feedback_stack_2[i] =  ao_feedback_stack_2[i+1];
	}
	ao_feedback_stack_2[4] = adc_value_temp;
	
	adc_value_temp = 0;
	for(i=0; i<5; i++)
	{
		adc_value_temp = adc_value_temp + ao_feedback_stack_2[i];
	}
	adc_value_temp = adc_value_temp / 5;
	
	adc_original_value_temp = (u16)adc_value_temp;
	OS_ENTER_CRITICAL();
	ALL_DATA_RAM[AO_FEEDBACK2_BASE] = adc_original_value_temp;
	OS_EXIT_CRITICAL();
	*/
}	

float to_float(u16 org_hi, u16 org_lo)
{
	u16 temp[2];
	float ret;
	float *p_ret;

	temp[0] = org_lo;
	temp[1] = org_hi;

	p_ret = (float *)(&(temp[0]));
	ret = *p_ret;
	
	return ret;
}


void P_Function_AO(void)
{
	OS_CPU_SR  cpu_sr;
	static s16 Function_AO_Temp[4];
	static s16 Function_AO_Type_Temp[4];
	static s16 Function_AO_Final_Temp[4];
	float cala_temp = 0;
	static float ao_k 	= 0.0658;
	static float ao_alf = 26.962;
//	static float ao_f2v = 2;   //频率(单位0.01Hz)转电压(单位mV)
//	                           //频率范围【0 5000】0.01Hz对应电压范围【0 10000】
// lc 101617 set ao_f2v=1, thus AO = real voltage in mV (range is 0-10000)
	static float ao_f2v = 1;
	s16 cal_coef_hi[4], cal_coef_lo[4], cal_offset_hi[4], cal_offset_lo[4], cal_start_th[4];
	float cal_coef, cal_offset;
	//u16 *p, *p_des;
	
	OS_ENTER_CRITICAL();
	Function_AO_Type_Temp[0] = ALL_DATA_RAM[OUT1_TYPE_BASE];
	Function_AO_Type_Temp[1] = ALL_DATA_RAM[OUT2_TYPE_BASE];
	Function_AO_Type_Temp[2] = ALL_DATA_RAM[OUT3_TYPE_BASE];
	Function_AO_Type_Temp[3] = ALL_DATA_RAM[OUT4_TYPE_BASE];

	Function_AO_Final_Temp[0] = ALL_DATA_RAM[DAC1_FINAL_BASE];
	Function_AO_Final_Temp[1] = ALL_DATA_RAM[DAC2_FINAL_BASE];
	Function_AO_Final_Temp[2] = ALL_DATA_RAM[DAC3_FINAL_BASE];
	Function_AO_Final_Temp[3] = ALL_DATA_RAM[DAC4_FINAL_BASE];
	
	cal_start_th[0] = ALL_DATA_RAM[CAL_START_TH_AO1];
	cal_coef_hi[0] = ALL_DATA_RAM[COEF_HI_AO1];
	cal_coef_lo[0] = ALL_DATA_RAM[COEF_LO_AO1];
	cal_offset_hi[0] = ALL_DATA_RAM[OFFSET_HI_AO1];
	cal_offset_lo[0] = ALL_DATA_RAM[OFFSET_LO_AO1];

	cal_start_th[1] = ALL_DATA_RAM[CAL_START_TH_AO2];
	cal_coef_hi[1] = ALL_DATA_RAM[COEF_HI_AO2];
	cal_coef_lo[1] = ALL_DATA_RAM[COEF_LO_AO2];
	cal_offset_hi[1] = ALL_DATA_RAM[OFFSET_HI_AO2];
	cal_offset_lo[1] = ALL_DATA_RAM[OFFSET_LO_AO2];
	
	cal_start_th[2] = ALL_DATA_RAM[CAL_START_TH_AO3];
	cal_coef_hi[2] = ALL_DATA_RAM[COEF_HI_AO3];
	cal_coef_lo[2] = ALL_DATA_RAM[COEF_LO_AO3];
	cal_offset_hi[2] = ALL_DATA_RAM[OFFSET_HI_AO3];
	cal_offset_lo[2] = ALL_DATA_RAM[OFFSET_LO_AO3];

	cal_start_th[3] = ALL_DATA_RAM[CAL_START_TH_AO4];
	cal_coef_hi[3] = ALL_DATA_RAM[COEF_HI_AO4];
	cal_coef_lo[3] = ALL_DATA_RAM[COEF_LO_AO4];
	cal_offset_hi[3] = ALL_DATA_RAM[OFFSET_HI_AO4];
	cal_offset_lo[3] = ALL_DATA_RAM[OFFSET_LO_AO4];

OS_EXIT_CRITICAL();

#if 1
    cala_temp = (float)Function_AO_Final_Temp[0]*ao_f2v; //频率转化为mv

		if (cala_temp > cal_start_th[0])
			{
				cal_coef = to_float(cal_coef_hi[0], cal_coef_lo[0]);
				cal_offset = to_float(cal_offset_hi[0], cal_offset_lo[0]);


				// verify if float data is transformed properly
				/*p = (u16 *)&cal_coef;
				p_des = (u16 *)&ALL_DATA_RAM[TEST_COEF_HI_AO1];

				*p_des = *p;
				*(p_des+1) = *(p+1);

				p = (u16 *)&cal_offset;
				p_des = (u16 *)&ALL_DATA_RAM[TEST_OFFSET_HI_AO1];

				*p_des = *p;
				*(p_des+1) = *(p+1);*/

				cala_temp = (cala_temp  - cal_offset) / cal_coef;
			}

    ALL_DATA_RAM[DAC1_ORIGIN_BASE] = (s16)cala_temp;
			if (cala_temp > 100) //lc 11/20/17
    cala_temp = cala_temp * ao_k + ao_alf; //mv转化为PWM值
		Function_AO_Temp[0] = (s16)cala_temp;
	
	cala_temp = (float)Function_AO_Final_Temp[1]*ao_f2v; //频率转化为mv
		if (cala_temp > cal_start_th[1])
			{
				cal_coef = to_float(cal_coef_hi[1], cal_coef_lo[1]);
				cal_offset = to_float(cal_offset_hi[1], cal_offset_lo[1]);
				cala_temp = (cala_temp  - cal_offset) / cal_coef;
			}
	ALL_DATA_RAM[DAC2_ORIGIN_BASE] = (s16)cala_temp;
	cala_temp = cala_temp * ao_k + ao_alf; //mv转化为PWM值
    Function_AO_Temp[1] = (s16)cala_temp;
	
	cala_temp = (float)Function_AO_Final_Temp[2]*ao_f2v; //频率转化为mv
		if (cala_temp > cal_start_th[2])
			{
				cal_coef = to_float(cal_coef_hi[2], cal_coef_lo[2]);
				cal_offset = to_float(cal_offset_hi[2], cal_offset_lo[2]);
				cala_temp = (cala_temp  - cal_offset) / cal_coef;
			}
	ALL_DATA_RAM[DAC3_ORIGIN_BASE] = (s16)cala_temp;
	cala_temp = cala_temp * ao_k + ao_alf; //mv转化为PWM值
    Function_AO_Temp[2] = (s16)cala_temp;
	
	cala_temp = (float)Function_AO_Final_Temp[3]*ao_f2v; //频率转化为mv
		if (cala_temp > cal_start_th[3])
			{
				cal_coef = to_float(cal_coef_hi[3], cal_coef_lo[3]);
				cal_offset = to_float(cal_offset_hi[3], cal_offset_lo[3]);
				cala_temp = (cala_temp  - cal_offset) / cal_coef;
			}
	ALL_DATA_RAM[DAC4_ORIGIN_BASE] = (s16)cala_temp;
	cala_temp = cala_temp * ao_k + ao_alf; //mv转化为PWM值
    Function_AO_Temp[3] = (s16)cala_temp;
#endif

	BSP_AO_Select_Mode(1,Function_AO_Type_Temp[0]);
	BSP_AO_Select_Mode(2,Function_AO_Type_Temp[1]);
	BSP_AO_Select_Mode(3,Function_AO_Type_Temp[2]);
	BSP_AO_Select_Mode(4,Function_AO_Type_Temp[3]);
	BSP_PWM_DutyCycle(1,Function_AO_Temp[0]);
	BSP_PWM_DutyCycle(2,Function_AO_Temp[1]);
	BSP_PWM_DutyCycle(3,Function_AO_Temp[2]);
	BSP_PWM_DutyCycle(4,Function_AO_Temp[3]);	
}

/*
void P_Function_AO(void)
{
	OS_CPU_SR  cpu_sr;
	static u16 Function_AO_Temp[4];
	static u16 Function_AO_Type_temp[4];
	static u16 Function_AO_Final_Temp[4];
	static float ao_k 	= 0.0658;
	static float ao_alf = 26.962;
	static float ao_v2f = 200;

#if 0
	ALL_DATA_RAM[DAC1_ORIGIN_BASE] = ALL_DATA_RAM[DAC1_FINAL_BASE]*ao_v2f;
#endif
	
	OS_ENTER_CRITICAL();
	Function_AO_Temp[0] 		 = (u16)((float)ALL_DATA_RAM[DAC1_ORIGIN_BASE]*ao_k + ao_alf);
	Function_AO_Temp[1] 		 = (u16)((float)ALL_DATA_RAM[DAC2_ORIGIN_BASE]*ao_k + ao_alf);
	Function_AO_Temp[2] 		 = (u16)((float)ALL_DATA_RAM[DAC3_ORIGIN_BASE]*ao_k + ao_alf);
	Function_AO_Temp[3] 		 = (u16)((float)ALL_DATA_RAM[DAC4_ORIGIN_BASE]*ao_k + ao_alf);
	Function_AO_Type_temp[0] = ALL_DATA_RAM[OUT1_TYPE_BASE];
	Function_AO_Type_temp[1] = ALL_DATA_RAM[OUT2_TYPE_BASE];
	Function_AO_Type_temp[2] = ALL_DATA_RAM[OUT3_TYPE_BASE];
	Function_AO_Type_temp[3] = ALL_DATA_RAM[OUT4_TYPE_BASE];
	memcpy(&ALL_DATA_RAM[DAC1_ORIGIN_READ_BASE],&ALL_DATA_RAM[DAC1_ORIGIN_BASE],(4*sizeof(ALL_DATA_RAM[DAC1_ORIGIN_BASE])));
	OS_EXIT_CRITICAL();
	
	BSP_PWM_DutyCycle(1,Function_AO_Temp[0]);
	BSP_PWM_DutyCycle(2,Function_AO_Temp[1]);
	BSP_PWM_DutyCycle(3,Function_AO_Temp[2]);
	BSP_PWM_DutyCycle(4,Function_AO_Temp[3]);
	BSP_AO_Select_Mode(1,Function_AO_Type_temp[0]);
	BSP_AO_Select_Mode(2,Function_AO_Type_temp[1]);
	BSP_AO_Select_Mode(3,Function_AO_Type_temp[2]);
	BSP_AO_Select_Mode(4,Function_AO_Type_temp[3]);
}
*/

void P_Function_DO()
{
	OS_CPU_SR  cpu_sr;
	u8 i;
	s16 Function_DO_Temp[16];
	u8 store_flag;
	
	OS_ENTER_CRITICAL();
	memcpy(&Function_DO_Temp[0],&ALL_DATA_RAM[DO1_DATA_BASE],(16*sizeof(ALL_DATA_RAM[DO1_DATA_BASE])));
	//memcpy(&ALL_DATA_RAM[DO1_DATA_READ_BASE],&ALL_DATA_RAM[DO1_DATA_BASE],(16*sizeof(ALL_DATA_RAM[DO1_DATA_BASE])));
	OS_EXIT_CRITICAL();
	
	store_flag = 0;
	for(i=0; i<16; i++)
	{
		if(Function_DO_Temp[i] != DO_realtime[i])
		{
			DO_realtime[i] = Function_DO_Temp[i];
			store_flag = 1;
//			DO_store();
		}
	}
	
	if (store_flag)
		DO_store();
//	
	store_flag = 0;
//	for(i=8; i<16; i++)
//	{
//		if(Function_DO_Temp[i] != DO_realtime[i])
//		{
//			DO_realtime[i] = Function_DO_Temp[i];
//			store_flag = 1;
//		}
//	}
////	
//	if (store_flag)
//		DO_store_hi();
//  store_flag = 0;

	for(i=0; i<16; i++)
	{
		if(Function_DO_Temp[i] == 0U)
		{
			DO_ResetBits(i+1);
		}
		else
		{
			DO_SetBits(i+1);
		}
	}	
}

void P_Function_DI()
{
	OS_CPU_SR  cpu_sr;
	u8 i;
	static u16 Function_DI_Temp[16];
	
	for(i=0; i<16; i++)
	{
		Function_DI_Temp[i] = !DI_ReadBits(i+1);
	}	
	OS_ENTER_CRITICAL();
	memcpy(&ALL_DATA_RAM[DI1_DATA_BASE],&Function_DI_Temp[0],(16*sizeof(ALL_DATA_RAM[DI1_DATA_BASE])));
	OS_EXIT_CRITICAL();
}

void P_Function_RunStatus(u16 blinktimeMS)
{
	Sys_Run_LED(0);
	OSTimeDlyHMSM(0,0,0,blinktimeMS);
	Sys_Run_LED(1);
	OSTimeDlyHMSM(0,0,0,blinktimeMS);
	TimeBug ++;
	if(TimeBug > 31104000)
	{
		while(1);
	}
}

//void DO_reload()
//{
//	unsigned char do_lo, do_hi;
//	unsigned char i;
//	
//	
//	do_lo = EEPROM_Read_Byte(DO_ADDR_LO);
//	do_hi = EEPROM_Read_Byte(DO_ADDR_HI);
//	
//	for(i=0; i<8; i++)
//		DO_realtime[i] =  (do_lo & (1 << i)) ? 1:0;

//	for(i=8; i<16; i++)
//		DO_realtime[i] =  (do_hi & (1 << (i-8))) ? 1:0;
//	
//	for(i=0; i<16; i++)
//	{
//			ALL_DATA_RAM[DO1_DATA_BASE + i] =  DO_realtime[i];
//	}
////	ALL_DATA_RAM[DO1_DATA_BASE] = do_lo;
////	ALL_DATA_RAM[DO1_DATA_BASE+15] = do_hi;
//}

//void DO_store_lo()
//{
//	unsigned char do_lo_data;
//	unsigned char i;
//	
//	do_lo_data = 0;
//	for(i=0; i<8; i++)
//	{
//		do_lo_data |= DO_realtime[i];
//		do_lo_data << 1;
//	}
//	EEPROM_Write_Byte(DO_ADDR_LO, do_lo_data);
//}

//void DO_store_hi()
//{
//	unsigned char do_hi_data;
//	unsigned char i;
//	
//	do_hi_data = 0;
//	for(i=8; i<16; i++)
//	  {
//		do_hi_data |= DO_realtime[i];
//	  do_hi_data << 1;
//		}
//	EEPROM_Write_Byte(DO_ADDR_HI, do_hi_data);
//}
void DO_reload()
{
  unsigned char i;
  unsigned char do_lo, do_hi;
	for(i=0; i<16; i++)
	{
	 DO_realtime[i] = EEPROM_Read_Byte(DO_ADDR_LO + i);
	 DO_realtime[i] = DO_realtime[i]%255;
	}
	for(i=0; i<16; i++)
	{
			ALL_DATA_RAM[DO1_DATA_BASE + i] =  DO_realtime[i];
	}
}
void DO_store()
{
	unsigned char i;
 	for(i=0; i<16; i++)
	{
	EEPROM_Write_Byte(DO_ADDR_LO + i, DO_realtime[i]);
	}

}