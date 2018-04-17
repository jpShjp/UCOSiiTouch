/**************-*1***** (C) COPYRIGHT 2014 YIYA ********************
* File Name          : control_handle.c
* Author             : YIYA Team
* Version            : V0.1
* Date               : 11/20/2014
* Description        : This file provides all the bsp firmware functions.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "comm_handle.h"
#include "ucos_ii.h"
#include "control_comm.h"
#include "control_global.h"
#include "sys.h"
#include "usart.h"
#include "control_main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function ----------------------------------------------------------*/
/* Private function ----------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
// COM2 modbus master --------------------------------
#define READ_SLAVE_COUNT 8;


//------------------------------------------------------
void P_Comm3_Handle(void)
{
    OS_CPU_SR  cpu_sr = 0;

    u8 i = 0;
    u8 sys_number_temp = 0;
    u16 data_crc_temp = 0;
    u16 num_of_registers_temp = 0;
    u16 starting_target_addr_temp = 0;
    u16 required_value_temp = 0;
    static COMM_DATA comm3_recebuf_temp;
    static COMM_DATA comm3_sendbuf_temp;
    u16 *pdes;
    u8 err;

    OS_ENTER_CRITICAL();
    sys_number_temp = (u8)ALL_DATA_RAM[SYS_NUMBER_BASE];
    memcpy(&comm3_recebuf_temp,&COMM3_ReceBuf,sizeof(COMM3_ReceBuf));
    COMM3_ReceBuf.DataCount = 0;
    COMM3_ReceBuf.DataLong  = 0;
    OS_EXIT_CRITICAL();
    
    data_crc_temp = Get_CRC(&comm3_recebuf_temp.DataBuf[0], (comm3_recebuf_temp.DataLong - 2));
    if(data_crc_temp == (u16)(((u16)comm3_recebuf_temp.DataBuf[comm3_recebuf_temp.DataLong - 2] << 8) | (comm3_recebuf_temp.DataBuf[comm3_recebuf_temp.DataLong - 1])))
    {
        if((comm3_recebuf_temp.DataBuf[0] == 0x01) || (comm3_recebuf_temp.DataBuf[0] == 0xFF))
        {
            switch(comm3_recebuf_temp.DataBuf[1])
            {
                case 0x03:
                  starting_target_addr_temp = ((u16)comm3_recebuf_temp.DataBuf[2]) << 8;
                  starting_target_addr_temp |= ((u16)comm3_recebuf_temp.DataBuf[3]);
                  num_of_registers_temp     = ((u16)comm3_recebuf_temp.DataBuf[4]) << 8;
                  num_of_registers_temp     |= ((u16)comm3_recebuf_temp.DataBuf[5]);

                  comm3_sendbuf_temp.DataBuf[0] = comm3_recebuf_temp.DataBuf[0];
                  comm3_sendbuf_temp.DataBuf[1] = comm3_recebuf_temp.DataBuf[1];
                  comm3_sendbuf_temp.DataBuf[2] = num_of_registers_temp << 1;

                  comm3_sendbuf_temp.DataLong    = comm3_sendbuf_temp.DataBuf[2] + 5;
                  comm3_sendbuf_temp.CommChannel = 3;
                  comm3_sendbuf_temp.DataCount   = 0;
                  comm3_sendbuf_temp.OK          = 1;
                  // Copy data(high byte and low byte) from ALL_DATA_RAM
                  for(i=0; i<num_of_registers_temp; i++)
                    {
                        OS_ENTER_CRITICAL();
                        comm3_sendbuf_temp.DataBuf[(i<<1)+3] = (u8)(ALL_DATA_RAM[starting_target_addr_temp+i] >> 8);
                        comm3_sendbuf_temp.DataBuf[(i<<1)+4] = (u8)(ALL_DATA_RAM[starting_target_addr_temp+i]);
                        OS_EXIT_CRITICAL();
                    }

                  data_crc_temp = Get_CRC(&comm3_sendbuf_temp.DataBuf[0], (comm3_sendbuf_temp.DataLong - 2));

                  comm3_sendbuf_temp.DataBuf[comm3_sendbuf_temp.DataLong - 2] = (u8)(data_crc_temp >> 8 & 0x00FF);
                  comm3_sendbuf_temp.DataBuf[comm3_sendbuf_temp.DataLong - 1] = (u8)(data_crc_temp      & 0x00FF);

                  OS_ENTER_CRITICAL();
                  memcpy(&COMM3_SendBuf,&comm3_sendbuf_temp,sizeof(comm3_sendbuf_temp));
                  OS_EXIT_CRITICAL();

                  BSP_COMM_Send(COMM3_SendBuf);
                break;

                case 0x06:
                  starting_target_addr_temp = ((u16)comm3_recebuf_temp.DataBuf[2]) << 8;
                  starting_target_addr_temp |= ((u16)comm3_recebuf_temp.DataBuf[3]);

                  required_value_temp = ((u16)comm3_recebuf_temp.DataBuf[4]) << 8;
                  required_value_temp |= ((u16)comm3_recebuf_temp.DataBuf[5]);

                    pdes = (u16*)(&ALL_DATA_RAM[starting_target_addr_temp]);
                  OS_ENTER_CRITICAL();
                  //ALL_DATA_RAM[starting_target_addr_temp] = required_value_temp;
                  *pdes = required_value_temp;
                  OS_EXIT_CRITICAL();

                  comm3_sendbuf_temp.DataLong    = comm3_recebuf_temp.DataLong;
                  memcpy(&comm3_sendbuf_temp.DataBuf[0], &comm3_recebuf_temp.DataBuf[0], comm3_sendbuf_temp.DataLong);

                  comm3_sendbuf_temp.CommChannel = 3;
                  comm3_sendbuf_temp.DataCount   = 0;
                  comm3_sendbuf_temp.OK          = 1;

                  OS_ENTER_CRITICAL();
                  memcpy(&COMM3_SendBuf,&comm3_sendbuf_temp,sizeof(comm3_sendbuf_temp));
                  OS_EXIT_CRITICAL();

                  BSP_COMM_Send(COMM3_SendBuf);
                break;

                case 0x10:
                  starting_target_addr_temp  = ((u16)comm3_recebuf_temp.DataBuf[2]) << 8;
                  starting_target_addr_temp |= ((u16)comm3_recebuf_temp.DataBuf[3]);
                  num_of_registers_temp      = ((u16)comm3_recebuf_temp.DataBuf[4]) << 8;
                  num_of_registers_temp     |= ((u16)comm3_recebuf_temp.DataBuf[5]);

                  for(i=0; i<num_of_registers_temp; i++)
                    {
                        required_value_temp  = ((u16)comm3_recebuf_temp.DataBuf[(i<<1)+7]) << 8;
                        required_value_temp |= ((u16)comm3_recebuf_temp.DataBuf[(i<<1)+8]);

                        pdes = (u16*)(&ALL_DATA_RAM[starting_target_addr_temp + i]);
                        OS_ENTER_CRITICAL();
                        //ALL_DATA_RAM[starting_target_addr_temp + i] = required_value_temp;
                        *pdes = required_value_temp;
                        OS_EXIT_CRITICAL();
                    }

                  comm3_sendbuf_temp.DataLong    = 8;
                  memcpy(&comm3_sendbuf_temp.DataBuf[0], &comm3_recebuf_temp.DataBuf[0], (comm3_sendbuf_temp.DataLong - 2));

                  comm3_sendbuf_temp.CommChannel = 3;
                  comm3_sendbuf_temp.DataCount   = 0;
                  comm3_sendbuf_temp.OK          = 1;

                  data_crc_temp = Get_CRC(&comm3_sendbuf_temp.DataBuf[0], (comm3_sendbuf_temp.DataLong - 2));

                  comm3_sendbuf_temp.DataBuf[comm3_sendbuf_temp.DataLong - 2] = (u8)(data_crc_temp >> 8 & 0x00FF);
                  comm3_sendbuf_temp.DataBuf[comm3_sendbuf_temp.DataLong - 1] = (u8)(data_crc_temp      & 0x00FF);

                  OS_ENTER_CRITICAL();
                  memcpy(&COMM3_SendBuf,&comm3_sendbuf_temp,sizeof(comm3_sendbuf_temp));
                  OS_EXIT_CRITICAL();

                  BSP_COMM_Send(COMM3_SendBuf);
                break;

            default:
              break;
            }
        }
    }
}

// By chunjie 2018-4-1
void P_Comm1_Send_Handle (void) {
    u8 err;
    u8 time_out_tmp, recv_complete_tmp;
   // comm2_master_complete_recv = 0;
    OS_CPU_SR  cpu_sr = 0;
    MMSenDFuncode03Frame(0x01, 0x00c8, 1);
    OSTmrStart(timer_100ms, &err); //Start timer

    while(!time_out_tmp | !recv_complete_tmp) {
      time_out_tmp = comm1_master_wait_time_out;
      recv_complete_tmp = comm1_master_complete_recv;
    }

    if (recv_complete_tmp) {
      comm1_master_complete_recv = 0;
      printf("Success to Receive.\n");
    }
    else if (time_out_tmp) {
      printf("COMM1 Master send timeout.\n");
    }

    OSTimeDlyHMSM(0,0,1,0);


}

void P_Comm2_Handle(void)
{
    OS_CPU_SR  cpu_sr = 0;

    u8 i = 0;
    u8 sys_number_temp = 0;
    u16 data_crc_temp = 0;
    u16 num_of_registers_temp = 0;
    u16 starting_target_addr_temp = 0;
    u16 required_value_temp = 0;
    static COMM_DATA comm2_recebuf_temp;
    static COMM_DATA comm2_sendbuf_temp;

    OS_ENTER_CRITICAL();
    sys_number_temp = (u8)ALL_DATA_RAM[SYS_NUMBER_BASE];
    memcpy(&comm2_recebuf_temp,&COMM2_ReceBuf,sizeof(COMM2_ReceBuf));
    COMM2_ReceBuf.DataCount = 0;
    COMM2_ReceBuf.DataLong  = 0;
    OS_EXIT_CRITICAL();





    //data_crc_temp = Get_CRC(&comm2_recebuf_temp.DataBuf[0], (comm2_recebuf_temp.DataLong - 2));
    if(1)//data_crc_temp == (u16)(((u16)comm2_recebuf_temp.DataBuf[comm2_recebuf_temp.DataLong - 2] << 8) | (comm2_recebuf_temp.DataBuf[comm2_recebuf_temp.DataLong - 1])))
    {
        if((comm2_recebuf_temp.DataBuf[0] == sys_number_temp) || (comm2_recebuf_temp.DataBuf[0] == 0xFF))
        {
            comm2_sendbuf_temp.DataBuf[0] = 0;
            comm2_sendbuf_temp.DataBuf[1] = 1;
            comm2_sendbuf_temp.DataBuf[2] = 2;
            comm2_sendbuf_temp.DataBuf[3] = 2;

            comm2_sendbuf_temp.DataLong    = 4;
          comm2_sendbuf_temp.CommChannel = 2;
            comm2_sendbuf_temp.DataCount   = 0;
            comm2_sendbuf_temp.OK          = 1;

            OS_ENTER_CRITICAL();
            memcpy(&COMM2_SendBuf,&comm2_sendbuf_temp,sizeof(comm2_sendbuf_temp));
            OS_EXIT_CRITICAL();

            BSP_COMM_Send(COMM2_SendBuf);
            /*
            switch(comm2_recebuf_temp.DataBuf[1])
            {
                case 0x03:
                  starting_target_addr_temp = ((u16)comm2_recebuf_temp.DataBuf[2]) << 8;
                  starting_target_addr_temp |= ((u16)comm2_recebuf_temp.DataBuf[3]);
                  num_of_registers_temp     = ((u16)comm2_recebuf_temp.DataBuf[4]) << 8;
                  num_of_registers_temp     |= ((u16)comm2_recebuf_temp.DataBuf[5]);

                  comm2_sendbuf_temp.DataBuf[0] = comm2_recebuf_temp.DataBuf[0];
                  comm2_sendbuf_temp.DataBuf[1] = comm2_recebuf_temp.DataBuf[1];
                  comm2_sendbuf_temp.DataBuf[2] = num_of_registers_temp << 1;

                  comm2_sendbuf_temp.DataLong    = comm2_sendbuf_temp.DataBuf[2] + 5;
                  comm2_sendbuf_temp.CommChannel = 2;
                  comm2_sendbuf_temp.DataCount   = 0;
                  comm2_sendbuf_temp.OK          = 1;

                  for(i=0; i<num_of_registers_temp; i++)
                    {
                        OS_ENTER_CRITICAL();
                        comm2_sendbuf_temp.DataBuf[(i<<1)+3] = (u8)(ALL_DATA_RAM[starting_target_addr_temp+i] >> 8);
                        comm2_sendbuf_temp.DataBuf[(i<<1)+4] = (u8)(ALL_DATA_RAM[starting_target_addr_temp+i]);
                        OS_EXIT_CRITICAL();
                    }

                  data_crc_temp = Get_CRC(&comm2_sendbuf_temp.DataBuf[0],(comm2_sendbuf_temp.DataLong - 2));

                  comm2_sendbuf_temp.DataBuf[comm2_sendbuf_temp.DataLong - 2] = (u8)(data_crc_temp >> 8 & 0x00FF);
                  comm2_sendbuf_temp.DataBuf[comm2_sendbuf_temp.DataLong - 1] = (u8)(data_crc_temp      & 0x00FF);

                  OS_ENTER_CRITICAL();
                  memcpy(&COMM2_SendBuf,&comm2_sendbuf_temp,sizeof(comm2_sendbuf_temp));
                  OS_EXIT_CRITICAL();

                  BSP_COMM_Send(COMM2_SendBuf);
                break;

                case 0x06:
                  starting_target_addr_temp = ((u16)comm2_recebuf_temp.DataBuf[2]) << 8;
                  starting_target_addr_temp |= ((u16)comm2_recebuf_temp.DataBuf[3]);

                  required_value_temp = ((u16)comm2_recebuf_temp.DataBuf[4]) << 8;
                  required_value_temp |= ((u16)comm2_recebuf_temp.DataBuf[5]);

                  OS_ENTER_CRITICAL();
                  ALL_DATA_RAM[starting_target_addr_temp] = required_value_temp;
                  OS_EXIT_CRITICAL();

                  comm2_sendbuf_temp.DataLong    = comm2_recebuf_temp.DataLong;
                  memcpy(&comm2_sendbuf_temp.DataBuf[0], &comm2_recebuf_temp.DataBuf[0], comm2_sendbuf_temp.DataLong);

                  comm2_sendbuf_temp.CommChannel = 2;
                  comm2_sendbuf_temp.DataCount   = 0;
                  comm2_sendbuf_temp.OK          = 1;

                  OS_ENTER_CRITICAL();
                  memcpy(&COMM2_SendBuf,&comm2_sendbuf_temp,sizeof(comm2_sendbuf_temp));
                  OS_EXIT_CRITICAL();

                  BSP_COMM_Send(COMM2_SendBuf);
                break;

                case 0x10:
                  starting_target_addr_temp  = ((u16)comm2_recebuf_temp.DataBuf[2]) << 8;
                  starting_target_addr_temp |= ((u16)comm2_recebuf_temp.DataBuf[3]);
                  num_of_registers_temp      = ((u16)comm2_recebuf_temp.DataBuf[4]) << 8;
                  num_of_registers_temp     |= ((u16)comm2_recebuf_temp.DataBuf[5]);

                  for(i=0; i<num_of_registers_temp; i++)
                    {
                        required_value_temp  = ((u16)comm2_recebuf_temp.DataBuf[(i<<1)+7]) << 8;
                        required_value_temp |= ((u16)comm2_recebuf_temp.DataBuf[(i<<1)+8]);

                        OS_ENTER_CRITICAL();
                        ALL_DATA_RAM[starting_target_addr_temp + i] = required_value_temp;
                        OS_EXIT_CRITICAL();
                    }

                  comm2_sendbuf_temp.DataLong    = 8;
                  memcpy(&comm2_sendbuf_temp.DataBuf[0], &comm2_recebuf_temp.DataBuf[0], (comm2_sendbuf_temp.DataLong - 2));

                  comm2_sendbuf_temp.CommChannel = 2;
                  comm2_sendbuf_temp.DataCount   = 0;
                  comm2_sendbuf_temp.OK          = 1;

                  data_crc_temp = Get_CRC(&comm2_sendbuf_temp.DataBuf[0], (comm2_sendbuf_temp.DataLong - 2));

                  comm2_sendbuf_temp.DataBuf[comm2_sendbuf_temp.DataLong - 2] = (u8)(data_crc_temp >> 8 & 0x00FF);
                  comm2_sendbuf_temp.DataBuf[comm2_sendbuf_temp.DataLong - 1] = (u8)(data_crc_temp      & 0x00FF);

                  OS_ENTER_CRITICAL();
                  memcpy(&COMM2_SendBuf,&comm2_sendbuf_temp,sizeof(comm2_sendbuf_temp));
                  OS_EXIT_CRITICAL();

                  BSP_COMM_Send(COMM2_SendBuf);
                break;

            default:
              break;
            }
            */
        }
    }
}

