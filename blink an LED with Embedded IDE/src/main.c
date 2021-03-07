#include <REG52.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit led = P0 ^ 0;

void delay(u16 i)
{
    while (i--)
        ;
}

void main()
{
    while (1)
    {
        led = 1;
        delay(50000);
        led = 0;
        delay(50000);
    }
}