//TPK8CONTROL.c	******************在control_main.c
//****************  分布式供暖系统热力站优化节能控制V2.0  ************
//&&&&&&&&&&&&&&&&&&&&&&&   TPK8CONTROL,V2.0  &&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&    BAY YTQ,2015.1   &&&&&&&&&&&&&&&&&&&&&&&&
//只在文件中增加了"TPK8CONTROL.H"和"TPK8CONTROL.C"  ******************
//用"YY" 找到需要共同确定的地方！*************************************
//#include <math.h>
#include<control_pid.h>
#include<control_global.h>

	#define k1 100			   //yy小数化整
	#define k2 1000			   //yy小数化整
	static int init_flag;
	int auto_flagz,auto_flagl,bpqerr_flagz,bpqerr_flagl;
	//&&&&&&&&&&&&&&&&  必须由采样得到的状态   &&&&&&&&&&&&&&&&
	//int				//根据编译器定义的输入INT,也可以用CHR或者位变量 	
	//		auto_flagZ,		//质调,=0为自动
	//		auto_flagL,		//量调,=0为自动
	//		bpqerr_flagZ,	//质调节变频器故障位=1为故障
	//		bpqerr_flagL;	//量调节变频器故障位=1为故障
	static int out_plz,out_pll;

//&&&&&&&&&&&&&&&&  必须由采样,通讯得到的数值   &&&&&&&&
float	 
		TW,		//实际外温,TWFROM=3--TW上位机通讯,否则本机采样
		PLL,	//量调节水泵实际频率  (HZ)
		PLZ,	//质调节水泵实际频率  (HZ)
        CY, 	//量调节[差]压	(MPa)[LLKZGK=1，3，4时必须!!!!!!!!!!!]
		TG,		//实际供水温度
		TH,		//实际回水温度
 	    DDXS;	//质调调度系数℃,由上位机通讯命令,对设定供水温度+-:
				//SetPointZ=SetPointZ+DDXS,从而实现系统的统一调度!

//&&&&&&&&&&&&&&&&  必须由参数设定表得到的数值   &&&&&&&&&&&&&&&
int 
		LLKZGK,		//流量控制工况
		WDKZGK;		//温度控制工况
float	
		DSJTN,		//冬设计室内温度℃
		DSJTW,		//冬季设计外温  ℃
		DQTTW,		//冬起调室外温度℃
		DSJTG,		//冬设计供水温度℃
		DSJTH,		//冬设计回水温度℃
		DSJCYL,		//冬设计[差]压Mpa(定压控制用,报警用)
		RDXJZ,		//热惰性修正系数
		PLMAXL,		//量调节设计工况频率HZ(<=50,对应冬设计室外温度)
		PLMINL,		//量调节最低频率HZ(对应冬起调室外温度)
		PLMINZ,		//质调节水泵最低频率  (HZ),>=0		
		Proportion,  //比例常数 Proportional Const
		ProportionZ, //质调比例常数 Proportional Const
		ProportionL, //量调比例常数 Proportional Const
		Integral,    //积分常数 Integral Const
		IntegralZ,   //质调积分常数 Integral Const
		IntegralL;   //量调积分常数 Integral Const

//&&&&&&&&&&&&&&&&  由本程序确定的静态变量的数值   &&&&&&&&&&&&&&&&
float	static
   		SumErrorZ,   //质调PI Sums of Errors,增量PreEror
   		SumErrorL,   //质调PI Sums of Errors,增量PreEror
		out0Z,		//质调节用
		out0L;		//量调节用
float
		OUT_PLL,	//量调节输出频率(Hz)!!!!!!!!!!!!!!!!!!!!
	    OUT_PLZ,	//质调节输出频率(Hz)!!!!!!!!!!!!!!!!!!!!!
		TWp,		//计算外温度
		OUT,
		Error,
		SumError,
		out0,
		SetPoint ,   //设定目标,
		SetPointZ,   //质调设定目标--设定供水温度℃,
		SetPointL,   //量调设定目标--设定[差]压MPa,
		Set_LL;		//LLKZGK=2,量调节设定相对流量%,可供显示

