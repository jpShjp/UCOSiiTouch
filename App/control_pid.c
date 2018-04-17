//TPK8CONTROL.c	******************��control_main.c
//****************  �ֲ�ʽ��ůϵͳ����վ�Ż����ܿ���V2.0  ************
//&&&&&&&&&&&&&&&&&&&&&&&   TPK8CONTROL,V2.0  &&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&    BAY YTQ,2015.1   &&&&&&&&&&&&&&&&&&&&&&&&
//ֻ���ļ���������"TPK8CONTROL.H"��"TPK8CONTROL.C"  ******************
//��"YY" �ҵ���Ҫ��ͬȷ���ĵط���*************************************
//#include <math.h>
#include<control_pid.h>
#include<control_global.h>

	#define k1 100			   //yyС������
	#define k2 1000			   //yyС������
	static int init_flag;
	int auto_flagz,auto_flagl,bpqerr_flagz,bpqerr_flagl;
	//&&&&&&&&&&&&&&&&  �����ɲ����õ���״̬   &&&&&&&&&&&&&&&&
	//int				//���ݱ��������������INT,Ҳ������CHR����λ���� 	
	//		auto_flagZ,		//�ʵ�,=0Ϊ�Զ�
	//		auto_flagL,		//����,=0Ϊ�Զ�
	//		bpqerr_flagZ,	//�ʵ��ڱ�Ƶ������λ=1Ϊ����
	//		bpqerr_flagL;	//�����ڱ�Ƶ������λ=1Ϊ����
	static int out_plz,out_pll;

//&&&&&&&&&&&&&&&&  �����ɲ���,ͨѶ�õ�����ֵ   &&&&&&&&
float	 
		TW,		//ʵ������,TWFROM=3--TW��λ��ͨѶ,���򱾻�����
		PLL,	//������ˮ��ʵ��Ƶ��  (HZ)
		PLZ,	//�ʵ���ˮ��ʵ��Ƶ��  (HZ)
        CY, 	//������[��]ѹ	(MPa)[LLKZGK=1��3��4ʱ����!!!!!!!!!!!]
		TG,		//ʵ�ʹ�ˮ�¶�
		TH,		//ʵ�ʻ�ˮ�¶�
 	    DDXS;	//�ʵ�����ϵ����,����λ��ͨѶ����,���趨��ˮ�¶�+-:
				//SetPointZ=SetPointZ+DDXS,�Ӷ�ʵ��ϵͳ��ͳһ����!

//&&&&&&&&&&&&&&&&  �����ɲ����趨��õ�����ֵ   &&&&&&&&&&&&&&&
int 
		LLKZGK,		//�������ƹ���
		WDKZGK;		//�¶ȿ��ƹ���
float	
		DSJTN,		//����������¶ȡ�
		DSJTW,		//�����������  ��
		DQTTW,		//����������¶ȡ�
		DSJTG,		//����ƹ�ˮ�¶ȡ�
		DSJTH,		//����ƻ�ˮ�¶ȡ�
		DSJCYL,		//�����[��]ѹMpa(��ѹ������,������)
		RDXJZ,		//�ȶ�������ϵ��
		PLMAXL,		//��������ƹ���Ƶ��HZ(<=50,��Ӧ����������¶�)
		PLMINL,		//���������Ƶ��HZ(��Ӧ����������¶�)
		PLMINZ,		//�ʵ���ˮ�����Ƶ��  (HZ),>=0		
		Proportion,  //�������� Proportional Const
		ProportionZ, //�ʵ��������� Proportional Const
		ProportionL, //������������ Proportional Const
		Integral,    //���ֳ��� Integral Const
		IntegralZ,   //�ʵ����ֳ��� Integral Const
		IntegralL;   //�������ֳ��� Integral Const

//&&&&&&&&&&&&&&&&  �ɱ�����ȷ���ľ�̬��������ֵ   &&&&&&&&&&&&&&&&
float	static
   		SumErrorZ,   //�ʵ�PI Sums of Errors,����PreEror
   		SumErrorL,   //�ʵ�PI Sums of Errors,����PreEror
		out0Z,		//�ʵ�����
		out0L;		//��������
