/*
*******************************************************************************
*    ģ�飺BSP_GSL268x
*    �������弶 GSL268x ����ģ������
           GSL268x_ADDR  0x80
*    ���ߣ�Huo
*    ʱ�䣺2017.12.12
*    �汾��UP-Magic-Version 1.0.0
*******************************************************************************
*/
#include "BSP_GSL268x.h"
#include "GSL268x_fw.h"

/* �ڲ��������� */
static void BSP_GSL268x_GPIO_Init(void);
static uint8_t I2C_WriteByte(uint8_t SendByte, uint8_t WriteAddress);
static uint8_t I2C_BufferWrite(uint8_t* pBuffer, uint8_t length, uint8_t WriteAddress);
static uint8_t I2C_BufferRead(uint8_t* pBuffer, uint8_t length, uint8_t ReadAddress);
static void gsl_start_core(void);
static void gsl_reset_core(void);
static void gsl_clear_reg(void);
static uint8_t check_mem_data(void);
static void gsl_fw_load(void);

/* �Ǿ�ȷ��ʱ���� */
static void BSP_GSL268x_DelayMs(volatile uint16_t xMs)
{
    volatile uint32_t Count=24000;
    while(xMs--)
    {
        Count=24000;
        while(Count--);
    }
}

/*
*******************************************************************************
*	�� �� ��: BSP_GSL268x_Init
*	����˵��: �弶 GSL268x ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
*******************************************************************************
*/
void BSP_GSL268x_Init(void)
{
//	BSP_MyIIC_Init();
    while(BSP_MyIIC_CheckDevice(GSL268x_ADDR)) {};
    //init gpio
    BSP_GSL268x_GPIO_Init();

    //set register
    gsl_clear_reg();
    gsl_reset_core();
    gsl_fw_load();
    gsl_start_core();
    BSP_GSL268x_DelayMs(20);
    check_mem_data();
}

/*
*******************************************************************************
*	�� �� ��: BSP_GSL268x_GPIO_Init
*	����˵��: �弶 GSL268x IO�� ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
*******************************************************************************
*/
static void BSP_GSL268x_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_GSL268x_INT, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
    GPIO_InitStructure.GPIO_Pin = PIN_GSL268x_INT; //��Ӧ����
    GPIO_Init(PORT_GSL268x_INT, &GPIO_InitStructure);
}

/*
*******************************************************************************
*	�� �� ��: BSP_GSL268x_Callback
*	����˵��: �弶 GSL268x �ϱ���λ���ܺ���
*	��    ��: PEN ��λ����
*	�� �� ֵ: 0-��Ч  1-��Ч
*******************************************************************************
*/
uint8_t BSP_GSL268x_Callback(uint16_t PEN[2])
{
    uint8_t TPCodebuf[4];

    if(GPIO_ReadInputDataBit(PORT_GSL268x_INT, PIN_GSL268x_INT))
    {
        I2C_BufferRead(&TPCodebuf[0],4,0x84);
        PEN[0] = ((0x0F&TPCodebuf[3])<<8)|TPCodebuf[2];  //X
        PEN[1] = TPCodebuf[1]<<8|TPCodebuf[0];           //Y

        return 0;
    }
    else
        return 1;
}

/*
*******************************************************************************
*	�� �� ��: gsl_start_core  gsl_reset_core  gsl_clear_reg  check_mem_data  gsl_fw_load
*	����˵��: GSL268x�Ĵ����������ܺ���
*	��    ��:
*	�� �� ֵ:
*******************************************************************************
*/
static void gsl_start_core(void)
{
    I2C_WriteByte(0x00,0xe0);
    BSP_GSL268x_DelayMs(1);
}
static void gsl_reset_core(void)
{
    I2C_WriteByte(0x88,0xE0);
    BSP_GSL268x_DelayMs(1);
    I2C_WriteByte(0x04,0xE4);
    BSP_GSL268x_DelayMs(1);
    I2C_WriteByte(0x00,0xbc);
    BSP_GSL268x_DelayMs(1);
}
static void gsl_clear_reg(void)
{
    I2C_WriteByte(0x88,0xE0);
    BSP_GSL268x_DelayMs(1);
    I2C_WriteByte(0x01,0x80);
    BSP_GSL268x_DelayMs(1);
    I2C_WriteByte(0x04,0xE4);
    BSP_GSL268x_DelayMs(1);
    I2C_WriteByte(0x00,0xE0);
    BSP_GSL268x_DelayMs(1);
}
static uint8_t check_mem_data(void)
{
    uint8_t read_buf[4]  = {0};

    I2C_BufferRead(read_buf,4,0xb0);
    BSP_GSL268x_DelayMs(1);
    if (read_buf[3] != 0x5a || read_buf[2] != 0x5a || read_buf[1] != 0x5a || read_buf[0] != 0x5a)
    {
#if 1
        printf("load firmware failed.!\n");
#endif
        return 0;
    }
    else
    {
#if 0
        printf("load firmware success.!\n");
#endif
        return 1;
    }
}
static void gsl_fw_load(void)
{
    uint8_t buf[5] = {0};
    uint32_t source_line = 0;
    uint8_t *cur = buf + 1;

    for (source_line = 0; source_line < TOUCH_CODE_LEN; source_line++)
    {
        buf[0]= GSL2681_FW[source_line].offset;
        buf[1] = (uint8_t)(GSL2681_FW[source_line].val&0x000000ff);
        buf[2] = (uint8_t)((GSL2681_FW[source_line].val&0x0000ff00) >> 8);
        buf[3] = (uint8_t)((GSL2681_FW[source_line].val&0x00ff0000) >> 16);
        buf[4] = (uint8_t)((GSL2681_FW[source_line].val&0xff000000) >> 24);

        if (buf[0] == 0xf0)
        {
            I2C_WriteByte(buf[1],buf[0]);
        }
        else
        {
            I2C_BufferWrite(cur, 4,buf[0]);
        }
    }
}