void TPK8control_init()
{
//	OS_CPU_SR  cpu_sr;
	//tpk8添加初始化
		TW=(float)ALL_DATA_RAM[ADC1_FINAL_BASE]/k1;//实际外温,TWFROM=3--TW上位机通讯,否则本机采样
		//TW=TW/k1;
		PLL=(float)ALL_DATA_RAM[ADC11_FINAL_BASE]/k1;	//adc11/12//量调节频率(Hz)
		//PLL=PLL/k1;
		PLZ=(float)ALL_DATA_RAM[ADC13_FINAL_BASE]/k1;	//adc13/14//质调节频率(Hz)
		TWp=TW; //(float)ALL_DATA_RAM[WINTER_OUTDOOR_TEMPERATURE_SET_BASE/k1];	//DSJTW;//计算外温度
		OUT_PLL=PLL;	//量调节输出频率(Hz)，无扰切换
	    OUT_PLZ=PLZ;	//质调节输出频率(Hz), 无扰切换
		out0Z=PLZ;		//质调节用，无扰切换
		out0L=PLL;		//量调节用，无扰切换
   		SumErrorZ=0;   	//质调PI Sums of Errors,增量PreEror
   		SumErrorL=0;    //量调PI Sums of Errors,增量PreEror
//#define	BPBONL	ALL_DATA_RAM[DO5_DATA_BASE];		//量调节变频泵,=0停止,=1启动! 
//#define	BPBONZ	ALL_DATA_RAM[DO6_DATA_BASE];		//质调节变频泵,=0停止,=1启动! 
		ALL_DATA_RAM[DO5_DATA_BASE]=0;//BPBONL,		//量调节变频泵,=0停止,=1启动!
		ALL_DATA_RAM[DO6_DATA_BASE]=0;
// 		SetPointZ=DSJPMAX   //质调设定目标--设定供水温度℃,
//		SetPointL=PLMAX     //量调设定目标--设定[差]压MPa,
}

