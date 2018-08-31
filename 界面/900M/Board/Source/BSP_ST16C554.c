/*
***************************************************************************
*    ģ�飺BSP_ST16C554
*    �������弶 ST16C554 ����ģ������
		   INT1A-PF6  INT1B-PF7  INT1C-PF8  INT1D-PF9
           INT2A-PA1  INT2B-PF10 INT2C-PA0  INT2D-PC4
*    ���ߣ�Huo
*    ʱ�䣺2018.03.07
*    �汾��V1.0.0
***************************************************************************
*/
#include "BSP_ST16C554.h"

#define RC632_STA_SOP    0x01
#define RC632_STA_LEN     0x02
#define RC632_STA_DAT     0x03
#define RC632_STA_EOF     0x04

extern uint8_t drbuf[105];
extern uint8_t rxflag;
extern uint8_t len;
extern uint8_t dataLen;
extern int recSta;

/* �ڲ��������� */
static void BSP_CS1A_GPIO_Init(void);
static void BSP_CS1A_EXTI_Init(void);
static void BSP_CS1A_NVIC_Init(void);
static void BSP_ST16C554_CS1A_Init(void);
static void BSP_CS1B_GPIO_Init(void);
static void BSP_CS1B_EXTI_Init(void);
static void BSP_CS1B_NVIC_Init(void);
static void BSP_ST16C554_CS1B_Init(void);
static void BSP_CS1C_GPIO_Init(void);
static void BSP_CS1C_EXTI_Init(void);
static void BSP_CS1C_NVIC_Init(void);
static void BSP_ST16C554_CS1C_Init(void);
static void BSP_CS1D_GPIO_Init(void);
static void BSP_CS1D_EXTI_Init(void);
static void BSP_CS1D_NVIC_Init(void);
static void BSP_ST16C554_CS1D_Init(void);
static void BSP_CS2A_GPIO_Init(void);
static void BSP_CS2A_EXTI_Init(void);
static void BSP_CS2A_NVIC_Init(void);
static void BSP_ST16C554_CS2A_Init(void);
static void BSP_CS2B_GPIO_Init(void);
static void BSP_CS2B_EXTI_Init(void);
static void BSP_CS2B_NVIC_Init(void);
static void BSP_ST16C554_CS2B_Init(void);
static void BSP_CS2C_GPIO_Init(void);
static void BSP_CS2C_EXTI_Init(void);
static void BSP_CS2C_NVIC_Init(void);
static void BSP_ST16C554_CS2C_Init(void);
static void BSP_CS2D_GPIO_Init(void);
static void BSP_CS2D_EXTI_Init(void);
static void BSP_CS2D_NVIC_Init(void);
static void BSP_ST16C554_CS2D_Init(void);

/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_Init
*	����˵��: �弶 ST16C554 ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_ST16C554_Init(void)
{
    BSP_ST16C554_CS1A_Init();
    BSP_ST16C554_CS1B_Init();
    BSP_ST16C554_CS1C_Init();
    BSP_ST16C554_CS1D_Init();
    BSP_ST16C554_CS2A_Init();
    BSP_ST16C554_CS2B_Init();
    BSP_ST16C554_CS2C_Init();
    BSP_ST16C554_CS2D_Init();
}

