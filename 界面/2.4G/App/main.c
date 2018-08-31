/*
***************************************************************************
*    ģ�飺Test_TouchPad 
*    ������TouchPad Ӧ�ò���
*    ���ߣ�Huo
*    ʱ�䣺2017.12.13
*    �汾��UP-Magic-Version 1.0.0
***************************************************************************
*/
//#include "stm32f4xx.h"
#include "BSP_DebugUART.h"
#include "BSP_Delay.h"
#include "Queue.h"
#include "BSP_MyIIC.h"
#include "BSP_GSL268x.h"
#include "BSP_Timer.h"
#include "BSP_TouchPad.h"
#include "BSP_ParallelBus.h"
#include "BSP_RA8875_Port.h"
#include "BSP_RA8875.h"
#include "fonts.h"
#include "BSP_TFTLCD.h"
//#include "StartBMP.h"  //�״ΰ������Ժ���Բ��ٰ�����
#include "BSP_SRAM.h"
#include "MainTask.h"
#include "BSP_UART.h"
#include "BSP_ST16C554.h"

#include "Test_emWin.h"

/* ������ */
int main(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	/* ���ȼ���������Ϊ 4����ʹ��Ĭ�Ϸ��鷽�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	BSP_DebugUART_Init(115200);
	BSP_Delay_Init();
	BSP_MyIIC_Init();
	BSP_GSL268x_Init();			//��������ʼ������ʵ��Ĭ��ʹ�ö�ʱɨ��
	BSP_PBus_Init();
	BSP_TFTLCD_Init();	/* ��ʼ����ʾ��Ӳ��(����GPIO��FSMC,��LCD���ͳ�ʼ��ָ��) */
	BSP_SRAM_Init();
	BSP_UART_Init(115200);
	BSP_ST16C554_Init();
	BSP_Timer_Init(1*1000);	//��ʱ 1*1000us = 1ms
		
	printf("         \t ���������� ��ʾ \r\n"
        "----------------------------------------------------\r\n"
        "\t��ӡ�������ϵĴ���λ����Ϣ \n\r"
        "----------------------------------------------------\n\r"
			);
	
	//Test_TouchPad();
	//Test_TFTLCD();
	
	MainTask();
}