/*
void TPK8control()
{
	OS_CPU_SR  cpu_sr;
	static float cala_1_temp = 0;
	static float cala_2_temp = 0;
	static float cala_3_temp = 0;
	static float cala_4_temp = 0;
	static float temperature_set_ref = 15;
	static float cala_p = 10;
	static float cala_alf = 3000;

	s16 bpqrun = 0;
	
	temperature_set_ref = (float)ALL_DATA_RAM[DIF_PRESSURE_ADJUST_BASE] / 100;

	bpqrun = ALL_DATA_RAM[DI1_DATA_BASE];
	if(bpqrun == 1)
	{
		ALL_DATA_RAM[DO1_DATA_BASE] = 1;
		cala_1_temp = (float)ALL_DATA_RAM[ADC5_FINAL_BASE]/100;
		cala_2_temp = temperature_set_ref - cala_1_temp;
		cala_3_temp = cala_2_temp * cala_p + cala_3_temp;
		if(cala_3_temp < 0)
		{
			cala_3_temp = 0;
		}

		ALL_DATA_RAM[DAC1_ORIGIN_BASE] = (s16)cala_3_temp;
	}else{
		ALL_DATA_RAM[DO1_DATA_BASE] = 0;
		ALL_DATA_RAM[DAC1_ORIGIN_BASE] = 0;
	}
}
*/
void TPK8control()
{
//	OS_CPU_SR  cpu_sr;
	//tpk8添加
			//根据编译器定义的输出INT,也可以用CHR或者位变量
	auto_flagz = ALL_DATA_RAM[DI9_DATA_BASE];
	auto_flagl = ALL_DATA_RAM[DI1_DATA_BASE];
	bpqerr_flagz = ALL_DATA_RAM[DI10_DATA_BASE];
	bpqerr_flagl = ALL_DATA_RAM[DI2_DATA_BASE];
	
	TW=(float)ALL_DATA_RAM[ADC1_FINAL_BASE]/k1;//实际外温,TWFROM=3--TW上位机通讯,否则本机采样
	//TW=TW/k1;
    CY=(float)ALL_DATA_RAM[ADC2_FINAL_BASE]/k2;//量调节[差]压	(MPa)[LLKZGK=1，3，4时必须
	//CY=CY/k2;
	TG=(float)ALL_DATA_RAM[ADC5_FINAL_BASE]/k1;//实际供水温度
	//TG=TG/k1;
	TH=(float)ALL_DATA_RAM[ADC6_FINAL_BASE]/k1;//实际回水温度
	//TH=TH/k1;
 	DDXS=0;	//YY质调调度系数℃,由上位机通讯命令,对设定供水温度+-://yy
	PLL=(float)ALL_DATA_RAM[ADC11_FINAL_BASE]/k1;	//adc11/12//量调节频率(Hz)
	//PLL=PLL/k1;
	PLZ=(float)ALL_DATA_RAM[ADC13_FINAL_BASE]/k1;	//adc13/14//质调节频率(Hz)
	//PLZ=PLZ/k1;
	DSJTW=(float)ALL_DATA_RAM[WINTER_OUTDOOR_TEMPERATURE_SET_BASE]/k1;	//计算外温度
	//DSJTW=DSJTW/k1;
	DSJTN=(float)ALL_DATA_RAM[WINTER_INDOOR_TEMPERATURE_SET_BASE]/k1;	//冬设计室内温度℃
	//DSJTN=DSJTN/k1;
	DSJCYL=(float)ALL_DATA_RAM[WINTER_DIF_PRESSURE_MAX_SET_BASE]/k2;	//冬设计[差]压Mpa(定压控制用,报警用)
	DSJCYL=DSJCYL/k2;
	//-(float)ALL_DATA_RAM[ WINTER_DIF_PRESSURE_MIN_SET_BASE]k2;//冬最小[差]压Mpa(报警用)	
	PLMAXL=(float)ALL_DATA_RAM[WINTER_FREQUENCY_MAX_SET_BASE]/k1;//量调节设计工况频率HZ(<=50,对应冬设计室外温度)
	//PLMAXL=PLMAXL/k1;	                
	PLMINL=(float)ALL_DATA_RAM[WINTER_FREQUENCY_MIN_SET_BASE]/k1;//量调节最低频率HZ(对应冬起调室外温度)
	//PLMINL=PLMINL/k1;	
	DQTTW=(float)ALL_DATA_RAM[WINTER_STARTPOINT_OUTDOOR_TEMPERATURE_SET_BASE]/k1;	//冬起调室外温度℃	
	//DQTTW=DQTTW/k1;
	DSJTG=(float)ALL_DATA_RAM[WINTER_WATER_SUPPLY_MAX_TEMPERATURE_SET_BASE]/k1;	//冬设计供水温度℃	
	//DSJTG=DSJTG/k1;
	DSJTH=(float)ALL_DATA_RAM[WINTER_WATER_SUPPLY_MIN_TEMPERATURE_SET_BASE]/k1;	//冬设计回水温度℃	
	//DSJTH=DSJTH/k1;
	RDXJZ=3;		//YY热惰性修正系数
	PLMINZ=5;		//YY质调节水泵最低频率  (HZ),>=0		

	ProportionZ=(float)ALL_DATA_RAM[PID_TEMPERATURE_CONTROL_PROPORTION_BASE];//质调比例常数 Proportional Const
	//ProportionZ=ProportionZ/k1;
	IntegralZ=(float)ALL_DATA_RAM[PID_TEMPERATURE_CONTROL_INTEGRAL_TIME_BASE]; //质调积分常数 Integral Const
	//IntegralZ=IntegralZ/k1;
	ProportionL=(float)ALL_DATA_RAM[PID_PRESSURE_CONTROL_PROPORTION_BASE];		//量调比例常数 Proportional Const
	//ProportionL=ProportionL/k1;
	IntegralL=(float)ALL_DATA_RAM[PID_PRESSURE_CONTROL_INTEGRAL_TIME_BASE]; //量调积分常数 Integral Const
	//IntegralL=IntegralL/k1;
	LLKZGK=0;	//yy
	WDKZGK= ALL_DATA_RAM[CONTROL_METHOD_SET_BASE];	//yy
		//TPK8control();	
	comput_TWp();				//计算外温-冬季
	if (LLKZGK==1)  autocontrol_lt1();	//量调-自动控制,用于定压系统
	if (LLKZGK==2)  autocontrol_lt2();	//量调-自动控制,用于阻力不变系统
	autocontrol_zt();					//自动控制--质调节
 }
		
void PID() 
//INPUT:Error, SumError,out0
//		Proportion,  //比例常数 Proportional Const，Hz/℃
//		Integral,    //积分常数 Integral Const
//OUTPUT:SumError ,out0
{  float A;
   if (Error>3) Error=3;			//防止饱和   30  
   if (Error<-3) Error=-3;		//防止饱和   30  
   A=Proportion * (Error-SumError);    // 比例项增量          
   SumError=Error;						//PreError=SumError = Error
   A=A  + Proportion*(Error/Integral);         	//+积分项,A=增量
   	//A=A +  Derivative * dError    // 微分项
   if (A>1) A=1;			//<+-10
   if (A<-1) A=-1;			//<+-10
	A=A+out0;
	
	if (A>PLMAXL) A=PLMAXL;
	if (A<PLMINL) A=PLMINL;
	out0=A;
}


