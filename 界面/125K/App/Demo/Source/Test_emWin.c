/*
***************************************************************************
*    ģ�飺LF_125K 
*    ��������Ƶ125Kģ�� Ӧ�ò���
*    ���ߣ�Zyan
*    ʱ�䣺2018.06.01
*    �汾��
***************************************************************************
*/
#include "Test_emWin.h"

/*
***************************************************************************
*	�� �� ��: Test_TouchPad
*	����˵��: TouchPad Ӧ�ú���
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void Test_TouchPad(void)
{
	while(1)
	{
		/* ����ʹ�� ��ʱ��ɨ�裡*/
//		if(!BSP_TouchPad_Callback())
//		{
//			if(BSP_TouchPad_MoveValid(PENX,PENY,PENX_PRE,PENY_PRE))
//				printf("��Ч��λ��\n");
//			else
//				printf("��Ч��λ��\n");
//		}
	}
}

void Test_TFTLCD(void)
{
	uint16_t x,y;
	FONT_T tFont;			/* ����һ������ṹ���������������������� */

	/* ����LCD������� */
	//#define VER_INFO	"�������� ħ��ʦ���⾺��ƽ̨ STM32������(V1.0h)"
	char VER_INFO[] =	"�������� ħ��ʦ���⾺��ƽ̨ STM32������(V1.0h)";

	LCD_ClrScr(CL_WHITE);	/* ��������ʾȫ�� */
	LCD_DrawLine(0,240,800,240,CL_BLACK);
	LCD_DrawLine(400,0,400,480,CL_BLACK);
	LCD_DrawBMP(0,0,240,400,(uint16_t*)0x0800F000);
	for(y=0;y<240;y++)
		for(x=0;x<400;x++)
		{
			LCD_PutPixel(x,y+240,LCD_GetPixel(x,y));
		}
//	LCD_DrawRect(450,60,100,120,CL_BLACK);
//	LCD_Fill_Rect(650,60,100,120,CL_BLACK);
	LCD_DrawRoundRect(450,60,100,120,15,CL_BLACK);
	LCD_FillRoundRect(650,60,100,120,15,CL_BLACK);
	LCD_DrawCircle(500,360,60,CL_BLACK);
	LCD_FillCircle(700,360,60,CL_BLACK);
	
	/* ����������� */
	{
		tFont.FontCode = FC_ST_16;		/* ������� 16���� */
		tFont.FrontColor = CL_WHITE;	/* ������ɫ */
		tFont.BackColor = CL_RED;		/* ���ֱ�����ɫ */
		tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
	}
	LCD_DispStr(400, 0, VER_INFO, &tFont);	/* ��ʾ����汾��Ϣ */

	while(1)
	{
		Test_SRAM();
	}
}

void Test_SRAM(void)
{
	uint32_t Count,ErrCount=0;
	uint16_t Buffer,Temp;
	
	printf("SRAM Test\r\n");
	while (1)
	{
		/* д�� 0xFFFF �ٶ�����У�� */
		Buffer=0xFFFF;
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_WriteBuffer(&Buffer,0x0000,1);
		}
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_ReadBuffer(&Temp,0x0000,1);
			if(Temp != 0xFFFF)
				ErrCount++;
		}

		/* д�� 0xFF00 �ٶ�����У�� */
		Buffer=0xFF00;
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_WriteBuffer(&Buffer,0x0000,1);
		}
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_ReadBuffer(&Temp,0x0000,1);
			if(Temp != 0xFF00)
				ErrCount++;
		}

		/* д�� 0x00FF �ٶ�����У�� */
		Buffer=0x00FF;
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_WriteBuffer(&Buffer,0x0000,1);
		}
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_ReadBuffer(&Temp,0x0000,1);
			if(Temp != 0x00FF)
				ErrCount++;
		}

		/* д�� 0x0000 �ٶ�����У�� */
		Buffer=0x0000;
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_WriteBuffer(&Buffer,0x0000,1);
		}
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_ReadBuffer(&Temp,0x0000,1);
			if(Temp != 0x0000)
				ErrCount++;
		}
		
		/* ������Խ�� */
		if(ErrCount)
			printf("Error!!! ErrCount=%d \r\n",ErrCount);
		else
			printf("Success!!! \r\n");
	}
}

