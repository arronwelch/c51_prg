#include <REG52.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit LSA = P2 ^ 2;
sbit LSB = P2 ^ 3;
sbit LSC = P2 ^ 4;

#define GPIO_DIG P0
#define GPIO_TRAFFIC P1

sbit RED10 = P1^0;
sbit GRN10 = P1^1;

sbit RED11 = P1^2;
sbit YLW11 = P1^3;
sbit GRN11 = P1^4;

sbit RED00 = P3^0;
sbit GRN00 = P3^1;

sbit RED01 = P1^5;
sbit YLW01 = P1^6;
sbit GRN01 = P1^7;

//Common GND
// ---a---
// |     |
// f     b
// |--g--|
// e     c
// |     |
// ---d---  *dp
// dp g f e d c b a
// 0  0 1 1 1 1 1 1

u8 code NsegCode[16] = 
// '0'  '1'   '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'
 {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
// 'A'  'b'   'C'   'd'   'E'   'F'
  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

void delay(u16 i)
{
    while (i--);
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
    TMOD|=0X10;//select timer1 mode 1:16 bit timwer,TR0 enable

    TH1=0XFC;//init value,delay 1ms
    TL1=0X18;//2^16-1000=64536=0xFC18

    ET1=1;//Timer1 interrupt enable
    EA=1;//enable interrupt gate
    TR1=1;//enable Timer1
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
        P0 = NsegCode[1+i];
        delay(100); //delay 1ms
        P0 = 0X00;  //clear
    }
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
    static u8 n;
    TH1=0XFC;//2^16-1000
    TL1=0X18;
    j++;
    if (j==1000)//1s
    {
        j=0;
        P0 = ~NsegCode[n++];
        if (n==16)
        {
            n=0;
        }
    }
}