//PID algrithm refer to
//http://blog.csdn.net/jianfengxia/article/details/44750807

#include<TPK8CONTROL.h>
#include<control_global.h>
#include "comm_handle.h"
#include "ucos_ii.h"
#include "control_comm.h"
#include "control_global.h"

//extern u16 DEBUG_ERROR;
typedef struct PID_tag{
u16 SetPoint; 
float Proportion;
float Integral;
float Derivative;
int LastError; // Error[-1]
int PrevError; // Error[-2]
int SumError; // Sums of Errors
}PID;

# define CHANNEL_TOTAL 2
PID pid[CHANNEL_TOTAL];
float k[CHANNEL_TOTAL], b[CHANNEL_TOTAL];

void PID_control_parameter_init_channel(u8 channelx)
{
//	pid[channelx].LastError = 560;
//	pid[channelx].PrevError = 5;
//	pid[channelx].SumError = 5;
	pid[channelx].LastError = 0;
	pid[channelx].PrevError = 0;
	pid[channelx].SumError = 0;
}
	

void PID_control_parameter_init()
{
	//default setting
	//PID cycle: 1s
	//PID control: ch1-disable, ch2-disable
	ALL_DATA_RAM[PID_CONTROL_CYCLE] = 2;
	ALL_DATA_RAM[PID_CONTROL_ENABLE_AI1] = 0;
	ALL_DATA_RAM[PID_CONTROL_ENABLE_AI2] = 0;
	
	ALL_DATA_RAM[PID_SAMPLING_VALUE_SELECTION_AI1] = 0; //from remote
	ALL_DATA_RAM[PID_SAMPLING_VALUE_SELECTION_AI2] = 0; //from remote

	PID_control_parameter_init_channel(1);
	PID_control_parameter_init_channel(2);
}

#if 0
u16 PIDCalc( PID *pp, u16 NextPoint)
{
	int dError,Error;
	int result;
	int result_p, result_i, result_d;
	//u16 *pdesc, *psrc;

	Error = (int)(pp->SetPoint) - NextPoint; //设定值减去当前值
 //ALL_DATA_RAM[PID_DEBUG_ERROR] = NextPoint;
	
  //lc 10192017 to avoid sumError overflow
	if (pp->SumError > 0 && Error > 0)
	{
		if ( (65535 - pp->SumError) > Error )
			pp->SumError += Error;
		else
			pp->SumError = 65535;
	}
	else if (pp->SumError < 0 && Error < 0)
	{
		if ( ((65535 + pp->SumError) - Error) > 0 )
			pp->SumError += Error;
		else
			pp->SumError = -65536;
	}
	else
		pp->SumError += Error;
	
	dError = pp->LastError - pp->PrevError; // 当前微分，偏差相减
	pp->PrevError = pp->LastError;
	pp->LastError = Error;
	
	result_p = pp->Proportion * Error;

	result_i = pp->Integral * pp->SumError;

	result_d = pp->Derivative * dError;

	/*psrc = (u16*)&result_p;
	pdesc = (u16*)(&ALL_DATA_RAM[PID_DEBUG_1]);
	*pdesc = *psrc;
	*(pdesc+1) = *(psrc+1);

	psrc = (u16*)&result_i;
	pdesc = (u16*)(&ALL_DATA_RAM[PID_DEBUG_3]);
	*pdesc = *psrc;
	*(pdesc+1) = *(psrc+1);
	
	psrc = (u16*)&result_d;
	pdesc = (u16*)(&ALL_DATA_RAM[PID_DEBUG_5]);
	*pdesc = *psrc;
	*(pdesc+1) = *(psrc+1);*/

	result = result_p + result_i + result_d;
	if(result > 10000)
	    result = 10000;
	else if (result < 0)
		result = 0;
	
	return (u16)result;
}
#else
u16 PIDCalc( PID *pp, u16 NextPoint)
{
	int dError,Error;
	int result;
	int result_p, result_i, result_d;

	Error = (int)(pp->SetPoint) - NextPoint; //设定值减去当前值
	
	dError = Error - 2*pp->LastError + pp->PrevError; // 当前微分，偏差相减

	result_p = pp->Proportion * (Error - pp->LastError);

	result_i = pp->Integral * Error;

	result_d = pp->Derivative * dError;

	pp->PrevError = pp->LastError;
	pp->LastError = Error;
	
	result = NextPoint + result_p + result_i + result_d;//实际值加上增量
	if(result > 10000)
	    result = 10000;
	else if (result < 0)
		result = 0;
	
	return (u16)result;
}
#endif

