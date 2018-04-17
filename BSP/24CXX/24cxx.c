#include "24cxx.h" 
#include "delay.h"
#include "ucos_ii.h"
#include "myiic.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//24CXX驱动 代码(适合24C01~24C16,24C32~256未经过测试!有待验证!)		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////



#define Device_Address  0xA0  //24CXX的设备地址
/*******************************************************************************
* 函数名称: 24CXX_Read_Byte()
* 功能描述: 从24CXX的指定地址读取1个字节的数据
* 输入参数: dataaddress--数据读取的地址
* 返回参数: 读取的数据
********************************************************************************/
unsigned char EEPROM_Read_Byte(unsigned short dataaddress)
{
        unsigned char temp;
	unsigned char addr_hi, addr_lo;
	
	addr_hi = (dataaddress & 0xff00) >> 8;
	addr_lo = (dataaddress & 0xff);
    
	      I2C_Start();                         // 起始条件
	      I2C_Send_Byte(Device_Address);       // 向从器件发送设备地址
	      if(I2C_Wait_Ack()==0)                // 如果有从器件应答
        {
					I2C_Send_Byte(addr_hi);      // 发送数据地址高字节
					if(I2C_Wait_Ack()==0)                // 如果有从器件应答
					{
						I2C_Send_Byte(addr_lo);      // 发送数据地址低字节
					}
				}
	      else 
             return 1;
	      if(I2C_Wait_Ack()==0) 
	      {
	          I2C_Start();                       // 起始条件
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
* 函数名称: 24CXX_Read_NByte()
* 功能描述: 从24CXX的指定地址读取N个字节的数据
* 输入参数: pbuffer--指向保存数据地址的指针
            n--读取数据的个数
            dataaddress--数据读取的首地址
* 返回参数: 读取结果：0--成功，1--失败
********************************************************************************/
unsigned char  EEPROM_Read_NByte( unsigned char *pbuffer, unsigned char n, unsigned short dataaddress )
{
	unsigned char addr_hi, addr_lo;
	
	addr_hi = (dataaddress & 0xff00) >> 8;
	addr_lo = (dataaddress & 0xff);

        I2C_Start();                         // 起始条件
	      I2C_Send_Byte(Device_Address);       // 向从器件发送设备地址
	      if(I2C_Wait_Ack()==0)                // 如果有从器件应答
        {
					I2C_Send_Byte(addr_hi);      // 发送数据地址高字节
					if(I2C_Wait_Ack()==0)                // 如果有从器件应答
					{
						I2C_Send_Byte(addr_lo);      // 发送数据地址低字节
					}
				}
	      else 
               return 1;
	      if(I2C_Wait_Ack()==0) 
	      {
	        I2C_Start();                       // 起始条件
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
* 函数名称:  24CXX_Write_Byte();
* 功能描述:  向24CXX的指定地址中写入1个字节的数据  
* 输入参数:  wdata--写入的数据
             dataaddress--数据读取的首地址
* 返回参数:  写入结果：0--成功，1--失败
********************************************************************************/
unsigned char  EEPROM_Write_Byte( unsigned short dataaddress, unsigned char wdata )
{
	unsigned char addr_hi, addr_lo;
	
	addr_hi = (dataaddress & 0xff00) >> 8;
	addr_lo = (dataaddress & 0xff);
	
        I2C_Start();                         // 起始条件
        I2C_Send_Byte(Device_Address);       // 向从器件发送设备地址
	      if(I2C_Wait_Ack()==0)                // 如果有从器件应答
        {
					I2C_Send_Byte(addr_hi);      // 发送数据地址高字节
					if(I2C_Wait_Ack()==0)                // 如果有从器件应答
					{
						I2C_Send_Byte(addr_lo);      // 发送数据地址低字节
					}
				}
	      else 
            return 1;
        if(I2C_Wait_Ack()==0)                // 如果从器件应答
            I2C_Send_Byte(wdata);            // 发送要写入的数据
        else 
               return 1 ;
        if(I2C_Wait_Ack()==0)                // 如果从器件应答
               I2C_Stop();                   // 如果从器件应答 ，发送停止条件
        else           
               return 1;
    
        delay_ms(50);                 //等待EEPROM完成内部写入
				//OSTimeDlyHMSM(0,0,0,5); //datasheet says the max waiting time is 5 ms
	      return 0 ; 
}
/*******************************************************************************
* 函数名称:  24CXX_Write_NByte();
* 功能描述:  向24CXX的指定地址中写入1个字节的数据  
* 输入参数:  dataaddress--数据读取的地址
* 返回参数:  写入结果：0--成功，1--失败
********************************************************************************/
unsigned char  EEPROM_Write_NByte( unsigned char *pbuffer, unsigned char n, unsigned short dataaddress )
{
         unsigned char  temp; 
	unsigned char addr_hi, addr_lo;
	
	addr_hi = (dataaddress & 0xff00) >> 8;
	addr_lo = (dataaddress & 0xff);
    
	       I2C_Start();                         // 起始条件
         I2C_Send_Byte(Device_Address);       // 向从器件发送设备地址
	       if(I2C_Wait_Ack()==0)                // 如果有从器件应答
        {
					I2C_Send_Byte(addr_hi);      // 发送数据地址高字节
					if(I2C_Wait_Ack()==0)                // 如果有从器件应答
					{
						I2C_Send_Byte(addr_lo);      // 发送数据地址低字节
					}
				}
	       else 
               return 1;
         if(I2C_Wait_Ack()==0)                // 如果从器件应答
             temp = I2C_Send_NByte(pbuffer, n);
	       else 
               return 1;
         delay_ms(50);                 //等待EEPROM完成内部写入
					//OSTimeDlyHMSM(0,0,0,5); //datasheet says the max waiting time is 5 ms
	       if(temp==0)    return 0;
	       else           return 1;

}
/*******************************************************************************
* 函数名称:  EEPROM_Read();
* 功能描述:  该函数用于读出16bit或者32bit的数据.
* 输入参数:  dataaddress--数据读取的地址
* 返回参数:  要读出数据的长度2,4
********************************************************************************/
unsigned long EEPROM_Read(unsigned short dataaddress, unsigned char  Len)
{  	
	     unsigned char i;
	     unsigned long temp=0;
	     for(i=0;i<Len;i++)
	     {
		      temp <<= 8;
		      //temp += EEPROM_Read_Byte(dataaddress+Len-i-1); 	(大端) 
          temp += EEPROM_Read_Byte(dataaddress+i);				 
	     }
	return temp;												    
}
/*******************************************************************************
* 函数名称:  24CXX_Write_NByte();
* 功能描述:  该函数用于写入16bit或者32bit的数据.
* 输入参数:  dataaddress--数据读取的地址
* 返回参数:  要写入数据的长度2,4
********************************************************************************/
void EEPROM_Write(unsigned short dataaddress, unsigned long wdata,unsigned char Len)
{  	
	     unsigned char i;
	     for(i=0;i<Len;i++)
	     {
		      //EEPROM_Write_Byte(WriteAddr+i,(wdata>>(8*i))&0xff);(大端) 
				  EEPROM_Write_Byte( dataaddress +i,(wdata>>(24-8*i))&0xff);
	     }												    
}
