#include <REG52.H>
#include "ds18b20.h"

typedef unsigned char u8;
typedef unsigned int u16;

//dynamic 8-seg  LED display BIT select
sbit LSA = P2 ^ 2;
sbit LSB = P2 ^ 3;
sbit LSC = P2 ^ 4;

#define GPIO_SEGCODE P0 //dynamic 8-seg LED display DECODE select GPIO

u8 code NsegCode[10] = 
// '0'  '1'   '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'
 {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

u8 SegDisplayData[6];

void delay(u16 t)
{
    while (t--);
}

void datapros(float temp)
{
    int tp;
    tp = temp;
    if(tp<0)
    {
        SegDisplayData[0]=0x40;//'-'
    }
    else
    {
        SegDisplayData[0]=0x00;
    }  
    SegDisplayData[1]=NsegCode[(tp/10000)];//125
    SegDisplayData[2]=NsegCode[(tp%10000/1000)];//25
    SegDisplayData[3]=NsegCode[(tp%1000/100)]|0x80;//5.
    SegDisplayData[4]=NsegCode[(tp%100/10)];//.0
    SegDisplayData[5]=NsegCode[(tp%100%10)];//.00
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
      //case (6):{LSA = 0;LSB = 1;LSC = 1;}break;
      //case (7):{LSA = 1;LSB = 1;LSC = 1;}break;
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