float cov_s16_to_float(s16 data_hi, s16 data_lo)
{
	s16 temp[2];
	float *p_data_float;
  
	temp[0] = data_lo;
	temp[1] = data_hi;

	p_data_float = (float *)(&(temp[0]));

	return (*p_data_float);
}

void cal_k_b(float x1, float y1, float x2, float y2, float *k, float *b)
{
	//除数不能为0
	if (x2 == x1)
	{
		*k = 0;
		*b = 0;
	}
	else
	{
		*k = (y2 - y1) / (x2 - x1);
		*b = y1 - (*k * x1);
	}
}

//cal k,b, which are used to map temperature/pressure to voltage
void cal_k_b_channel(u8 channelx)
{
  float range_min, range_max;
  float *p_range_min, *p_range_max;
	
	s16 temp_min[2], temp_max[2];

	if (channelx == 1)
	{
		temp_min[0] = ALL_DATA_RAM[PID_RANGE_MIN_LO_AI1];
		temp_min[1] = ALL_DATA_RAM[PID_RANGE_MIN_HI_AI1];
		temp_max[0] = ALL_DATA_RAM[PID_RANGE_MAX_LO_AI1];
		temp_max[1] = ALL_DATA_RAM[PID_RANGE_MAX_HI_AI1];
	}
	else
	{
		temp_min[0] = ALL_DATA_RAM[PID_RANGE_MIN_LO_AI2];
		temp_min[1] = ALL_DATA_RAM[PID_RANGE_MIN_HI_AI2];
		temp_max[0] = ALL_DATA_RAM[PID_RANGE_MAX_LO_AI2];
		temp_max[1] = ALL_DATA_RAM[PID_RANGE_MAX_HI_AI2];
	}
	
	p_range_min = (float *)(&(temp_min[0]));
	range_min = *p_range_min;
	p_range_max = (float *)(&(temp_max[0]));
	range_max = *p_range_max;
	
	cal_k_b(range_min, 0.0, range_max, 10000.0, &(k[channelx]), &(b[channelx]));
	
	/*pk = (u16*)(&k[channelx]);
	pb = (u16*)(&b[channelx]);

  ALL_DATA_RAM[PID_RANGE_MIN_HI_AI2] = *(pk+1);
  ALL_DATA_RAM[PID_RANGE_MIN_LO_AI2] = *(pk);
  ALL_DATA_RAM[PID_RANGE_MAX_HI_AI2] = *(pb+1);
  ALL_DATA_RAM[PID_RANGE_MAX_LO_AI2] = *(pb);*/
}

// read value from AI
u16 get_value_current(u8 channelx)
{
	u16 current_value;

	if (channelx == 1)
	{
		if(ALL_DATA_RAM[PID_SAMPLING_VALUE_SELECTION_AI1])
			current_value = ALL_DATA_RAM[ADC1_ORIGIN_BASE + channelx - 1]; //from local
		else
			current_value = ALL_DATA_RAM[PID_SAMPLING_VALUE_REMOTE_AI1]; //from remote
	}
	else
	{
		if(ALL_DATA_RAM[PID_SAMPLING_VALUE_SELECTION_AI2])
			current_value = ALL_DATA_RAM[ADC1_ORIGIN_BASE + channelx - 1];
		else
			current_value = ALL_DATA_RAM[PID_SAMPLING_VALUE_REMOTE_AI2];
	}

	return current_value;
}

