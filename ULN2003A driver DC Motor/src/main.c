#include <REG52.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit moto = P1 ^ 0;

void delay(u16 i)
{
    while (i--)
        ;
}

void main()
{
    u8 i;
    moto=0;

    for ( i = 0; i < 100; i++)
    {
        moto=1;
        delay(8000);
        moto=0;
        delay(2000);
    }


    for ( i = 0; i < 100; i++)
    {
        moto=1;
        delay(6000);
        moto=0;
        delay(4000);
    }    

    for ( i = 0; i < 100; i++)
    {
        moto=1;
        delay(5000);
        moto=0;
        delay(5000);
    }

    for ( i = 0; i < 100; i++)
    {
        moto=1;
        delay(4000);
        moto=0;
        delay(6000);
    }

    for ( i = 0; i < 100; i++)
    {
        moto=1;
        delay(2000);
        moto=0;
        delay(8000);
    }

    moto=0;
    while (1)
    {
    }
}