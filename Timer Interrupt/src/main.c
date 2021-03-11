#include <REG52.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit D11 = P2 ^ 0;
sbit D12 = P2 ^ 1;

void delay(u16 i)
{
    while (i--)
        ;
}

void Timer0Init()
{
    TMOD|=0X01;//mode 1:16 bit timwer

    TH0=0XFC;//1ms
    TL0=0X18;//2^16-1000

    ET0=1;
    EA=1;
    TR0=1;
}

void Timer1Init()
{
    TMOD|=0X10;//mode 1:16 bit timwer

    TH1=0XFC;//1ms
    TL1=0X18;//2^16-1000

    ET1=1;
    EA=1;
    TR1=1;
}

void main()
{
    Timer0Init();
    Timer1Init();
    while (1);
}

void Timer0() interrupt 1
{
    static u16 i;
    TH0=0XFC;//2^16-1000
    TL0=0X18;
    i++;
    if (i==1000)//1s
    {
        i=0;
        D11=~D11;
    }
}

void Timer1() interrupt 3
{
    static u16 j;
    TH1=0XFC;//2^16-1000
    TL1=0X18;
    j++;
    if (j==2000)//1s
    {
        j=0;
        D12=~D12;
    }
}