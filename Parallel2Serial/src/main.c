#include <REG52.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

#define GPIO_LED P0

sbit SH_LD = P1 ^ 6;
sbit CLK = P3 ^ 6;
sbit QH = P1 ^ 7;

/*
 * 74HC165
 * Parallel-in/Serial-out 8-Bit Shift Register
 */
u8 Read74HC165()
{
    u8 indata = 0;
    u8 i;

    SH_LD = 0; //Load Parallel in data
    _nop_();
    SH_LD = 1;
    _nop_(); //Shift mode

    for (i = 0; i < 8; i++)
    {
        indata = indata << 1; //must in start,loop 8 times,left shift 7 times
        CLK = 0;
        _nop_();
        indata |= QH; //0x80
        CLK = 1;      //Transition from low to high level
    }
    return indata;
}

void main()
{
    u8 h165Value;
    GPIO_LED = 0; //clear
    while (1)
    {
        h165Value = Read74HC165();
        if (h165Value != 0xFF)
            GPIO_LED = ~h165Value;
    }
}