float
		OUT_PLL,	//���������Ƶ��(Hz)!!!!!!!!!!!!!!!!!!!!
	    OUT_PLZ,	//�ʵ������Ƶ��(Hz)!!!!!!!!!!!!!!!!!!!!!
		TWp,		//�������¶�
		OUT,
		Error,
		SumError,
		out0,
		SetPoint ,   //�趨Ŀ��,
		SetPointZ,   //�ʵ��趨Ŀ��--�趨��ˮ�¶ȡ�,
		SetPointL,   //�����趨Ŀ��--�趨[��]ѹMPa,
		Set_LL;		//LLKZGK=2,�������趨�������%,�ɹ���ʾ

void TPK8control_init()
{
//	OS_CPU_SR  cpu_sr;
	//tpk8��ӳ�ʼ��
		TW=(float)ALL_DATA_RAM[ADC1_FINAL_BASE]/k1;//ʵ������,TWFROM=3--TW��λ��ͨѶ,���򱾻�����
		//TW=TW/k1;
		PLL=(float)ALL_DATA_RAM[ADC11_FINAL_BASE]/k1;	//adc11/12//������Ƶ��(Hz)
		//PLL=PLL/k1;
		PLZ=(float)ALL_DATA_RAM[ADC13_FINAL_BASE]/k1;	//adc13/14//�ʵ���Ƶ��(Hz)
		TWp=TW; //(float)ALL_DATA_RAM[WINTER_OUTDOOR_TEMPERATURE_SET_BASE/k1];	//DSJTW;//�������¶�
		OUT_PLL=PLL;	//���������Ƶ��(Hz)�������л�
	    OUT_PLZ=PLZ;	//�ʵ������Ƶ��(Hz), �����л�
		out0Z=PLZ;		//�ʵ����ã������л�
		out0L=PLL;		//�������ã������л�
   		SumErrorZ=0;   	//�ʵ�PI Sums of Errors,����PreEror
   		SumErrorL=0;    //����PI Sums of Errors,����PreEror
//#define	BPBONL	ALL_DATA_RAM[DO5_DATA_BASE];		//�����ڱ�Ƶ��,=0ֹͣ,=1����! 
//#define	BPBONZ	ALL_DATA_RAM[DO6_DATA_BASE];		//�ʵ��ڱ�Ƶ��,=0ֹͣ,=1����! 
		ALL_DATA_RAM[DO5_DATA_BASE]=0;//BPBONL,		//�����ڱ�Ƶ��,=0ֹͣ,=1����!
		ALL_DATA_RAM[DO6_DATA_BASE]=0;
// 		SetPointZ=DSJPMAX   //�ʵ��趨Ŀ��--�趨��ˮ�¶ȡ�,
//		SetPointL=PLMAX     //�����趨Ŀ��--�趨[��]ѹMPa,
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
	//tpk8���
			//���ݱ�������������INT,Ҳ������CHR����λ����
	auto_flagz = ALL_DATA_RAM[DI9_DATA_BASE];
	auto_flagl = ALL_DATA_RAM[DI1_DATA_BASE];
	bpqerr_flagz = ALL_DATA_RAM[DI10_DATA_BASE];
	bpqerr_flagl = ALL_DATA_RAM[DI2_DATA_BASE];
	
	TW=(float)ALL_DATA_RAM[ADC1_FINAL_BASE]/k1;//ʵ������,TWFROM=3--TW��λ��ͨѶ,���򱾻�����
	//TW=TW/k1;
    CY=(float)ALL_DATA_RAM[ADC2_FINAL_BASE]/k2;//������[��]ѹ	(MPa)[LLKZGK=1��3��4ʱ����
	//CY=CY/k2;
	TG=(float)ALL_DATA_RAM[ADC5_FINAL_BASE]/k1;//ʵ�ʹ�ˮ�¶�
	//TG=TG/k1;
	TH=(float)ALL_DATA_RAM[ADC6_FINAL_BASE]/k1;//ʵ�ʻ�ˮ�¶�
	//TH=TH/k1;
 	DDXS=0;	//YY�ʵ�����ϵ����,����λ��ͨѶ����,���趨��ˮ�¶�+-://yy
	PLL=(float)ALL_DATA_RAM[ADC11_FINAL_BASE]/k1;	//adc11/12//������Ƶ��(Hz)
	//PLL=PLL/k1;
	PLZ=(float)ALL_DATA_RAM[ADC13_FINAL_BASE]/k1;	//adc13/14//�ʵ���Ƶ��(Hz)
	//PLZ=PLZ/k1;
	DSJTW=(float)ALL_DATA_RAM[WINTER_OUTDOOR_TEMPERATURE_SET_BASE]/k1;	//�������¶�
	//DSJTW=DSJTW/k1;
	DSJTN=(float)ALL_DATA_RAM[WINTER_INDOOR_TEMPERATURE_SET_BASE]/k1;	//����������¶ȡ�
	//DSJTN=DSJTN/k1;
	DSJCYL=(float)ALL_DATA_RAM[WINTER_DIF_PRESSURE_MAX_SET_BASE]/k2;	//�����[��]ѹMpa(��ѹ������,������)
	DSJCYL=DSJCYL/k2;
	//-(float)ALL_DATA_RAM[ WINTER_DIF_PRESSURE_MIN_SET_BASE]k2;//����С[��]ѹMpa(������)	
	PLMAXL=(float)ALL_DATA_RAM[WINTER_FREQUENCY_MAX_SET_BASE]/k1;//��������ƹ���Ƶ��HZ(<=50,��Ӧ����������¶�)
	//PLMAXL=PLMAXL/k1;	                
	PLMINL=(float)ALL_DATA_RAM[WINTER_FREQUENCY_MIN_SET_BASE]/k1;//���������Ƶ��HZ(��Ӧ����������¶�)
	//PLMINL=PLMINL/k1;	
	DQTTW=(float)ALL_DATA_RAM[WINTER_STARTPOINT_OUTDOOR_TEMPERATURE_SET_BASE]/k1;	//����������¶ȡ�	
	//DQTTW=DQTTW/k1;
	DSJTG=(float)ALL_DATA_RAM[WINTER_WATER_SUPPLY_MAX_TEMPERATURE_SET_BASE]/k1;	//����ƹ�ˮ�¶ȡ�	
	//DSJTG=DSJTG/k1;
	DSJTH=(float)ALL_DATA_RAM[WINTER_WATER_SUPPLY_MIN_TEMPERATURE_SET_BASE]/k1;	//����ƻ�ˮ�¶ȡ�	
	//DSJTH=DSJTH/k1;
	RDXJZ=3;		//YY�ȶ�������ϵ��
	PLMINZ=5;		//YY�ʵ���ˮ�����Ƶ��  (HZ),>=0		

	ProportionZ=(float)ALL_DATA_RAM[PID_TEMPERATURE_CONTROL_PROPORTION_BASE];//�ʵ��������� Proportional Const
	//ProportionZ=ProportionZ/k1;
	IntegralZ=(float)ALL_DATA_RAM[PID_TEMPERATURE_CONTROL_INTEGRAL_TIME_BASE]; //�ʵ����ֳ��� Integral Const
	//IntegralZ=IntegralZ/k1;
	ProportionL=(float)ALL_DATA_RAM[PID_PRESSURE_CONTROL_PROPORTION_BASE];		//������������ Proportional Const
	//ProportionL=ProportionL/k1;
	IntegralL=(float)ALL_DATA_RAM[PID_PRESSURE_CONTROL_INTEGRAL_TIME_BASE]; //�������ֳ��� Integral Const
	//IntegralL=IntegralL/k1;
	LLKZGK=0;	//yy
	WDKZGK= ALL_DATA_RAM[CONTROL_METHOD_SET_BASE];	//yy
		//TPK8control();	
	comput_TWp();				//��������-����
	if (LLKZGK==1)  autocontrol_lt1();	//����-�Զ�����,���ڶ�ѹϵͳ
	if (LLKZGK==2)  autocontrol_lt2();	//����-�Զ�����,������������ϵͳ
	autocontrol_zt();					//�Զ�����--�ʵ���
 }
		