void comput_TWp()
//TPK8-3/4&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&计算外温-冬季
/*已知:float	TW,		//实际外温
				DSJTW,	//冬季设计外温  ℃
				DQTTW,	//冬起调室外温度
				RDXJZ,	//热惰性修正系数
  确定:float	TWp,	//计算外温度; 
  初始化:		TWp=TW;
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
{
TWp=(TWp*RDXJZ+TW)/(1+RDXJZ);			//高阶滤波//计算外温
 	//如果冬计算外温TWP<设计外温DSJTW,取计算外温TWP=设计外温DSJTW.
 if (TWp<DSJTW) TWp= DSJTW; 
	//5C	2011.11.14要求冬设计室外温度>5C,=5C
 if (TWp>DQTTW) TWp=DQTTW;
}

void autocontrol_lt1()			//自动控制—定压调节
{
//float   A;
if (auto_flagl==1) {ALL_DATA_RAM[DO5_DATA_BASE]=0; OUT_PLL=PLL; SumErrorL=0;}		//无扰
if (bpqerr_flagl==1) {ALL_DATA_RAM[DO5_DATA_BASE]=0; OUT_PLL=0; }		//=0
if ((bpqerr_flagl==0) && (auto_flagl==0))		//变频器为无故障//为自动
   {ALL_DATA_RAM[DO5_DATA_BASE]=1;		//变频泵启动=1
   	Error = DSJCYL-CY;		//冬设计[差]压Mpa(定压控制用,)-CY// 冬偏差
	Proportion= ProportionL;
	SumError= SumErrorL;
	out0=out0L;
	PID();
	SumErrorL= SumError;
	out0L=out0;
	OUT_PLL= out0;	//L调节输出频率(Hz),
   }
	ALL_DATA_RAM[DAC1_FINAL_BASE] =(s16) OUT_PLL*100;	//
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&TPK3-量调-自动控制-冬季-无值班
void autocontrol_lt2()	//量调-自动控制,用于阻力不变系统,用频率计算流量,无工频泵
/*	已知:	bit 	bpqerr_flagL,		//变频器故障位=1为故障
					auto_flagL,			//=0为自动
			float	TWp,		//计算外温度
					DSJTW,		//冬季设计外温  ℃
					DQTTW,		//冬起调室外温度
					PLMAX,		//设计频率<=50Hz
					PLMIN,		//最低频率<PLMAX
	确定:   bit 	BPBONL		//变频泵启动=1/停止=0			
  		 	//bit 	GPBON		//工频泵启动=1/停止=0
			float	OUT_PL,		//量调节输出频率:PLMIM--PLMAX;
	初始化: bit		BPBON=0		//变频泵停止			
  		 	//bit 	GPBON=0		//工频泵停止
	     	float 	OUT_PL=PLL 	//量调变频泵;无扰
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
{
if ((bpqerr_flagl==0) && (auto_flagl==0)) 	//变频器为无故障//为自动
  {ALL_DATA_RAM[DO5_DATA_BASE]=1;		//变频泵启动=1
   if (PLMAXL>50)PLMAXL=50;
   OUT_PLL=PLMINL+(PLMAXL-PLMINL)*(DSJTW-TWp)/(DSJTW-DQTTW);	//量调节输出频率(Hz)
   Set_LL=OUT_PLL*100/PLMAXL;				//量调节设定相对流量%,供显示
   //OUT_PL=PLMIN+(PLMAX-PLMIN)*(DSJTW-TWp)/(DSJTW-DQTTW);	//量调节输出频率(Hz)
   //DAOUT_PL=OUT_PL*DA_DATAmax/50;	  	//量调节DA或者PWM输出
  }
else
//#define	BPBONL	ALL_DATA_RAM[DO5_DATA_BASE];		//量调节变频泵,=0停止,=1启动! 
  {ALL_DATA_RAM[DO5_DATA_BASE]=0;							//变频泵停止=0
   if (bpqerr_flagl==1) OUT_PLL=0;		//=0
  }
ALL_DATA_RAM[DAC1_FINAL_BASE] =(s16) OUT_PLL*100;	//
}


/*************************************质调节********************
void autocontrol_zt()		//自动控制--质调节
确定:   bit 	BPBONZ		//变频泵启动=1/停止=0
  		//bit 	GPBONZ		//工频泵启动=1/停止=0
		float	OUT_PLZ,	//质调节输出频率;
请输出到端口!
*****************************************************************
//=2011.12.26:调度系数DDXS(按要求增加)
//注意:只要/只能用通讯确定调度系数DDXS
 float 	PL,GW,TH,PLZ,	//量调节频率PL,供水温,回水温,频率
		DDXS,	//DDXS调度系数℃只能通讯改变,TWfrom=3,TW通讯
		TWp,	//设计外温度
		DSJTW,	//冬季设计外温  ℃
		PLMAX,	//量调节设计频率
		DQTTW,	//冬起调室外温度
		DSJTN,	//冬设计室内温度
		DSJTG,	//冬设计供水温度
		DSJTH;	//冬设计回水温度
float   SumError,    //位置Sums of Errors,增量PreEror
 	    DDXS,		//DDXS调度系数℃只能通讯改变
float  SetPoint;        //  设定目标 Desired Value
float  Proportion;         //  比例常数 Proportional Const
float  Integral;           //  积分常数 Integral Const
//float  Derivative;       //  微分常数 Derivative Const
//float  LastError;        //  Error[-1],微分
//float  PrevError;        //  Error[-2],微分
float  	PLZMIN;	//质调节水泵最低频率		
float  	SetPoint;
float  SumError;    //位置Sums of Errors,增量PreEror  
确定:   bit 	BPBONZ		//变频泵启动=1/停止=0
  		//bit 	GPBONZ		//工频泵启动=1/停止=0
		float	OUT_PLZ,	//质调节输出频率;
初始化: bit		BPBONZ=0		//变频泵停止			
  		//bit 	GPBONZ=0		//工频泵停止
	    float 	out0=PLZ,		//质调-变频泵DA0-无扰;
				SumError=0,
				DDXS=0;			//调度系数 	 
*******************************************************************/

