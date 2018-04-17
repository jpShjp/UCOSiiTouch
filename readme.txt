全部移植到3.5库中，几个关键点：
1.os_cfg_r.h中的#define OS_TICKS_PER_SEC       1000
2.stm32f10x_it.c中，所有的中断函数都要在这儿定义，包括USART3_IRQHandler、USART2_IRQHandler、USART1_IRQHandler这三个中断函数。
3.control_main函数中，P_Comm3，2,1_Handle()这个函数不要删掉，这有请求信号量的过程。
4.1-80号版本：ALL_DATA_RAM[ADG1611_ACTIVE_LEVEL] = 1; // default is active high
  80号之后:   ALL_DATA_RAM[ADG1611_ACTIVE_LEVEL] = 0; // default is active low
5.comtrol_global.c里面：ALL_DATA_RAM[OUT1_TYPE_BASE] = 2;设置成电压模式。
6.触摸屏的AO寄存器地址。DAC_FINAL_BASE和DAC_ORIGIN_BASE对掉一下。
7.为了和触摸屏程序保持一致，波特率改为了115200.
