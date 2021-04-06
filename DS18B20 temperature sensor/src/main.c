#include "reg51.h"
#include "intrins.h"//声明void _nop_(void);

//STC12C5A16S2
//XTAL 12.0MHz
//

sbit DQ = P3^7;

typedef unsigned char u8;
typedef unsigned int u16;

//dynamic 8-seg  LED display BIT select
sbit LSA = P2 ^ 2;
sbit LSB = P2 ^ 3;
sbit LSC = P2 ^ 4;

#define GPIO_SEGCODE P0 //dynamic 8-seg LED display DECODE select GPIO

u8 code NsegCode[16] = 
// '0'  '1'   '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'
 {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
// 'A'  'b'   'C'   'd'   'E'   'F'
  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

u8 SegDisplayData[8];

void delay(u16 t)
{
    while (t--)
        ;
}

//TEMPERATURE (°C) DIGITAL OUTPUT
//(BINARY)
//DIGITAL OUTPUT
//(HEX)
//+125 0000 0111 1101 0000 07D0h
//+85* 0000 0101 0101 0000 0550h
//+25.0625 0000 0001 1001 0001 0191h
//+10.125 0000 0000 1010 0010 00A2h
//+0.5 0000 0000 0000 1000 0008h
//0 0000 0000 0000 0000 0000h
//-0.5 1111 1111 1111 1000 FFF8h
//-10.125 1111 1111 0101 1110 FF5Eh
//-25.0625 1111 1110 0110 1111 FE6Fh
//-55 1111 1100 1001 0000 FC90h
void datapros(u16 temp)
{
    u16 tph,tpl;
    if(temp>0x7ff)
    {
        SegDisplayData[0]=0x40;
        temp = (~temp)+1;
        tph = temp*0.0625;
        tpl = ((temp*0.0625)-tph)*10000;
    }
    else
    {
        SegDisplayData[0]=0x00;
        tph = temp*0.0625;
        tpl = ((temp*0.0625)-tph)*10000;
    }
    if((tph/100)==0)
    {
        SegDisplayData[1]=0X00;
    }
    else
    {
        SegDisplayData[1]=NsegCode[tph/100];
    }    
    SegDisplayData[2]=NsegCode[tph%100/10];
    SegDisplayData[3]=NsegCode[tph%10]|0x80;
    SegDisplayData[4]=NsegCode[tpl%10000/1000];
    SegDisplayData[5]=NsegCode[tpl%1000/100];
    SegDisplayData[6]=NsegCode[tpl%100/10];
    SegDisplayData[7]=NsegCode[tpl%10];
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


//每个IC都不一样，得抓波形来慢慢调时序
void delay_18B20(unsigned int i)
{
    for(;i>0;i--);
}

void Init_DS18B20(void) 	
{
  unsigned char x=0;
  //MCU poweron default pull up to High
  DQ = 0;          //DQ拉低
  delay_18B20(400);//569.6us,最小480us
  DQ = 1;          //拉高总线
  //28.67us DS18B20 output LOW(15-60us)
  //BS18B20 delay 107.7us releasee bus(60-240us)
  delay_18B20(400);//569.6us,延时只要大于300us就可以
}

void WriteOneChar(unsigned char dat)
{
    unsigned char i = 0;
    for (i = 8; i > 0; i--)
    {
        DQ = 0;//拉低总线
        delay_18B20(1);//拉低总线至少1us,现在4us 
        //4us在15us之内,释放单总线,写1
        DQ = dat & 0x01;
        if (DQ)
        {
            delay_18B20(40);//延时60us
            DQ = 1;//释放单总线
            delay_18B20(1);//释放单总线至少1us,现在4us
        }
        else
        {
            delay_18B20(40);//写0,至少拉低60us
            DQ = 1;//释放单总线
            delay_18B20(1);//释放单总线至少1us,现在4us
        }
        dat >>= 1;
    }
}

unsigned char ReadOneChar(void)
{
    unsigned char i = 0;
    unsigned char dat = 0;
    for (i = 8; i > 0; i--)
    { 
        DQ = 0;// 拉低总线
        delay_18B20(1);//4us
        dat >>= 1; //每读取移位向右移移位
        DQ = 1;    //拉高总线,如果IC输出0，没法在逻辑分析仪上看到上拉波形
        delay_18B20(2);//8us
        if (DQ)
            dat |= 0x80;
        delay_18B20(20);//30us
    }
    return (dat);
}

unsigned int ReadTemperature(void)
{							 
 unsigned char a,b;
 unsigned int temp;
 Init_DS18B20();
 WriteOneChar(0xCC);  // 跳过读序列号操作
 WriteOneChar(0x44);  // 启动温度转换
 delay_18B20(100);    // 
 Init_DS18B20();
 WriteOneChar(0xCC);  //跳过读序列号操作
 WriteOneChar(0xBE);  //读取温度寄存器
 delay_18B20(100);
 a=ReadOneChar();     //读温度低位
 b=ReadOneChar();     //读温度高位
 temp=(b<<8)+a; //当前采集温度原码
 return(temp);
}


void main()
{
    GPIO_SEGCODE = 0X00;  //clear
    while (1)
    {
        datapros(ReadTemperature());
        DigDisplay();
    }    
}