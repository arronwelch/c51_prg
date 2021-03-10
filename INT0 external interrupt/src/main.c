#include <REG52.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit D11 = P2 ^ 0;
sbit D12 = P2 ^ 1;
sbit K3 = P3 ^ 2;
sbit K4 = P3 ^ 3;

void delay(u16 i)
{
    while (i--)
        ;
}

void Int0_Init()
{
    IT0 = 1; //Set Transition from high to low level to triger Interrupt
    EX0 = 1; //Enable INT0 Interrupt
    EA = 1;  //Enable Total Interrupt
}

void Int1_Init()
{
    IT1 = 1; //Set Transition from high to low level to triger Interrupt
    EX1 = 1; //Enable INT0 Interrupt
    EA = 1;  //Enable Total Interrupt
}

void main()
{
    Int0_Init();
    Int1_Init();
    while (1)
        ;
}

void Int0() interrupt 0
{
    delay(2000);
    if (K3 == 0)
    {
        D11 = ~D11;
        K3 = 1;
    }
}

void Int1() interrupt 2
{
    delay(2000);
    if (K4 == 0)
    {
        D12 = ~D12;
        K3 = 1;
    }
}