#include <REG52.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit D11 = P0 ^ 0;
sbit K1 = P1 ^ 0;

void delay(u16 i)
{
    while (i--)
        ;
}

void keyCtrl()
{
    if (K1 == 0)
    {
        delay(1000);
        if (K1 == 0)
        {
            D11 = ~D11;
        }
        while (!K1)
            ;
    }
}

void main()
{
    D11 = 0;
    while (1)
    {
        keyCtrl();
    }
}