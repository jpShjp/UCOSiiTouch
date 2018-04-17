#include "myiic.h"
#include "delay.h"
#include "stm32f10x_it.h" 
//#include "control_bsp.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
/*******************************************************************************
* ��������: I2C_Gpio_Config();
* ��������: ����I2C����,�����ģ��ķ���ʵ��I2C����
* �������: void
* ���ز���: ��
********************************************************************************/
 void I2C_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure ;

     RCC_APB2PeriphClockCmd(I2C_PORT_CLK ,ENABLE);

     //����I2C_SCL/I2C_SDA����
     GPIO_InitStructure.GPIO_Pin   = I2C_SDA_PIN |I2C_SCL_PIN ;
     GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_OD;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
     GPIO_Init(I2C_PRORT, &GPIO_InitStructure); 
     GPIO_SetBits(I2C_PRORT, I2C_SDA_PIN |I2C_SCL_PIN );
     	
}
/*******************************************************************************
* ��������: I2C_SDA_OUT();
* ��������: ����SDA�˿�Ϊ���ģʽ
* �������: void
* ���ز���: ��
********************************************************************************/
 void I2C_SDA_OUT(void)
{
      GPIO_InitTypeDef  GPIO_InitStructure; 
      GPIO_InitStructure.GPIO_Pin   = I2C_SDA_PIN  ;
      GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_OD;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
      GPIO_Init(I2C_PRORT, &GPIO_InitStructure); 
}
/*******************************************************************************
* ��������: I2C_SDA_IN();
* ��������: ����SDA�˿�Ϊ����ģʽ
* �������: void
* ���ز���: ��
********************************************************************************/
 void I2C_SDA_IN(void)
{
      GPIO_InitTypeDef  GPIO_InitStructure; 
      GPIO_InitStructure.GPIO_Pin   = I2C_SDA_PIN  ;
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
      GPIO_Init(I2C_PRORT, &GPIO_InitStructure); 
}
/*******************************************************************************
* ��������: I2C_Start();
* ��������: ��ʼ����
* �������: void
* ���ز���: ��
********************************************************************************/
void I2C_Start(void)
{
	I2C_SDA_OUT();
	I2C_SDA_H ;	  	  
	I2C_SCL_H ;
	delay_us(4);
 	I2C_SDA_L;   //START:when CLK is high,DATA change form high to low 
	delay_us(4);
	I2C_SCL_L  ;//ǯסI2C���ߣ�׼�����ͻ�������� 
  delay_us(4);
}
/*******************************************************************************
* ��������: I2C_Stop();
* ��������: ֹͣ����
* �������: void
* ���ز���: ��
********************************************************************************/
void I2C_Stop(void)
{
	      I2C_SDA_OUT();
        //I2C_SCL_L  ;
	      I2C_SDA_L; //STOP:when CLK is high DATA change form low to high	  	  
	      delay_us(4);
        I2C_SCL_H ;
	      delay_us(4);
	      I2C_SDA_H ; //����I2C���߽����ź�
        delay_us(4);	
}
/*******************************************************************************
* ��������: I2C_Wait_Ack();
* ��������: ���ӻ���Ӧ�����
* �������: ��
* ���ز���: �ӻ��Ƿ���Ӧ��1--ʧ�ܣ�0--�ɹ�
********************************************************************************/
unsigned char I2C_Wait_Ack(void)
{
	unsigned char utemp ;
	
	      I2C_SDA_H ;
        delay_us(2);	   
	      I2C_SCL_H ;
        delay_us(2);	 
	      I2C_SDA_IN();  //SDA����Ϊ����   
        utemp = I2C_SDA_VAL;
        delay_us(2);
        I2C_SCL_L  ;	
        delay_us(2);
        I2C_SDA_OUT();
        if( utemp)       return  1 ;
        else             return  0 ;
	
} 
/*******************************************************************************
* ��������: I2C_Ack();
* ��������: ����I2C������Ӧ�����
* �������: ��
* ���ز���: ��
********************************************************************************/
void I2C_Ack(void)
{       
        
	I2C_SCL_L;
  I2C_SDA_OUT();	
	I2C_SDA_L;
	delay_us(2);
	I2C_SCL_H ;
	delay_us(2);
	I2C_SCL_L;
        //I2C_SDA_H;
}
/*******************************************************************************
* ��������: I2C_NAck();
* ��������: ������I2C������Ӧ�����
* �������: ��
* ���ز���: ��
********************************************************************************/
void I2C_NAck(void)
{
	I2C_SCL_L;
  I2C_SDA_OUT();	
	I2C_SDA_H;
	delay_us(2);
	I2C_SCL_H ;
	delay_us(2);
	I2C_SCL_L;
}
/*******************************************************************************
* ��������: I2C_Write_1();
* ��������: ��IIC���߷���һ��1
* �������: ��
* ���ز���: ��
********************************************************************************/
 void I2C_Write_1(void)
{
	I2C_SDA_H;
	delay_us(2);
	I2C_SCL_H ;
	delay_us(2);
	I2C_SCL_L;		
	delay_us(2);
}
/*******************************************************************************
* ��������: I2C_Write_0();
* ��������: ��IIC���߷���һ��0
* �������: ��
* ���ز���: ��
********************************************************************************/
 void I2C_Write_0(void)
{
	I2C_SDA_L;
	delay_us(2);
	I2C_SCL_H ;
	delay_us(2);
	I2C_SCL_L;		
	delay_us(2);
}
/*******************************************************************************
* ��������: I2C_Send_Byte();
* ��������: ��IIC���߷���һ���ֽڵ�����
* �������: ucData--���͵�����
* ���ز���: ��
********************************************************************************/
void I2C_Send_Byte(unsigned char ucData)
{    
      unsigned char i;
	    //I2C_SDA_OUT();	
	    //I2C_SCL_L;		
      for(i = 8;i > 0;i--)
      {
	      if( ucData & 0x80)  I2C_Write_1();
	      else                I2C_Write_0();
        delay_us(2);
	      ucData <<= 1;
       }                  
	
       I2C_SDA_H;
			 delay_us(2);
			
}
/*******************************************************************************
* ��������: I2C_Send_Byte();
* ��������: ��IIC���߷���N���ֽڵ�����
* �������: pbuffer--ָ�������ݴ���׵�ַ
            n--���ݵĸ���
* ���ز���: �����Ƿ�ɹ��ı�־��0--�ɹ���1--ʧ��
********************************************************************************/
unsigned char I2C_Send_NByte(unsigned char * pbuffer, unsigned char n)
{
        unsigned char i;
        for(i = 0;i < n;i++)
        {  
           I2C_Send_Byte(* pbuffer);
	         if(I2C_Wait_Ack()==0)      
	         {
	            pbuffer++;			
	          }  
	          else 
	         {
	           I2C_Stop();
	            return 1;
	          }
        }
         I2C_Stop();
         return 0;	
}
/*******************************************************************************
* ��������: I2C_Read_Byte();
* ��������: ��IIC���߶�ȡһ���ֽ�
* �������: ��
* ���ز���: ��ȡ������
********************************************************************************/
unsigned char I2C_Read_Byte(void)
{    
      unsigned char  uData =0x00;
      unsigned char utemp,i;
      for(i = 8;i > 0;i--)
      {   
          I2C_SDA_H;
	        delay_us(2);
	        I2C_SCL_H ;
	         delay_us(2);
          I2C_SDA_IN();  //SDA����Ϊ����        
          delay_us(2);
          utemp = I2C_SDA_VAL;
          uData <<= 1;
          if(utemp) uData |= 0x01;
          delay_us(2);
          I2C_SCL_L;		
          delay_us(2);
          I2C_SDA_OUT();	
	
       }                  	
          I2C_SDA_H;
          delay_us(2);
			    return( uData );
}
/*******************************************************************************
* ��������: I2C_Read_NByte();
* ��������: ��IIC���߶�ȡN���ֽڵ�����
* �������: pbuffer--ָ�������ݴ���׵�ַ
            n--���ݵĸ���
* ���ز���: ��ȡ������
********************************************************************************/
void I2C_Read_NByte(unsigned char * pbuffer, unsigned char n)
{    
        unsigned char  i;
	for(i = 0;i < n;i++)
	{
	    pbuffer[i] = I2C_Read_Byte();
	    if( i < (n-1))   I2C_Ack();  //Ӧ��
	    else             I2C_NAck(); //��Ӧ��
	}
	 
	I2C_Stop();

}