void P_Comm1_Handle(void)
{
    OS_CPU_SR  cpu_sr = 0;
    u8 err;
    u8  i = 0;
    u8  sys_number_temp = 0;
    u16 data_crc_temp = 0;
    u16 num_of_registers_temp = 0;
    u16 starting_target_addr_temp = 0;
    u16 required_value_temp = 0;
    static COMM_DATA comm1_recebuf_temp;
    static COMM_DATA comm1_sendbuf_temp;

    OS_ENTER_CRITICAL();
    sys_number_temp = (u8)ALL_DATA_RAM[SYS_NUMBER_BASE];
    memcpy(&comm1_recebuf_temp,&COMM1_ReceBuf,sizeof(COMM1_ReceBuf));
    COMM1_ReceBuf.DataBuf[0] = 0x00;
    COMM1_ReceBuf.DataBuf[1] = 0x00;
    COMM1_ReceBuf.DataBuf[2] = 0x00;
    COMM1_ReceBuf.DataBuf[3] = 0x00;
    COMM1_ReceBuf.DataBuf[4] = 0x00;
    COMM1_ReceBuf.DataCount = 0;
    COMM1_ReceBuf.DataLong  = 0;
    OS_EXIT_CRITICAL();
    OSTmrStart(timer_100ms, &err); //Start timer
    data_crc_temp = Get_CRC(&comm1_recebuf_temp.DataBuf[0], (comm1_recebuf_temp.DataLong - 2));
    if(data_crc_temp == (u16)(((u16)comm1_recebuf_temp.DataBuf[comm1_recebuf_temp.DataLong - 2] << 8) | (comm1_recebuf_temp.DataBuf[comm1_recebuf_temp.DataLong - 1])))
    {
        if((comm1_recebuf_temp.DataBuf[0] == sys_number_temp) || (comm1_recebuf_temp.DataBuf[0] == 0xFF))
        {
            switch(comm1_recebuf_temp.DataBuf[1])
            {
                case 0x03:
                  starting_target_addr_temp = ((u16)comm1_recebuf_temp.DataBuf[2]) << 8;
                  starting_target_addr_temp |= ((u16)comm1_recebuf_temp.DataBuf[3]);
                  num_of_registers_temp     = ((u16)comm1_recebuf_temp.DataBuf[4]) << 8;
                  num_of_registers_temp     |= ((u16)comm1_recebuf_temp.DataBuf[5]);

                  comm1_sendbuf_temp.DataBuf[0] = comm1_recebuf_temp.DataBuf[0]; // adress
                  comm1_sendbuf_temp.DataBuf[1] = comm1_recebuf_temp.DataBuf[1]; // func code
                  comm1_sendbuf_temp.DataBuf[2] = num_of_registers_temp << 1;   //

                  comm1_sendbuf_temp.DataLong    = comm1_sendbuf_temp.DataBuf[2] + 5;
                  comm1_sendbuf_temp.CommChannel = 1;
                  comm1_sendbuf_temp.DataCount   = 0;
                  comm1_sendbuf_temp.OK          = 1;

                  for(i=0; i<num_of_registers_temp; i++)
                    {
                        OS_ENTER_CRITICAL();
                        comm1_sendbuf_temp.DataBuf[(i<<1)+3] = (u8)(ALL_DATA_RAM[starting_target_addr_temp+i] >> 8);
                        comm1_sendbuf_temp.DataBuf[(i<<1)+4] = (u8)(ALL_DATA_RAM[starting_target_addr_temp+i]);
                        OS_EXIT_CRITICAL();
                    }

                  data_crc_temp = Get_CRC(&comm1_sendbuf_temp.DataBuf[0], (comm1_sendbuf_temp.DataLong - 2));

                  comm1_sendbuf_temp.DataBuf[comm1_sendbuf_temp.DataLong - 2] = (u8)(data_crc_temp >> 8 & 0x00FF);
                  comm1_sendbuf_temp.DataBuf[comm1_sendbuf_temp.DataLong - 1] = (u8)(data_crc_temp      & 0x00FF);

                  OS_ENTER_CRITICAL();
                  memcpy(&COMM1_SendBuf,&comm1_sendbuf_temp,sizeof(comm1_sendbuf_temp));
                  OS_EXIT_CRITICAL();

                  BSP_COMM_Send(COMM1_SendBuf);
                break;

                case 0x06:
                  starting_target_addr_temp = ((u16)comm1_recebuf_temp.DataBuf[2]) << 8;
                  starting_target_addr_temp |= ((u16)comm1_recebuf_temp.DataBuf[3]);

                  required_value_temp = ((u16)comm1_recebuf_temp.DataBuf[4]) << 8;
                  required_value_temp |= ((u16)comm1_recebuf_temp.DataBuf[5]);

                  OS_ENTER_CRITICAL();
                  ALL_DATA_RAM[starting_target_addr_temp] = required_value_temp;
                  OS_EXIT_CRITICAL();

                  comm1_sendbuf_temp.DataLong    = comm1_recebuf_temp.DataLong;
                  memcpy(&comm1_sendbuf_temp.DataBuf[0], &comm1_recebuf_temp.DataBuf[0], comm1_sendbuf_temp.DataLong);

                  comm1_sendbuf_temp.CommChannel = 1;
                  comm1_sendbuf_temp.DataCount   = 0;
                  comm1_sendbuf_temp.OK          = 1;

                  OS_ENTER_CRITICAL();
                  memcpy(&COMM1_SendBuf,&comm1_sendbuf_temp,sizeof(comm1_sendbuf_temp));
                  OS_EXIT_CRITICAL();

                  BSP_COMM_Send(COMM1_SendBuf);
                break;

                case 0x10:
                  starting_target_addr_temp  = ((u16)comm1_recebuf_temp.DataBuf[2]) << 8;
                  starting_target_addr_temp |= ((u16)comm1_recebuf_temp.DataBuf[3]);
                  num_of_registers_temp      = ((u16)comm1_recebuf_temp.DataBuf[4]) << 8;
                  num_of_registers_temp     |= ((u16)comm1_recebuf_temp.DataBuf[5]);

                  for(i=0; i<num_of_registers_temp; i++)
                    {
                        required_value_temp  = ((u16)comm1_recebuf_temp.DataBuf[(i<<1)+7]) << 8;
                        required_value_temp |= ((u16)comm1_recebuf_temp.DataBuf[(i<<1)+8]);

                        OS_ENTER_CRITICAL();
                        ALL_DATA_RAM[starting_target_addr_temp + i] = required_value_temp;
                        OS_EXIT_CRITICAL();
                    }

                  comm1_sendbuf_temp.DataLong    = 8;
                  memcpy(&comm1_sendbuf_temp.DataBuf[0], &comm1_recebuf_temp.DataBuf[0], (comm1_sendbuf_temp.DataLong - 2));

                  comm1_sendbuf_temp.CommChannel = 1;
                  comm1_sendbuf_temp.DataCount   = 0;
                  comm1_sendbuf_temp.OK          = 1;

                  data_crc_temp = Get_CRC(&comm1_sendbuf_temp.DataBuf[0], (comm1_sendbuf_temp.DataLong - 2));

                  comm1_sendbuf_temp.DataBuf[comm1_sendbuf_temp.DataLong - 2] = (u8)(data_crc_temp >> 8 & 0x00FF);
                  comm1_sendbuf_temp.DataBuf[comm1_sendbuf_temp.DataLong - 1] = (u8)(data_crc_temp      & 0x00FF);

                  OS_ENTER_CRITICAL();
                  memcpy(&COMM1_SendBuf,&comm1_sendbuf_temp,sizeof(comm1_sendbuf_temp));
                  OS_EXIT_CRITICAL();

                  BSP_COMM_Send(COMM1_SendBuf);
                break;

                default:
                  break;
            }
        }
    }
}



 /*--------------------------------------------------------------------
函数名称：SenDFuncode03Frame()
函数功能：
注意事项：
提示说明：
输    入：SlaveAddress,RegStar_Address,RegLen
返    回
--------------------------------------------------------------------*/
 void MMSenDFuncode03Frame(u8 SlaveAddress,uint16_t RegStar_Address,u16 RegLen)
 {      OS_CPU_SR  cpu_sr = 0;
        u16 data_crc_temp = 0;
        COMM_DATA comm1_fun03_sendbuf;
        //Master.ModbusAdd=SlaveAddress;

        //Master.SendDataBuf[0] = SlaveAddress;  //
        //Master.SendDataBuf[1] = 0x03;     //
        //Master.SendDataBuf[2] =((RegStar_Address & 0xff00)>>8);//
        //Master.SendDataBuf[3] =(RegStar_Address & 0x00ff); //
        //Master.SendDataBuf[4] =((RegLen & 0xff00)>>8);     //
        //Master.SendDataBuf[5] =(RegLen & 0x00ff);     //

        comm1_fun03_sendbuf.DataBuf[0] = SlaveAddress;
        comm1_fun03_sendbuf.DataBuf[1] = 0x06;
        //起始地址
        comm1_fun03_sendbuf.DataBuf[2] =((RegStar_Address & 0xff00)>>8);//
        comm1_fun03_sendbuf.DataBuf[3] =(RegStar_Address & 0x00ff); //
        // 寄存器数量
        comm1_fun03_sendbuf.DataBuf[4] =((RegLen & 0xff00)>>8);     //
        comm1_fun03_sendbuf.DataBuf[5] =(RegLen & 0x00ff);     //

        // What is the length incluing CRC
        comm1_fun03_sendbuf.DataLong    = 8;
        comm1_fun03_sendbuf.CommChannel = 1;
        comm1_fun03_sendbuf.DataCount   = 0;
        comm1_fun03_sendbuf.OK          = 1;


        data_crc_temp = Get_CRC(&comm1_fun03_sendbuf.DataBuf[0], (comm1_fun03_sendbuf.DataLong - 2));

        comm1_fun03_sendbuf.DataBuf[comm1_fun03_sendbuf.DataLong - 2] = (u8)(data_crc_temp >> 8 & 0x00FF);
        comm1_fun03_sendbuf.DataBuf[comm1_fun03_sendbuf.DataLong - 1] = (u8)(data_crc_temp      & 0x00FF);

        OS_ENTER_CRITICAL();
        memcpy(&COMM1_SendBuf,&comm1_fun03_sendbuf,sizeof(comm1_fun03_sendbuf));
        OS_EXIT_CRITICAL();

        BSP_COMM_Send(COMM1_SendBuf);

 }
  /*--------------------------------------------------------------------


--------------------------------------------------------------------*/


//-------------------------------------------------------------------
void MasterReadSlave(void) {
  u8 err;
  MMSenDFuncode03Frame(0x30, 0x00, 2);
//  OSTmrStart(timer_100ms, &err); //Start timer

  // if time-out and not rev
  // jumpt out
}


