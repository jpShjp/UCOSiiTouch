ȫ����ֲ��3.5���У������ؼ��㣺
1.os_cfg_r.h�е�#define OS_TICKS_PER_SEC       1000
2.stm32f10x_it.c�У����е��жϺ�����Ҫ��������壬����USART3_IRQHandler��USART2_IRQHandler��USART1_IRQHandler�������жϺ�����
3.control_main�����У�P_Comm3��2,1_Handle()���������Ҫɾ�������������ź����Ĺ��̡�
4.1-80�Ű汾��ALL_DATA_RAM[ADG1611_ACTIVE_LEVEL] = 1; // default is active high
  80��֮��:   ALL_DATA_RAM[ADG1611_ACTIVE_LEVEL] = 0; // default is active low
5.comtrol_global.c���棺ALL_DATA_RAM[OUT1_TYPE_BASE] = 2;���óɵ�ѹģʽ��
6.��������AO�Ĵ�����ַ��DAC_FINAL_BASE��DAC_ORIGIN_BASE�Ե�һ�¡�
7.Ϊ�˺ʹ��������򱣳�һ�£������ʸ�Ϊ��115200.
