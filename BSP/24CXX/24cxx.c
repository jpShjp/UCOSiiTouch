#include "24cxx.h" 
#include "delay.h"
#include "ucos_ii.h"
#include "myiic.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//24CXX���� ����(�ʺ�24C01~24C16,24C32~256δ��������!�д���֤!)		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////



#define Device_Address  0xA0  //24CXX���豸��ַ
/*******************************************************************************
* ��������: 24CXX_Read_Byte()
* ��������: ��24CXX��ָ����ַ��ȡ1���ֽڵ�����
* �������: dataaddress--���ݶ�ȡ�ĵ�ַ
* ���ز���: ��ȡ������
********************************************************************************/
unsigned char EEPROM_Read_Byte(unsigned short dataaddress)
{
        unsigned char temp;
	unsigned char addr_hi, addr_lo;
	
	addr_hi = (dataaddress & 0xff00) >> 8;
	addr_lo = (dataaddress & 0xff);
    
	      I2C_Start();                         // ��ʼ����
	      I2C_Send_Byte(Device_Address);       // ������������豸��ַ
	      if(I2C_Wait_Ack()==0)                // ����д�����Ӧ��
        {
					I2C_Send_Byte(addr_hi);      // �������ݵ�ַ���ֽ�
					if(I2C_Wait_Ack()==0)                // ����д�����Ӧ��
					{
						I2C_Send_Byte(addr_lo);      // �������ݵ�ַ���ֽ�
					}
				}
	      else 
             return 1;
	      if(I2C_Wait_Ack()==0) 
	      {
	          I2C_Start();                       // ��ʼ����
	          I2C_Send_Byte((Device_Address|0x01));
	      }
	      else    
             return 0;
	      if(I2C_Wait_Ack()==0)	
             temp = I2C_Read_Byte();
	      else 
             return 0;

	       I2C_NAck();
	       I2C_Stop();
	       return temp;
     	
}
/*******************************************************************************
* ��������: 24CXX_Read_NByte()
* ��������: ��24CXX��ָ����ַ��ȡN���ֽڵ�����
* �������: pbuffer--ָ�򱣴����ݵ�ַ��ָ��
            n--��ȡ���ݵĸ���
            dataaddress--���ݶ�ȡ���׵�ַ
* ���ز���: ��ȡ�����0--�ɹ���1--ʧ��
********************************************************************************/
unsigned char  EEPROM_Read_NByte( unsigned char *pbuffer, unsigned char n, unsigned short dataaddress )
{
	unsigned char addr_hi, addr_lo;
	
	addr_hi = (dataaddress & 0xff00) >> 8;
	addr_lo = (dataaddress & 0xff);

        I2C_Start();                         // ��ʼ����
	      I2C_Send_Byte(Device_Address);       // ������������豸��ַ
	      if(I2C_Wait_Ack()==0)                // ����д�����Ӧ��
        {
					I2C_Send_Byte(addr_hi);      // �������ݵ�ַ���ֽ�
					if(I2C_Wait_Ack()==0)                // ����д�����Ӧ��
					{
						I2C_Send_Byte(addr_lo);      // �������ݵ�ַ���ֽ�
					}
				}
	      else 
               return 1;
	      if(I2C_Wait_Ack()==0) 
	      {
	        I2C_Start();                       // ��ʼ����
	        I2C_Send_Byte((Device_Address|0x01));
	      }
      	else      
					    return 1;
	      if(I2C_Wait_Ack()==0)	
               I2C_Read_NByte(pbuffer, n);
	       else 
               return 1;
	
	      return  0;	
}
/*******************************************************************************
* ��������:  24CXX_Write_Byte();
* ��������:  ��24CXX��ָ����ַ��д��1���ֽڵ�����  
* �������:  wdata--д�������
             dataaddress--���ݶ�ȡ���׵�ַ
* ���ز���:  д������0--�ɹ���1--ʧ��
********************************************************************************/
unsigned char  EEPROM_Write_Byte( unsigned short dataaddress, unsigned char wdata )
{
	unsigned char addr_hi, addr_lo;
	
	addr_hi = (dataaddress & 0xff00) >> 8;
	addr_lo = (dataaddress & 0xff);
	
        I2C_Start();                         // ��ʼ����
        I2C_Send_Byte(Device_Address);       // ������������豸��ַ
	      if(I2C_Wait_Ack()==0)                // ����д�����Ӧ��
        {
					I2C_Send_Byte(addr_hi);      // �������ݵ�ַ���ֽ�
					if(I2C_Wait_Ack()==0)                // ����д�����Ӧ��
					{
						I2C_Send_Byte(addr_lo);      // �������ݵ�ַ���ֽ�
					}
				}
	      else 
            return 1;
        if(I2C_Wait_Ack()==0)                // ���������Ӧ��
            I2C_Send_Byte(wdata);            // ����Ҫд�������
        else 
               return 1 ;
        if(I2C_Wait_Ack()==0)                // ���������Ӧ��
               I2C_Stop();                   // ���������Ӧ�� ������ֹͣ����
        else           
               return 1;
    
        delay_ms(50);                 //�ȴ�EEPROM����ڲ�д��
				//OSTimeDlyHMSM(0,0,0,5); //datasheet says the max waiting time is 5 ms
	      return 0 ; 
}
/*******************************************************************************
* ��������:  24CXX_Write_NByte();
* ��������:  ��24CXX��ָ����ַ��д��1���ֽڵ�����  
* �������:  dataaddress--���ݶ�ȡ�ĵ�ַ
* ���ز���:  д������0--�ɹ���1--ʧ��
********************************************************************************/
unsigned char  EEPROM_Write_NByte( unsigned char *pbuffer, unsigned char n, unsigned short dataaddress )
{
         unsigned char  temp; 
	unsigned char addr_hi, addr_lo;
	
	addr_hi = (dataaddress & 0xff00) >> 8;
	addr_lo = (dataaddress & 0xff);
    
	       I2C_Start();                         // ��ʼ����
         I2C_Send_Byte(Device_Address);       // ������������豸��ַ
	       if(I2C_Wait_Ack()==0)                // ����д�����Ӧ��
        {
					I2C_Send_Byte(addr_hi);      // �������ݵ�ַ���ֽ�
					if(I2C_Wait_Ack()==0)                // ����д�����Ӧ��
					{
						I2C_Send_Byte(addr_lo);      // �������ݵ�ַ���ֽ�
					}
				}
	       else 
               return 1;
         if(I2C_Wait_Ack()==0)                // ���������Ӧ��
             temp = I2C_Send_NByte(pbuffer, n);
	       else 
               return 1;
         delay_ms(50);                 //�ȴ�EEPROM����ڲ�д��
					//OSTimeDlyHMSM(0,0,0,5); //datasheet says the max waiting time is 5 ms
	       if(temp==0)    return 0;
	       else           return 1;

}
/*******************************************************************************
* ��������:  EEPROM_Read();
* ��������:  �ú������ڶ���16bit����32bit������.
* �������:  dataaddress--���ݶ�ȡ�ĵ�ַ
* ���ز���:  Ҫ�������ݵĳ���2,4
********************************************************************************/
unsigned long EEPROM_Read(unsigned short dataaddress, unsigned char  Len)
{  	
	     unsigned char i;
	     unsigned long temp=0;
	     for(i=0;i<Len;i++)
	     {
		      temp <<= 8;
		      //temp += EEPROM_Read_Byte(dataaddress+Len-i-1); 	(���) 
          temp += EEPROM_Read_Byte(dataaddress+i);				 
	     }
	return temp;												    
}
/*******************************************************************************
* ��������:  24CXX_Write_NByte();
* ��������:  �ú�������д��16bit����32bit������.
* �������:  dataaddress--���ݶ�ȡ�ĵ�ַ
* ���ز���:  Ҫд�����ݵĳ���2,4
********************************************************************************/
void EEPROM_Write(unsigned short dataaddress, unsigned long wdata,unsigned char Len)
{  	
	     unsigned char i;
	     for(i=0;i<Len;i++)
	     {
		      //EEPROM_Write_Byte(WriteAddr+i,(wdata>>(8*i))&0xff);(���) 
				  EEPROM_Write_Byte( dataaddress +i,(wdata>>(24-8*i))&0xff);
	     }												    
}
