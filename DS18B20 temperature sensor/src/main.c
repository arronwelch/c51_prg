#include <REG52.H>
#include "ds18b20.h"

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

u8 SegDisplayData[6]={0};

void delay(u16 t)
{
    while (t--)
        ;
}

void datapros(int temp)
{
    float tp;
    if(temp<0)
    {
        SegDisplayData[0]=0x40;//'-'
        temp=temp-1;
        temp=~temp;
        tp = temp;
        temp = tp*0.0625*100+0.5;
    }
    else
    {
        SegDisplayData[0]=0x00;
        tp = temp;
        temp = tp*0.0625*100+0.5;
    }
    SegDisplayData[1]=NsegCode[temp/10000];
    SegDisplayData[2]=NsegCode[temp%10000/1000];
    SegDisplayData[3]=NsegCode[temp%10000%1000/100]|0x80;
    SegDisplayData[4]=NsegCode[temp%100/10];
    SegDisplayData[5]=NsegCode[temp%100%10];
}

void DigDisplay()
{
    u8 i;
    for (i = 0; i < 6; i++)
    {
        switch (i)
        {
        case (0):{LSA = 0;LSB = 0;LSC = 0;}break;
        case (1):{LSA = 1;LSB = 0;LSC = 0;}break;
        case (2):{LSA = 0;LSB = 1;LSC = 0;}break;
        case (3):{LSA = 1;LSB = 1;LSC = 0;}break;
        case (4):{LSA = 0;LSB = 0;LSC = 1;}break;
        case (5):{LSA = 1;LSB = 0;LSC = 1;}break;      
        default:break;
        }
        GPIO_SEGCODE = SegDisplayData[i];
        delay(100); //delay 1ms
        GPIO_SEGCODE = 0X00;  //clear
    }
}

void main()
{
    GPIO_SEGCODE = 0X00; 
    while (1)
    {
        datapros(Ds18b20ReadTemp());
        DigDisplay();
    }
}