void autocontrol_zt()			//自动控制--质调节
{
float   g,TNW,A;
if (WDKZGK ==1) SetPointZ=DSJTG;//冬设计供水温度℃,定温控制
if (WDKZGK ==2)					//变温控制
   {//void setpoint()			//设定目标Tg
	TNW=(DSJTN-TWp)/(DSJTN-DSJTW);
//	A=sqrt(TNW);
//	A=A*sqrt(A);		//A=TNW^0.75; OR A=pow(TNW,0.74)
//	A=TNW ^ 0.74;
	A=pow(TNW,2);
	g=2*PLL/PLMAXL;
	SetPointZ=DSJTN+A*(DSJTG+DSJTH-2*DSJTN)/2;	//石(3.48)
	SetPointZ=DDXS+SetPoint+(DSJTG-DSJTH)*TNW/g;//YTQ?
   }
//CONTROL
//#define	BPBONZ	ALL_DATA_RAM[DO6_DATA_BASE];		//质调节变频泵,=0停止,=1启动! 
if (auto_flagz==0) {ALL_DATA_RAM[DO6_DATA_BASE]=0; OUT_PLZ=PLZ; SumErrorZ=0;}	//无扰
if (bpqerr_flagz==1) {ALL_DATA_RAM[DO6_DATA_BASE]=0; OUT_PLZ=0;}		//=0
//g=2*PL/PLMAX;
//if (g>0.2) && 
if ((bpqerr_flagz==0) && (auto_flagz==1)) //变频器为无故障//为自动
   {ALL_DATA_RAM[DO6_DATA_BASE]=1;				//变频泵启动=1
   	Error = SetPointZ-TG;   // 冬偏差
	Proportion= ProportionZ;
	SumError= SumErrorZ;
	out0=out0Z;
	PID();
	SumErrorZ= SumError;
	out0Z=out0;
	OUT_PLZ= out0;	//质调节输出频率(Hz),
   }
  ALL_DATA_RAM[DAC3_FINAL_BASE] = (s16)(OUT_PLZ*100);//
	ALL_DATA_RAM[FINAL_OUTDOOR_SET_TEMPERATURE_BASE] = (s16)(SetPointZ*100);
//ALL_DATA_RAM[DAC3_FINAL_BASE] = 2500;
}

