
#include "reg52.h"

typedef unsigned char u8;
typedef unsigned int u16;

sbit PWM = P2^1;
bit DIR;

//global variable
u16 count,value,timer1;

void Timer1Init()
{
    TMOD |= 0X10;//Timer1 Mode1:16bit

    TH1 = 0xFF;//1us
    TL1 = 0xFF;

    ET1 = 1;//Timer1 interrupt allow
    EA = 1;//Enable Master interrupt
    TR1 = 1;//Enable Timer1
}

void main(void)
{
    Timer1Init();
    while (1)
    {

        if (count > 100)
        {
            count = 0;
            if(DIR)
            {
                value++;
            }
            else
            {
                value--;
            }
        }

        if (value==1000)
        {
            DIR = 0;
        }
        if(value == 0)
        {
            DIR = 1;
        }

        if(timer1 > 1000)//period = 1000*1us = 1ms
        {
            timer1 = 0;
        }

        if(timer1 < value)
        {
            PWM = 1;
        }
        else
        {
            PWM = 0;
        }   

    }
    
}

void Time1(void) interrupt 3//interrupt No. = 3
{
    TH1 = 0XFF;//1us
    TL1 = 0XFF;
    timer1++;//period
    count++;//duty cycle
}