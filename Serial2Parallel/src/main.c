#include <REG52.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit SRCLK = P3 ^ 6; //shift register clock
sbit DRCLK = P3 ^ 5; //8-bit D-type storage register clock
sbit SER = P3 ^ 4;   //serial

void delay(u16 i)
{
    while (i--)
        ;
}

/*
 * 74HC595
 * 8-Bit Shift Registers with Output Latches
 */
void HC595SendByte(u8 dat)
{
    u8 a;
    for (a = 0; a < 8; a++)
    {
        SER = dat >> 7;
        dat <<= 1;

        SRCLK = 0;
        _nop_();
        _nop_();
        SRCLK = 1; //Transition from low to high level
    }
    DRCLK = 0;
    _nop_();
    _nop_();
    DRCLK = 1; //Transition from low to high level
}

void main()
{
    //u8 i;
    //while (1)
    //{
    //    for ( i = 0; i < 8; i++)
    //    {
    //        HC595SendByte(0x01<<i);
    //        delay(50000);
    //    }
    //}

    u8 ledNum = 0x01;
    while (1)
    {
        HC595SendByte(ledNum);
        ledNum = _crol_(ledNum, 1);
        delay(50000);
    }
}