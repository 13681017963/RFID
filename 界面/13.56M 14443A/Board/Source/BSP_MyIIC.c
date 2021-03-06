/*
***************************************************************************
*    模块：BSP_MyIIC
*    描述：板级 模拟IIC 功能模块驱动
		   SCL-PB6  SDA-PB7
*    作者：Huo
*    时间：2017.09.25
*    版本：UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_MyIIC.h"

/* 内部函数声明 */
static void BSP_MyIIC_Delay(void);

/*
***************************************************************************
*	函 数 名: BSP_MyIIC_Init
*	功能说明: 板级 模拟IIC 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_MyIIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_I2C_PORT, ENABLE);	/* 打开GPIO时钟 */

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	/* 输出模式 */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	/* 设为开漏模式 */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;/* 上下拉电阻不使能 */

    GPIO_InitStructure.GPIO_Pin = PIN_I2C_SCL;
    GPIO_Init(PORT_I2C_SCL, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = PIN_I2C_SDA;
    GPIO_Init(PORT_I2C_SDA, &GPIO_InitStructure);

    /* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
    BSP_MyIIC_Stop();
}

/*
***************************************************************************
*	函 数 名: BSP_MyIIC_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_MyIIC_Delay(void)
{
    volatile uint8_t i;

    /*　
    	CPU主频168MHz时，在内部Flash运行, MDK工程不优化。用台式示波器观测波形。
    	循环次数为5时，SCL频率 = 1.78MHz (读耗时: 92ms, 读写正常，但是用示波器探头碰上就读写失败。时序接近临界)
    	循环次数为10时，SCL频率 = 1.1MHz (读耗时: 138ms, 读速度: 118724B/s)
    	循环次数为30时，SCL频率 = 440KHz， SCL高电平时间1.0us，SCL低电平时间1.2us

    	上拉电阻选择2.2K欧时，SCL上升沿时间约0.5us，如果选4.7K欧，则上升沿约1us

    	实际应用选择400KHz左右的速率即可
    */
    for (i = 0; i < 30; i++);
//	for (i = 0; i < 40; i++);
}

/*
***************************************************************************
*	函 数 名: BSP_MyIIC_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_MyIIC_Start(void)
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
    I2C_SDA_1();
    I2C_SCL_1();
    BSP_MyIIC_Delay();
    I2C_SDA_0();
    BSP_MyIIC_Delay();

    I2C_SCL_0();
    BSP_MyIIC_Delay();
}

/*
***************************************************************************
*	函 数 名: BSP_MyIIC_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_MyIIC_Stop(void)
{
    /* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
    I2C_SDA_0();
    I2C_SCL_1();
    BSP_MyIIC_Delay();
    I2C_SDA_1();
    BSP_MyIIC_Delay();
}

/*
***************************************************************************
*	函 数 名: BSP_MyIIC_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参:  _ucByte ： 等待发送的字节
*	返 回 值: 无
***************************************************************************
*/
void BSP_MyIIC_SendByte(uint8_t _ucByte)
{
    uint8_t i;

    /* 先发送字节的高位bit7 */
    for (i = 0; i < 8; i++)
    {
        if (_ucByte & 0x80)
        {
            I2C_SDA_1();
        }
        else
        {
            I2C_SDA_0();
        }
        BSP_MyIIC_Delay();
        I2C_SCL_1();
        BSP_MyIIC_Delay();
        I2C_SCL_0();
        if (i == 7)
        {
            I2C_SDA_1(); // 释放总线
        }
        _ucByte <<= 1;	/* 左移一个bit */
        BSP_MyIIC_Delay();
    }
}

/*
***************************************************************************
*	函 数 名: BSP_MyIIC_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参: 无
*	返 回 值: 读到的数据
***************************************************************************
*/
uint8_t BSP_MyIIC_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        I2C_SCL_1();
        BSP_MyIIC_Delay();
        if (I2C_SDA_READ())
        {
            value++;
        }
        I2C_SCL_0();
        BSP_MyIIC_Delay();
    }
    return value;
}

/*
***************************************************************************
*	函 数 名: BSP_MyIIC_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参: 无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
***************************************************************************
*/
uint8_t BSP_MyIIC_WaitAck(void)
{
    uint8_t re;

    I2C_SDA_1();	/* CPU释放SDA总线 */
    BSP_MyIIC_Delay();
    I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    BSP_MyIIC_Delay();
    if (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    I2C_SCL_0();
    BSP_MyIIC_Delay();
    return re;
}

/*
***************************************************************************
*	函 数 名: BSP_MyIIC_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_MyIIC_Ack(void)
{
    I2C_SDA_0();	/* CPU驱动SDA = 0 */
    BSP_MyIIC_Delay();
    I2C_SCL_1();	/* CPU产生1个时钟 */
    BSP_MyIIC_Delay();
    I2C_SCL_0();
    BSP_MyIIC_Delay();
    I2C_SDA_1();	/* CPU释放SDA总线 */
}

/*
***************************************************************************
*	函 数 名: BSP_MyIIC_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_MyIIC_NAck(void)
{
    I2C_SDA_1();	/* CPU驱动SDA = 1 */
    BSP_MyIIC_Delay();
    I2C_SCL_1();	/* CPU产生1个时钟 */
    BSP_MyIIC_Delay();
    I2C_SCL_0();
    BSP_MyIIC_Delay();
}

/*
***************************************************************************
*	函 数 名: BSP_MyIIC_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参: Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
***************************************************************************
*/
uint8_t BSP_MyIIC_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;

    if (I2C_SDA_READ() && I2C_SCL_READ())
    {
        BSP_MyIIC_Start();		/* 发送启动信号 */

        /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
        BSP_MyIIC_SendByte(_Address | I2C_WR);
        ucAck = BSP_MyIIC_WaitAck();	/* 检测设备的ACK应答 */

        BSP_MyIIC_Stop();			/* 发送停止信号 */

        return ucAck;
    }
    return 1;	/* I2C总线异常 */
}