/*
*******************************************************************************
*	�� �� ��: I2C_WriteByte  I2C_BufferWrite  I2C_BufferRead
*	����˵��: GSL268x�ӿڲ������ܺ���
*	��    ��:
*	�� �� ֵ:
*******************************************************************************
*/
static uint8_t I2C_WriteByte(uint8_t SendByte, uint8_t WriteAddress)
{
    uint16_t m;

    for (m = 0; m < 1000; m++)
    {
        /* ��1��������I2C���������ź� */
        BSP_MyIIC_Start();
        /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
        BSP_MyIIC_SendByte(GSL268x_ADDR | I2C_WR);	/* �˴���дָ�� */
        /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
        if (BSP_MyIIC_WaitAck() == 0)
        {
            break;
        }
    }
    if (m  == 1000)
    {
        /* EEPROM����д��ʱ,����I2C����ֹͣ�ź� */
        BSP_MyIIC_Stop();
        return 0;
    }
    /* ��4���������ֽڵ�ַ */
    BSP_MyIIC_SendByte((uint8_t)WriteAddress);
    if (BSP_MyIIC_WaitAck() != 0)
    {
        /* EEPROM������Ӧ��,����I2C����ֹͣ�ź� */
        BSP_MyIIC_Stop();
        return 0;
    }
    /* ��6������ʼд������ */
    BSP_MyIIC_SendByte(SendByte);
    /* ��7��������ACK */
    if (BSP_MyIIC_WaitAck() != 0)
    {
        /* EEPROM������Ӧ��,����I2C����ֹͣ�ź� */
        BSP_MyIIC_Stop();
        return 0;
    }
    /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
    BSP_MyIIC_Stop();
    return 1;
}
static uint8_t I2C_BufferWrite(uint8_t* pBuffer, uint8_t length, uint8_t WriteAddress)
{
    uint16_t i,m;

    for (i = 0; i < length; i++)
    {
        for (m = 0; m < 1000; m++)
        {
            /* ��1��������I2C���������ź� */
            BSP_MyIIC_Start();
            /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
            BSP_MyIIC_SendByte(GSL268x_ADDR | I2C_WR);	/* �˴���дָ�� */
            /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
            if (BSP_MyIIC_WaitAck() == 0)
            {
                break;
            }
        }
        if (m  == 1000)
        {
            /* EEPROM����д��ʱ,����I2C����ֹͣ�ź� */
            BSP_MyIIC_Stop();
            return 0;
        }
        /* ��4���������ֽڵ�ַ */
        BSP_MyIIC_SendByte((uint8_t)WriteAddress);
        if (BSP_MyIIC_WaitAck() != 0)
        {
            /* EEPROM������Ӧ��,����I2C����ֹͣ�ź� */
            BSP_MyIIC_Stop();
            return 0;
        }
        /* ��6������ʼд������ */
        BSP_MyIIC_SendByte(pBuffer[i]);
        /* ��7��������ACK */
        if (BSP_MyIIC_WaitAck() != 0)
        {
            /* EEPROM������Ӧ��,����I2C����ֹͣ�ź� */
            BSP_MyIIC_Stop();
            return 0;
        }
        WriteAddress++;
    }
    /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
    BSP_MyIIC_Stop();
    return 1;
}
static uint8_t I2C_BufferRead(uint8_t* pBuffer, uint8_t length, uint8_t ReadAddress)
{
    uint16_t i;

    /* ��1��������I2C���������ź� */
    BSP_MyIIC_Start();
    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    BSP_MyIIC_SendByte(GSL268x_ADDR | I2C_WR);	/* �˴���дָ�� */
    /* ��3��������ACK */
    if (BSP_MyIIC_WaitAck() != 0)
    {
        /* EEPROM������Ӧ��,����I2C����ֹͣ�ź� */
        BSP_MyIIC_Stop();
        return 0;
    }
    /* ��4���������ֽڵ�ַ */
    BSP_MyIIC_SendByte((uint8_t)ReadAddress);
    if (BSP_MyIIC_WaitAck() != 0)
    {
        /* EEPROM������Ӧ��,����I2C����ֹͣ�ź� */
        BSP_MyIIC_Stop();
        return 0;
    }
    /* ��5��������I2C����ֹͣ�ź� */
    BSP_MyIIC_Stop();
    BSP_GSL268x_DelayMs(10);		//huo 20160905

    /* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
    BSP_MyIIC_Start();
    /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    BSP_MyIIC_SendByte(GSL268x_ADDR | I2C_RD);	/* �˴��Ƕ�ָ�� */
    /* ��8��������ACK */
    if (BSP_MyIIC_WaitAck() != 0)
    {
        /* EEPROM������Ӧ��,����I2C����ֹͣ�ź� */
        BSP_MyIIC_Stop();
        return 0;
    }
    /* ��9����ѭ����ȡ���� */
    for (i = 0; i < length; i++)
    {
        pBuffer[i] = BSP_MyIIC_ReadByte();	/* ��1���ֽ� */

        /* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
        if (i != length - 1)
        {
            BSP_MyIIC_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
        }
        else
        {
            BSP_MyIIC_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
        }
    }
    /* ����I2C����ֹͣ�ź� */
    BSP_MyIIC_Stop();
    return 1;	/* ִ�гɹ� */
}