void PID() 
//INPUT:Error, SumError,out0
//		Proportion,  //�������� Proportional Const��Hz/��
//		Integral,    //���ֳ��� Integral Const
//OUTPUT:SumError ,out0
{  float A;
   if (Error>3) Error=3;			//��ֹ����   30  
   if (Error<-3) Error=-3;		//��ֹ����   30  
   A=Proportion * (Error-SumError);    // ����������          
   SumError=Error;						//PreError=SumError = Error
   A=A  + Proportion*(Error/Integral);         	//+������,A=����
   	//A=A +  Derivative * dError    // ΢����
   if (A>1) A=1;			//<+-10
   if (A<-1) A=-1;			//<+-10
	A=A+out0;
	
	if (A>PLMAXL) A=PLMAXL;
	if (A<PLMINL) A=PLMINL;
	out0=A;
}


void comput_TWp()
//TPK8-3/4&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&��������-����
/*��֪:float	TW,		//ʵ������
				DSJTW,	//�����������  ��
				DQTTW,	//����������¶�
				RDXJZ,	//�ȶ�������ϵ��
  ȷ��:float	TWp,	//�������¶�; 
  ��ʼ��:		TWp=TW;
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
{
TWp=(TWp*RDXJZ+TW)/(1+RDXJZ);			//�߽��˲�//��������
 	//�������������TWP<�������DSJTW,ȡ��������TWP=�������DSJTW.
 if (TWp<DSJTW) TWp= DSJTW; 
	//5C	2011.11.14Ҫ����������¶�>5C,=5C
 if (TWp>DQTTW) TWp=DQTTW;
}

void autocontrol_lt1()			//�Զ����ơ���ѹ����
{
//float   A;
if (auto_flagl==1) {ALL_DATA_RAM[DO5_DATA_BASE]=0; OUT_PLL=PLL; SumErrorL=0;}		//����
if (bpqerr_flagl==1) {ALL_DATA_RAM[DO5_DATA_BASE]=0; OUT_PLL=0; }		//=0
if ((bpqerr_flagl==0) && (auto_flagl==0))		//��Ƶ��Ϊ�޹���//Ϊ�Զ�
   {ALL_DATA_RAM[DO5_DATA_BASE]=1;		//��Ƶ������=1
   	Error = DSJCYL-CY;		//�����[��]ѹMpa(��ѹ������,)-CY// ��ƫ��
	Proportion= ProportionL;
	SumError= SumErrorL;
	out0=out0L;
	PID();
	SumErrorL= SumError;
	out0L=out0;
	OUT_PLL= out0;	//L�������Ƶ��(Hz),
   }
	ALL_DATA_RAM[DAC1_FINAL_BASE] =(s16) OUT_PLL*100;	//
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&TPK3-����-�Զ�����-����-��ֵ��
void autocontrol_lt2()	//����-�Զ�����,������������ϵͳ,��Ƶ�ʼ�������,�޹�Ƶ��
/*	��֪:	bit 	bpqerr_flagL,		//��Ƶ������λ=1Ϊ����
					auto_flagL,			//=0Ϊ�Զ�
			float	TWp,		//�������¶�
					DSJTW,		//�����������  ��
					DQTTW,		//����������¶�
					PLMAX,		//���Ƶ��<=50Hz
					PLMIN,		//���Ƶ��<PLMAX
	ȷ��:   bit 	BPBONL		//��Ƶ������=1/ֹͣ=0			
  		 	//bit 	GPBON		//��Ƶ������=1/ֹͣ=0
			float	OUT_PL,		//���������Ƶ��:PLMIM--PLMAX;
	��ʼ��: bit		BPBON=0		//��Ƶ��ֹͣ			
  		 	//bit 	GPBON=0		//��Ƶ��ֹͣ
	     	float 	OUT_PL=PLL 	//������Ƶ��;����
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
{
if ((bpqerr_flagl==0) && (auto_flagl==0)) 	//��Ƶ��Ϊ�޹���//Ϊ�Զ�
  {ALL_DATA_RAM[DO5_DATA_BASE]=1;		//��Ƶ������=1
   if (PLMAXL>50)PLMAXL=50;
   OUT_PLL=PLMINL+(PLMAXL-PLMINL)*(DSJTW-TWp)/(DSJTW-DQTTW);	//���������Ƶ��(Hz)
   Set_LL=OUT_PLL*100/PLMAXL;				//�������趨�������%,����ʾ
   //OUT_PL=PLMIN+(PLMAX-PLMIN)*(DSJTW-TWp)/(DSJTW-DQTTW);	//���������Ƶ��(Hz)
   //DAOUT_PL=OUT_PL*DA_DATAmax/50;	  	//������DA����PWM���
  }
else
//#define	BPBONL	ALL_DATA_RAM[DO5_DATA_BASE];		//�����ڱ�Ƶ��,=0ֹͣ,=1����! 
  {ALL_DATA_RAM[DO5_DATA_BASE]=0;							//��Ƶ��ֹͣ=0
   if (bpqerr_flagl==1) OUT_PLL=0;		//=0
  }
ALL_DATA_RAM[DAC1_FINAL_BASE] =(s16) OUT_PLL*100;	//
}


/*************************************�ʵ���********************
void autocontrol_zt()		//�Զ�����--�ʵ���
ȷ��:   bit 	BPBONZ		//��Ƶ������=1/ֹͣ=0
  		//bit 	GPBONZ		//��Ƶ������=1/ֹͣ=0
		float	OUT_PLZ,	//�ʵ������Ƶ��;
��������˿�!
*****************************************************************
//=2011.12.26:����ϵ��DDXS(��Ҫ������)
//ע��:ֻҪ/ֻ����ͨѶȷ������ϵ��DDXS
 float 	PL,GW,TH,PLZ,	//������Ƶ��PL,��ˮ��,��ˮ��,Ƶ��
		DDXS,	//DDXS����ϵ����ֻ��ͨѶ�ı�,TWfrom=3,TWͨѶ
		TWp,	//������¶�
		DSJTW,	//�����������  ��
		PLMAX,	//���������Ƶ��
		DQTTW,	//����������¶�
		DSJTN,	//����������¶�
		DSJTG,	//����ƹ�ˮ�¶�
		DSJTH;	//����ƻ�ˮ�¶�
float   SumError,    //λ��Sums of Errors,����PreEror
 	    DDXS,		//DDXS����ϵ����ֻ��ͨѶ�ı�
float  SetPoint;        //  �趨Ŀ�� Desired Value
float  Proportion;         //  �������� Proportional Const
float  Integral;           //  ���ֳ��� Integral Const
//float  Derivative;       //  ΢�ֳ��� Derivative Const
//float  LastError;        //  Error[-1],΢��
//float  PrevError;        //  Error[-2],΢��
float  	PLZMIN;	//�ʵ���ˮ�����Ƶ��		
float  	SetPoint;
float  SumError;    //λ��Sums of Errors,����PreEror  
ȷ��:   bit 	BPBONZ		//��Ƶ������=1/ֹͣ=0
  		//bit 	GPBONZ		//��Ƶ������=1/ֹͣ=0
		float	OUT_PLZ,	//�ʵ������Ƶ��;
��ʼ��: bit		BPBONZ=0		//��Ƶ��ֹͣ			
  		//bit 	GPBONZ=0		//��Ƶ��ֹͣ
	    float 	out0=PLZ,		//�ʵ�-��Ƶ��DA0-����;
				SumError=0,
				DDXS=0;			//����ϵ�� 	 
*******************************************************************/

