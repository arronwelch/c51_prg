#include <REG52.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

//#define LED P0

sbit buzzer = P1 ^ 5;

void delay(u16 i) //10us
{
    while (i--)
        ;
}

void main()
{
    while (1)
    {
        buzzer = ~buzzer; //方波
        delay(100);
        //TH=100*10us=1ms
        //T=2ms
        //f=2KHz
    }
}