#include <REG52.H>
#include "i2c.h"

typedef unsigned char u8;
typedef unsigned int u16;

sbit k1 = P1^0;//save
sbit k2 = P1^1;//read
sbit k3 = P1^2;//add++
sbit k4 = P1^3;//clear

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

u8 num=0,SegDisplayData[4]={0};

void delay(u16 t)
{
    while (t--)
        ;
}

void Keypros()
{
    if (k1==0)
    {
        delay(1000);
        if (k1==0)
        {
            At24c02Write(1,num);
        }
        while (!k1);
    }
    if (k2==0)
    {
        delay(1000);
        if (k2==0)
        {
            num=At24c02Read(1);
        }
        while (!k2);
    }
    if (k3==0)
    {
        delay(1000);
        if (k3==0)
        {
            num++;
            if (num>255)
            {
                num=0;
            }
        }
        while (!k3);
    }
    if (k4==0)
    {
        delay(1000);
        if (k4==0)
        {
            num=0;
        }
        while (!k4);
    }
}

void datapros()
{
    SegDisplayData[0]=NsegCode[num/1000];
    SegDisplayData[1]=NsegCode[num%1000/100];
    SegDisplayData[2]=NsegCode[num%1000%100/10];
    SegDisplayData[3]=NsegCode[num%1000%100%10];
}

void DigDisplay()
{
    u8 i;
    for (i = 0; i < 4; i++)
    {
        switch (i)
        {
        case (0):{LSA = 0;LSB = 0;LSC = 0;}break;
        case (1):{LSA = 1;LSB = 0;LSC = 0;}break;
        case (2):{LSA = 0;LSB = 1;LSC = 0;}break;
        case (3):{LSA = 1;LSB = 1;LSC = 0;}break;
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
        Keypros();
        datapros();
        DigDisplay();
    }
}