void autocontrol_zt()			//�Զ�����--�ʵ���
{
float   g,TNW,A;
if (WDKZGK ==1) SetPointZ=DSJTG;//����ƹ�ˮ�¶ȡ�,���¿���
if (WDKZGK ==2)					//���¿���
   {//void setpoint()			//�趨Ŀ��Tg
	TNW=(DSJTN-TWp)/(DSJTN-DSJTW);
//	A=sqrt(TNW);
//	A=A*sqrt(A);		//A=TNW^0.75; OR A=pow(TNW,0.74)
//	A=TNW ^ 0.74;
	A=pow(TNW,2);
	g=2*PLL/PLMAXL;
	SetPointZ=DSJTN+A*(DSJTG+DSJTH-2*DSJTN)/2;	//ʯ(3.48)
	SetPointZ=DDXS+SetPoint+(DSJTG-DSJTH)*TNW/g;//YTQ?
   }
//CONTROL
//#define	BPBONZ	ALL_DATA_RAM[DO6_DATA_BASE];		//�ʵ��ڱ�Ƶ��,=0ֹͣ,=1����! 
if (auto_flagz==0) {ALL_DATA_RAM[DO6_DATA_BASE]=0; OUT_PLZ=PLZ; SumErrorZ=0;}	//����
if (bpqerr_flagz==1) {ALL_DATA_RAM[DO6_DATA_BASE]=0; OUT_PLZ=0;}		//=0
//g=2*PL/PLMAX;
//if (g>0.2) && 
if ((bpqerr_flagz==0) && (auto_flagz==1)) //��Ƶ��Ϊ�޹���//Ϊ�Զ�
   {ALL_DATA_RAM[DO6_DATA_BASE]=1;				//��Ƶ������=1
   	Error = SetPointZ-TG;   // ��ƫ��
	Proportion= ProportionZ;
	SumError= SumErrorZ;
	out0=out0Z;
	PID();
	SumErrorZ= SumError;
	out0Z=out0;
	OUT_PLZ= out0;	//�ʵ������Ƶ��(Hz),
   }
  ALL_DATA_RAM[DAC3_FINAL_BASE] = (s16)(OUT_PLZ*100);//
	ALL_DATA_RAM[FINAL_OUTDOOR_SET_TEMPERATURE_BASE] = (s16)(SetPointZ*100);
//ALL_DATA_RAM[DAC3_FINAL_BASE] = 2500;
}