// read value from AI
u16 get_value_target(u8 channelx)
{
  float target_value;
	float *p_target_value;
	s16 temp[2];
	u16 target_value_voltage;
	
	if (channelx == 1)
	{
		temp[0] = ALL_DATA_RAM[PID_TARGET_LO_AI1];
		temp[1] = ALL_DATA_RAM[PID_TARGET_HI_AI1];
	}
	else
	{
		temp[0] = ALL_DATA_RAM[PID_TARGET_LO_AI2];
		temp[1] = ALL_DATA_RAM[PID_TARGET_HI_AI2];
	}

	p_target_value = (float *)(&(temp[0]));
	target_value = *p_target_value;
	
  //transform to voltage in mV
	target_value_voltage = (u16)(target_value * k[channelx] + b[channelx]);

  //ALL_DATA_RAM[PID_RANGE_MIN_HI_AI2] = target_value_voltage;
	
	return target_value_voltage;
}

//set PID output to register
void PID_value_out(u8 channelx, u16 cal_value)
{
	if (channelx == 1)
	{
		ALL_DATA_RAM[PID_CAL_AI1] = cal_value;
	}
	else
	{
		ALL_DATA_RAM[PID_CAL_AI2] = cal_value;
	}	
}

void get_PID_parameter(u8 channelx, PID *p)
{
	
  //range mapping
  cal_k_b_channel(channelx);
	
  if (channelx == 1)
	{
		p -> Proportion = cov_s16_to_float(ALL_DATA_RAM[PID_PROPORTION_HI_AI1], ALL_DATA_RAM[PID_PROPORTION_LO_AI1]);
		p -> Integral = cov_s16_to_float(ALL_DATA_RAM[PID_INTERGRAL_HI_AI1], ALL_DATA_RAM[PID_INTERGRAL_LO_AI1]);
		p -> Derivative = cov_s16_to_float(ALL_DATA_RAM[PID_DERIVATIVE_HI_AI1], ALL_DATA_RAM[PID_DERIVATIVE_LO_AI1]);
		p -> SetPoint = get_value_target(channelx);
//		p -> Proportion = 560;
//		p -> Integral = 5;
//		p -> Derivative = 5;
//		p -> SetPoint = 100;
	}
	else 
	{
		p -> Proportion = cov_s16_to_float(ALL_DATA_RAM[PID_PROPORTION_HI_AI2], ALL_DATA_RAM[PID_PROPORTION_LO_AI2]);
		p -> Integral = cov_s16_to_float(ALL_DATA_RAM[PID_INTERGRAL_HI_AI2], ALL_DATA_RAM[PID_INTERGRAL_LO_AI2]);
		p -> Derivative = cov_s16_to_float(ALL_DATA_RAM[PID_DERIVATIVE_HI_AI2], ALL_DATA_RAM[PID_DERIVATIVE_LO_AI2]);
		p -> SetPoint = get_value_target(channelx);
//				p -> Proportion = 560;
//		p -> Integral = 5;
//		p -> Derivative = 5;
//		p -> SetPoint = 100;
	}
}	


void pid_control(u8 channelx)
{
  u16 pid_cal_value;
	u16 current_samping_value = get_value_current(channelx);
  OS_CPU_SR  cpu_sr = 0;
	//get PID parameter on the fly in case of changing by host
	OS_ENTER_CRITICAL();
	get_PID_parameter(channelx, &pid[channelx]);
	OS_EXIT_CRITICAL();

	pid_cal_value = PIDCalc(&(pid[channelx]), current_samping_value);
	
  //write to registers
	PID_value_out(channelx, pid_cal_value);
}

void pid_control_top()
{
	if (ALL_DATA_RAM[PID_CONTROL_ENABLE_AI1])
	{
		pid_control(1);
	}
	
	if (ALL_DATA_RAM[PID_CONTROL_ENABLE_AI2])
	{
		pid_control(2);
	}
}