/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS1A_Init
*	����˵��: �弶 ST16C554 CS1A �ڲ���ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_ST16C554_CS1A_Init(void)
{
    //Registers Value
    uint16_t IER_VAL=ST16C554_IER_VAL;
    uint16_t LCR_DIVVAL=ST16C554_LCR_DIVVAL, LCR_DATVAL=ST16C554_LCR_DATVAL;
    uint16_t DLL_VAL=ST16C554_DLL_VAL, DLM_VAL=ST16C554_DLM_VAL;	//115200

    BSP_CS1A_GPIO_Init();
    BSP_CS1A_EXTI_Init();
    BSP_CS1A_NVIC_Init();

    BSP_PBus_WriteBuffer(&LCR_DIVVAL, (ST16C554_CS1A_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);	//LCR addr:011b<<1
    BSP_PBus_WriteBuffer(&DLL_VAL, (ST16C554_CS1A_ADD+(ST16C554_REG_DLL<<1))<<1, 0x01);		//DLL addr:000b<<1
    BSP_PBus_WriteBuffer(&DLM_VAL, (ST16C554_CS1A_ADD+(ST16C554_REG_DLM<<1))<<1, 0x01);		//DLM addr:001b<<1
    BSP_PBus_WriteBuffer(&LCR_DATVAL, (ST16C554_CS1A_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);
    BSP_PBus_WriteBuffer(&IER_VAL, (ST16C554_CS1A_ADD+(ST16C554_REG_IER<<1))<<1, 0x01);		//IER addr:001b<<1
}
/* GPIO��ʼ������ */
static void BSP_CS1A_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS1A_EXTI_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);	//ʹ�ܸ��ù���ʱ��

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource6);
    EXTI_InitStructure.EXTI_Line=EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS1A_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    /* ��ռ���ȼ����ã����ȼ�����Ϊ 4 ������£���ռ���ȼ������÷�Χ 0-15 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 12;
    /* �����ȼ����ã����ȼ�����Ϊ 4 ������£������ȼ���Ч��ȡ��ֵ 0 ���� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS1A
*	����˵��: �弶 ST16C554 CS1A ���ܺ���
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_ST16C554_CS1A_ISR(void)
{
    if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_6))
    {
        EXTI_ClearITPendingBit(EXTI_Line6);
        USART_SendData(USART1,BSP_ST16C554_CS1A_Read());
    }
}
uint16_t BSP_ST16C554_CS1A_Read(void)
{
    uint16_t data;
    BSP_PBus_ReadBuffer(&data, (ST16C554_CS1A_ADD+(ST16C554_REG_RHR<<1))<<1, 1);
    return data;
}
void BSP_ST16C554_CS1A_Write(uint16_t BSP_ST16C554_CS1A_Val)
{
    BSP_PBus_WriteBuffer(&BSP_ST16C554_CS1A_Val, (ST16C554_CS1A_ADD+(ST16C554_REG_THR<<1))<<1, 1);
}


/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS1B_Init
*	����˵��: �弶 ST16C554 CS1B �ڲ���ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_ST16C554_CS1B_Init(void)
{
    //Registers Value
    uint16_t IER_VAL=ST16C554_IER_VAL;
    uint16_t LCR_DIVVAL=ST16C554_LCR_DIVVAL, LCR_DATVAL=ST16C554_LCR_DATVAL;
    uint16_t DLL_VAL=ST16C554_DLL_VAL, DLM_VAL=ST16C554_DLM_VAL;	//115200

    BSP_CS1B_GPIO_Init();
    BSP_CS1B_EXTI_Init();
    BSP_CS1B_NVIC_Init();

    BSP_PBus_WriteBuffer(&LCR_DIVVAL, (ST16C554_CS1B_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);	//LCR addr:011b<<1
    BSP_PBus_WriteBuffer(&DLL_VAL, (ST16C554_CS1B_ADD+(ST16C554_REG_DLL<<1))<<1, 0x01);		//DLL addr:000b<<1
    BSP_PBus_WriteBuffer(&DLM_VAL, (ST16C554_CS1B_ADD+(ST16C554_REG_DLM<<1))<<1, 0x01);		//DLM addr:001b<<1
    BSP_PBus_WriteBuffer(&LCR_DATVAL, (ST16C554_CS1B_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);
    BSP_PBus_WriteBuffer(&IER_VAL, (ST16C554_CS1B_ADD+(ST16C554_REG_IER<<1))<<1, 0x01);		//IER addr:001b<<1
}
/* GPIO��ʼ������ */
static void BSP_CS1B_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS1B_EXTI_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);	//ʹ�ܸ��ù���ʱ��

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource7);
    EXTI_InitStructure.EXTI_Line=EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS1B_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    /* ��ռ���ȼ����ã����ȼ�����Ϊ 4 ������£���ռ���ȼ������÷�Χ 0-15 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 12;
    /* �����ȼ����ã����ȼ�����Ϊ 4 ������£������ȼ���Ч��ȡ��ֵ 0 ���� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS1B
*	����˵��: �弶 ST16C554 CS1B ���ܺ���
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_ST16C554_CS1B_ISR(void)
{

}
uint16_t BSP_ST16C554_CS1B_Read(void)
{
    uint16_t data;
    BSP_PBus_ReadBuffer(&data, (ST16C554_CS1B_ADD+(ST16C554_REG_RHR<<1))<<1, 1);
    return data;
}
void BSP_ST16C554_CS1B_Write(uint16_t BSP_ST16C554_CS1B_Val)
{
    BSP_PBus_WriteBuffer(&BSP_ST16C554_CS1B_Val, (ST16C554_CS1B_ADD+(ST16C554_REG_THR<<1))<<1, 1);
}


/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS1C_Init
*	����˵��: �弶 ST16C554 CS1C �ڲ���ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_ST16C554_CS1C_Init(void)
{
    //Registers Value
    uint16_t IER_VAL=ST16C554_IER_VAL;
    uint16_t LCR_DIVVAL=ST16C554_LCR_DIVVAL, LCR_DATVAL=ST16C554_LCR_DATVAL;
    uint16_t DLL_VAL=ST16C554_DLL_VAL, DLM_VAL=ST16C554_DLM_VAL;	//115200

    BSP_CS1C_GPIO_Init();
    BSP_CS1C_EXTI_Init();
    BSP_CS1C_NVIC_Init();

    BSP_PBus_WriteBuffer(&LCR_DIVVAL, (ST16C554_CS1C_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);	//LCR addr:011b<<1
    BSP_PBus_WriteBuffer(&DLL_VAL, (ST16C554_CS1C_ADD+(ST16C554_REG_DLL<<1))<<1, 0x01);		//DLL addr:000b<<1
    BSP_PBus_WriteBuffer(&DLM_VAL, (ST16C554_CS1C_ADD+(ST16C554_REG_DLM<<1))<<1, 0x01);		//DLM addr:001b<<1
    BSP_PBus_WriteBuffer(&LCR_DATVAL, (ST16C554_CS1C_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);
    BSP_PBus_WriteBuffer(&IER_VAL, (ST16C554_CS1C_ADD+(ST16C554_REG_IER<<1))<<1, 0x01);		//IER addr:001b<<1
}
/* GPIO��ʼ������ */
static void BSP_CS1C_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS1C_EXTI_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);	//ʹ�ܸ��ù���ʱ��

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource8);
    EXTI_InitStructure.EXTI_Line=EXTI_Line8;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS1C_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    /* ��ռ���ȼ����ã����ȼ�����Ϊ 4 ������£���ռ���ȼ������÷�Χ 0-15 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 12;
    /* �����ȼ����ã����ȼ�����Ϊ 4 ������£������ȼ���Ч��ȡ��ֵ 0 ���� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS1C
*	����˵��: �弶 ST16C554 CS1C ���ܺ���
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_ST16C554_CS1C_ISR(void)
{
    if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8))
    {
        EXTI_ClearITPendingBit(EXTI_Line8);
        USART_SendData(USART1,BSP_ST16C554_CS1C_Read());
    }
}
uint16_t BSP_ST16C554_CS1C_Read(void)
{
    uint16_t data;
    BSP_PBus_ReadBuffer(&data, (ST16C554_CS1C_ADD+(ST16C554_REG_RHR<<1))<<1, 1);
    return data;
}
void BSP_ST16C554_CS1C_Write(uint16_t BSP_ST16C554_CS1C_Val)
{
    BSP_PBus_WriteBuffer(&BSP_ST16C554_CS1C_Val, (ST16C554_CS1C_ADD+(ST16C554_REG_THR<<1))<<1, 1);
}


/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS1D_Init
*	����˵��: �弶 ST16C554 CS1D �ڲ���ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_ST16C554_CS1D_Init(void)
{
    //Registers Value
    uint16_t IER_VAL=ST16C554_IER_VAL;
    uint16_t LCR_DIVVAL=ST16C554_LCR_DIVVAL, LCR_DATVAL=ST16C554_LCR_DATVAL;
    uint16_t DLL_VAL=ST16C554_DLL_VAL, DLM_VAL=ST16C554_DLM_VAL;	//115200

    BSP_CS1D_GPIO_Init();
    BSP_CS1D_EXTI_Init();
    BSP_CS1D_NVIC_Init();

    BSP_PBus_WriteBuffer(&LCR_DIVVAL, (ST16C554_CS1D_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);	//LCR addr:011b<<1
    BSP_PBus_WriteBuffer(&DLL_VAL, (ST16C554_CS1D_ADD+(ST16C554_REG_DLL<<1))<<1, 0x01);		//DLL addr:000b<<1
    BSP_PBus_WriteBuffer(&DLM_VAL, (ST16C554_CS1D_ADD+(ST16C554_REG_DLM<<1))<<1, 0x01);		//DLM addr:001b<<1
    BSP_PBus_WriteBuffer(&LCR_DATVAL, (ST16C554_CS1D_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);
    BSP_PBus_WriteBuffer(&IER_VAL, (ST16C554_CS1D_ADD+(ST16C554_REG_IER<<1))<<1, 0x01);		//IER addr:001b<<1
}
/* GPIO��ʼ������ */
static void BSP_CS1D_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS1D_EXTI_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);	//ʹ�ܸ��ù���ʱ��

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource9);
    EXTI_InitStructure.EXTI_Line=EXTI_Line9;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS1D_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    /* ��ռ���ȼ����ã����ȼ�����Ϊ 4 ������£���ռ���ȼ������÷�Χ 0-15 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 12;
    /* �����ȼ����ã����ȼ�����Ϊ 4 ������£������ȼ���Ч��ȡ��ֵ 0 ���� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS1D
*	����˵��: �弶 ST16C554 CS1D ���ܺ���
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_ST16C554_CS1D_ISR(void)
{
    if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9))
    {
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}

uint16_t BSP_ST16C554_CS1D_Read(void)
{
    uint16_t data;
    BSP_PBus_ReadBuffer(&data, (ST16C554_CS1D_ADD+(ST16C554_REG_RHR<<1))<<1, 1);
    return data;
}
void BSP_ST16C554_CS1D_Write(uint16_t BSP_ST16C554_CS1D_Val)
{
    BSP_PBus_WriteBuffer(&BSP_ST16C554_CS1D_Val, (ST16C554_CS1D_ADD+(ST16C554_REG_THR<<1))<<1, 1);
}


/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS2A_Init
*	����˵��: �弶 ST16C554 CS2A �ڲ���ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_ST16C554_CS2A_Init(void)
{
    //Registers Value
    uint16_t IER_VAL=ST16C554_IER_VAL;
    uint16_t LCR_DIVVAL=ST16C554_LCR_DIVVAL, LCR_DATVAL=ST16C554_LCR_DATVAL;
    uint16_t DLL_VAL=ST16C554_DLL_VAL, DLM_VAL=ST16C554_DLM_VAL;	//115200

    BSP_CS2A_GPIO_Init();
    BSP_CS2A_EXTI_Init();
    BSP_CS2A_NVIC_Init();

    BSP_PBus_WriteBuffer(&LCR_DIVVAL, (ST16C554_CS2A_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);	//LCR addr:011b<<1
    BSP_PBus_WriteBuffer(&DLL_VAL, (ST16C554_CS2A_ADD+(ST16C554_REG_DLL<<1))<<1, 0x01);		//DLL addr:000b<<1
    BSP_PBus_WriteBuffer(&DLM_VAL, (ST16C554_CS2A_ADD+(ST16C554_REG_DLM<<1))<<1, 0x01);		//DLM addr:001b<<1
    BSP_PBus_WriteBuffer(&LCR_DATVAL, (ST16C554_CS2A_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);
    BSP_PBus_WriteBuffer(&IER_VAL, (ST16C554_CS2A_ADD+(ST16C554_REG_IER<<1))<<1, 0x01);		//IER addr:001b<<1
}
/* GPIO��ʼ������ */
static void BSP_CS2A_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS2A_EXTI_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);	//ʹ�ܸ��ù���ʱ��

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
    EXTI_InitStructure.EXTI_Line=EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS2A_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    /* ��ռ���ȼ����ã����ȼ�����Ϊ 4 ������£���ռ���ȼ������÷�Χ 0-15 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 12;
    /* �����ȼ����ã����ȼ�����Ϊ 4 ������£������ȼ���Ч��ȡ��ֵ 0 ���� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS2A
*	����˵��: �弶 ST16C554 CS2A ���ܺ���
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_ST16C554_CS2A_ISR(void)
{
    uint8_t ret;
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
    {
        EXTI_ClearITPendingBit(EXTI_Line1);
        ret = BSP_ST16C554_CS2A_Read();
        if(ret == 0xff) return;
        switch(recSta)
        {
        case RC632_STA_SOP:
            rxflag = 0;
            len = 0;
            if(ret == 0xaa)
            {
                drbuf[len++] = ret;
                recSta = RC632_STA_LEN;
            }
            break;
        case RC632_STA_LEN:
            dataLen = ret;
            drbuf[len++] =  ret;
            recSta = RC632_STA_DAT;
            break;
        case RC632_STA_DAT:
            drbuf[len++] = ret;
            if(len == dataLen+1)
                recSta = RC632_STA_EOF;
            break;
        case RC632_STA_EOF:
            recSta = RC632_STA_SOP;
            dataLen = 0;
            drbuf[len++] = ret;
            if(ret == 0x55)
                rxflag = 1;
            break;
        }
    }
}
uint16_t BSP_ST16C554_CS2A_Read(void)
{
    uint16_t data;
    BSP_PBus_ReadBuffer(&data, (ST16C554_CS2A_ADD+(ST16C554_REG_RHR<<1))<<1, 1);
    return data;
}
void BSP_ST16C554_CS2A_Write(uint16_t BSP_ST16C554_CS2A_Val)
{
    BSP_PBus_WriteBuffer(&BSP_ST16C554_CS2A_Val, (ST16C554_CS2A_ADD+(ST16C554_REG_THR<<1))<<1, 1);
}


/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS2B_Init
*	����˵��: �弶 ST16C554 CS2B �ڲ���ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_ST16C554_CS2B_Init(void)
{
    //Registers Value
    uint16_t IER_VAL=ST16C554_IER_VAL;
    uint16_t LCR_DIVVAL=ST16C554_LCR_DIVVAL, LCR_DATVAL=ST16C554_LCR_DATVAL;
    uint16_t DLL_VAL=ST16C554_DLL_VAL, DLM_VAL=ST16C554_DLM_VAL;	//115200

    BSP_CS2B_GPIO_Init();
    BSP_CS2B_EXTI_Init();
    BSP_CS2B_NVIC_Init();

    BSP_PBus_WriteBuffer(&LCR_DIVVAL, (ST16C554_CS2B_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);	//LCR addr:011b<<1
    BSP_PBus_WriteBuffer(&DLL_VAL, (ST16C554_CS2B_ADD+(ST16C554_REG_DLL<<1))<<1, 0x01);		//DLL addr:000b<<1
    BSP_PBus_WriteBuffer(&DLM_VAL, (ST16C554_CS2B_ADD+(ST16C554_REG_DLM<<1))<<1, 0x01);		//DLM addr:001b<<1
    BSP_PBus_WriteBuffer(&LCR_DATVAL, (ST16C554_CS2B_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);
    BSP_PBus_WriteBuffer(&IER_VAL, (ST16C554_CS2B_ADD+(ST16C554_REG_IER<<1))<<1, 0x01);		//IER addr:001b<<1
}
/* GPIO��ʼ������ */
static void BSP_CS2B_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS2B_EXTI_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);	//ʹ�ܸ��ù���ʱ��

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource10);
    EXTI_InitStructure.EXTI_Line=EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS2B_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    /* ��ռ���ȼ����ã����ȼ�����Ϊ 4 ������£���ռ���ȼ������÷�Χ 0-15 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 12;
    /* �����ȼ����ã����ȼ�����Ϊ 4 ������£������ȼ���Ч��ȡ��ֵ 0 ���� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS2B
*	����˵��: �弶 ST16C554 CS2B ���ܺ���
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_ST16C554_CS2B_ISR(void)
{
    if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_10))
    {
        EXTI_ClearITPendingBit(EXTI_Line10);
        USART_SendData(USART1,BSP_ST16C554_CS2B_Read());
    }
}
uint16_t BSP_ST16C554_CS2B_Read(void)
{
    uint16_t data;
    BSP_PBus_ReadBuffer(&data, (ST16C554_CS2B_ADD+(ST16C554_REG_RHR<<1))<<1, 1);
    return data;
}
void BSP_ST16C554_CS2B_Write(uint16_t BSP_ST16C554_CS2B_Val)
{
    BSP_PBus_WriteBuffer(&BSP_ST16C554_CS2B_Val, (ST16C554_CS2B_ADD+(ST16C554_REG_THR<<1))<<1, 1);
}


/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS2C_Init
*	����˵��: �弶 ST16C554 CS2C �ڲ���ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_ST16C554_CS2C_Init(void)
{
    //Registers Value
    uint16_t IER_VAL=ST16C554_IER_VAL;
    uint16_t LCR_DIVVAL=ST16C554_LCR_DIVVAL, LCR_DATVAL=ST16C554_LCR_DATVAL;
    uint16_t DLL_VAL=ST16C554_DLL_VAL, DLM_VAL=ST16C554_DLM_VAL;	//115200

    BSP_CS2C_GPIO_Init();
    BSP_CS2C_EXTI_Init();
    BSP_CS2C_NVIC_Init();

    BSP_PBus_WriteBuffer(&LCR_DIVVAL, (ST16C554_CS2C_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);	//LCR addr:011b<<1
    BSP_PBus_WriteBuffer(&DLL_VAL, (ST16C554_CS2C_ADD+(ST16C554_REG_DLL<<1))<<1, 0x01);		//DLL addr:000b<<1
    BSP_PBus_WriteBuffer(&DLM_VAL, (ST16C554_CS2C_ADD+(ST16C554_REG_DLM<<1))<<1, 0x01);		//DLM addr:001b<<1
    BSP_PBus_WriteBuffer(&LCR_DATVAL, (ST16C554_CS2C_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);
    BSP_PBus_WriteBuffer(&IER_VAL, (ST16C554_CS2C_ADD+(ST16C554_REG_IER<<1))<<1, 0x01);		//IER addr:001b<<1
}
/* GPIO��ʼ������ */
static void BSP_CS2C_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS2C_EXTI_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);	//ʹ�ܸ��ù���ʱ��

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    EXTI_InitStructure.EXTI_Line=EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS2C_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    /* ��ռ���ȼ����ã����ȼ�����Ϊ 4 ������£���ռ���ȼ������÷�Χ 0-15 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 12;
    /* �����ȼ����ã����ȼ�����Ϊ 4 ������£������ȼ���Ч��ȡ��ֵ 0 ���� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS2C
*	����˵��: �弶 ST16C554 CS2C ���ܺ���
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_ST16C554_CS2C_ISR(void)
{
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
    {
        EXTI_ClearITPendingBit(EXTI_Line0);
        USART_SendData(USART1,BSP_ST16C554_CS2C_Read());
    }
}
uint16_t BSP_ST16C554_CS2C_Read(void)
{
    uint16_t data;
    BSP_PBus_ReadBuffer(&data, (ST16C554_CS2C_ADD+(ST16C554_REG_RHR<<1))<<1, 1);
    return data;
}
void BSP_ST16C554_CS2C_Write(uint16_t BSP_ST16C554_CS2C_Val)
{
    BSP_PBus_WriteBuffer(&BSP_ST16C554_CS2C_Val, (ST16C554_CS2C_ADD+(ST16C554_REG_THR<<1))<<1, 1);
}


/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS2D_Init
*	����˵��: �弶 ST16C554 CS2D �ڲ���ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_ST16C554_CS2D_Init(void)
{
    //Registers Value
    uint16_t IER_VAL=ST16C554_IER_VAL;
    uint16_t LCR_DIVVAL=ST16C554_LCR_DIVVAL, LCR_DATVAL=ST16C554_LCR_DATVAL;
    uint16_t DLL_VAL=ST16C554_DLL_VAL, DLM_VAL=ST16C554_DLM_VAL;	//115200

    BSP_CS2D_GPIO_Init();
    BSP_CS2D_EXTI_Init();
    BSP_CS2D_NVIC_Init();

    BSP_PBus_WriteBuffer(&LCR_DIVVAL, (ST16C554_CS2D_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);	//LCR addr:011b<<1
    BSP_PBus_WriteBuffer(&DLL_VAL, (ST16C554_CS2D_ADD+(ST16C554_REG_DLL<<1))<<1, 0x01);		//DLL addr:000b<<1
    BSP_PBus_WriteBuffer(&DLM_VAL, (ST16C554_CS2D_ADD+(ST16C554_REG_DLM<<1))<<1, 0x01);		//DLM addr:001b<<1
    BSP_PBus_WriteBuffer(&LCR_DATVAL, (ST16C554_CS2D_ADD+(ST16C554_REG_LCR<<1))<<1, 0x01);
    BSP_PBus_WriteBuffer(&IER_VAL, (ST16C554_CS2D_ADD+(ST16C554_REG_IER<<1))<<1, 0x01);		//IER addr:001b<<1
}
/* GPIO��ʼ������ */
static void BSP_CS2D_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS2D_EXTI_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);	//ʹ�ܸ��ù���ʱ��

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource4);
    EXTI_InitStructure.EXTI_Line=EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
}
/* �ⲿ�жϳ�ʼ������ */
static void BSP_CS2D_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    /* ��ռ���ȼ����ã����ȼ�����Ϊ 4 ������£���ռ���ȼ������÷�Χ 0-15 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 12;
    /* �����ȼ����ã����ȼ�����Ϊ 4 ������£������ȼ���Ч��ȡ��ֵ 0 ���� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
***************************************************************************
*	�� �� ��: BSP_ST16C554_CS2D
*	����˵��: �弶 ST16C554 CS2D ���ܺ���
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_ST16C554_CS2D_ISR(void)
{
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4))
    {
        EXTI_ClearITPendingBit(EXTI_Line4);
        USART_SendData(USART1,BSP_ST16C554_CS2D_Read());
    }
}
uint16_t BSP_ST16C554_CS2D_Read(void)
{
    uint16_t data;
    BSP_PBus_ReadBuffer(&data, (ST16C554_CS2D_ADD+(ST16C554_REG_RHR<<1))<<1, 1);
    return data;
}
void BSP_ST16C554_CS2D_Write(uint16_t BSP_ST16C554_CS2D_Val)
{
    BSP_PBus_WriteBuffer(&BSP_ST16C554_CS2D_Val, (ST16C554_CS2D_ADD+(ST16C554_REG_THR<<1))<<1, 1);
}
