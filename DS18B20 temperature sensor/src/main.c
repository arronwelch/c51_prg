#include "reg52.h"

typedef unsigned char u8;
typedef unsigned int u16;

sbit DQ = P3 ^ 7;

//dynamic 8-seg LED display BIT select
sbit LSA = P2 ^ 2;
sbit LSB = P2 ^ 3;
sbit LSC = P2 ^ 4;

#define GPIO_SEGCODE P0 //dynamic 8-seg LED display DECODE select GPIO
u8 code NsegCode[16] = 
// '0'  '1'   '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'
 {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
// 'A'  'b'   'C'   'd'   'E'   'F'
  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

u8 SegDisplayData[8]={0};

void delay(u16 i)
{
    while (i--);
}

void delay_18B20(unsigned int i)
{
    for (; i > 0; i--)
        ;
}

void Init_DS18B20(void)
{
    unsigned char x = 0;
    DQ = 1;          //DQ拉高
    delay_18B20(8);  //稍作延时
    DQ = 0;          //DQ拉低
    delay_18B20(80); //延时大于480us
    DQ = 1;          //拉高总线
    delay_18B20(14);
    x = DQ; //若x=0初始化成功，若x=1初始化失败
    delay_18B20(20);
}

unsigned char ReadOneChar(void)
{
    unsigned char i = 0;
    unsigned char dat = 0;
    for (i = 8; i > 0; i--)
    {
        DQ = 0;    // 拉低总线
        dat >>= 1; //每读取移位向右移移位
        DQ = 1;    //拉高总线
        if (DQ)
            dat |= 0x80;
        delay_18B20(4);
    }
    return (dat);
}

void WriteOneChar(unsigned char dat)
{
    unsigned char i = 0;
    for (i = 8; i > 0; i--)
    {
        DQ = 0;
        DQ = dat & 0x01;
        if (DQ)
        {
            delay_18B20(1);
            DQ = 1;
        }
        else
        {
            delay_18B20(5);
            DQ = 1;
        }
        dat >>= 1;
    }
}

unsigned int ReadTemperature(void)
{
    unsigned char a = 0, b = 0;
    unsigned int temp = 0;
    Init_DS18B20();
    WriteOneChar(0xCC); // 跳过读序列号操作
    WriteOneChar(0x44); // 启动温度转换
    delay_18B20(100);   //
    Init_DS18B20();
    WriteOneChar(0xCC); //跳过读序列号操作
    WriteOneChar(0xBE); //读取温度寄存器
    delay_18B20(100);
    a = ReadOneChar();           //读温度低位
    b = ReadOneChar();           //读温度高位
    temp = ((b * 256 + a) >> 4); //当前采集温度除16得到实际温度
    return (temp);
}

void DigDisplay()
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        switch (i)
        {
        case (0):{LSA = 0;LSB = 0;LSC = 0;}break;
        case (1):{LSA = 1;LSB = 0;LSC = 0;}break;
        case (2):{LSA = 0;LSB = 1;LSC = 0;}break;
        case (3):{LSA = 1;LSB = 1;LSC = 0;}break;
        case (4):{LSA = 0;LSB = 0;LSC = 1;}break;
        case (5):{LSA = 1;LSB = 0;LSC = 1;}break;
        case (6):{LSA = 0;LSB = 1;LSC = 1;}break;
        case (7):{LSA = 1;LSB = 1;LSC = 1;}break;
        default:break;
        }
        GPIO_SEGCODE = SegDisplayData[i];
        delay(100); //delay 1ms
        GPIO_SEGCODE = 0X00;  //clear
    }
}

void main()
{
	unsigned int temp=0;
    while (1)
    {
        temp=ReadTemperature();
        SegDisplayData[0]=NsegCode[temp/100];
        SegDisplayData[1]=NsegCode[temp%100/10];
        SegDisplayData[2]=NsegCode[temp%10];
		DigDisplay();
    }
}