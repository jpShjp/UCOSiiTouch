#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//IO方向设置
#define I2C_PRORT         GPIOB
#define I2C_PORT_CLK      RCC_APB2Periph_GPIOE

#define I2C_SDA_PIN       GPIO_Pin_9
#define I2C_SCL_PIN       GPIO_Pin_8


#define	I2C_SDA_H         GPIO_SetBits(I2C_PRORT, I2C_SDA_PIN)
#define	I2C_SDA_L         GPIO_ResetBits(I2C_PRORT, I2C_SDA_PIN)
#define I2C_SDA_VAL       GPIO_ReadInputDataBit(I2C_PRORT,I2C_SDA_PIN)
#define	I2C_SCL_H         GPIO_SetBits(I2C_PRORT, I2C_SCL_PIN )
#define	I2C_SCL_L         GPIO_ResetBits(I2C_PRORT, I2C_SCL_PIN)



/*  子函数定义        */
extern  void I2C_GPIO_Init(void);
extern  void I2C_SDA_OUT(void);
extern  void I2C_SDA_IN(void);
extern  void I2C_Start(void);
extern  void I2C_Stop(void);
extern  unsigned char I2C_Wait_Ack(void);
extern  void I2C_Ack(void);
extern  void I2C_NAck(void);
extern  void I2C_Write_1(void);
extern  void I2C_Write_0(void);
extern  void I2C_Send_Byte(unsigned char ucData);
extern  unsigned char I2C_Send_NByte(unsigned char * pbuffer, unsigned char n);
extern  unsigned char I2C_Read_Byte(void);
extern  void I2C_Read_NByte(unsigned char * pbuffer, unsigned char n);
  
#endif
















