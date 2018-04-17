/*********************************Copyright (c)*********************************                                      
  *                                            
  ******************************************************************************
  * 文件名称：24CXX.H
  * 描    述：EEPROM模块头文件
  * 创 建 者: 
  * 创建日期:   
  * 修 改 者:             
  * 修改日期:      
  * 版    本: 
  ******************************************************************************
  * attention
  *
  ******************************************************************************
  */

#ifndef  __24CXX_H__
#define  __24CXX_H__


/**********************头文件调用及变量定义声明*********************************/
/*  包含必要的头文件  */


/*  子函数定义        */
unsigned char EEPROM_Read_Byte(unsigned short dataaddress );
unsigned char  EEPROM_Read_NByte( unsigned char *pbuffer, unsigned char n, unsigned short dataaddress );
unsigned char  EEPROM_Write_Byte( unsigned short dataaddress, unsigned char wdata );
unsigned char  EEPROM_Write_NByte( unsigned char *pbuffer, unsigned char n, unsigned short dataaddress );
unsigned long  EEPROM_Read(unsigned short dataaddress, unsigned char  Len);
void           EEPROM_Write(unsigned short dataaddress, unsigned long wdata,unsigned char Len);



#endif  

