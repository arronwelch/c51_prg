#include <REG52.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit SRCLK = P3 ^ 6; //shift register clock
sbit DRCLK = P3 ^ 5; //8-bit D-type storage register clock
sbit SER = P3 ^ 4;   //serial

/*
 * 74HC595
 * 8-Bit Shift Registers with Output Latches
 */
void HC595Send4Byte(u8 dat1, u8 dat2, u8 dat3, u8 dat4)
{
    u8 a;
    for (a = 0; a < 8; a++)
    {
        SER = dat1 >> 7;
        dat1 <<= 1;

        SRCLK = 0;
        _nop_();
        _nop_();
        SRCLK = 1; //Transition from low to high level
    }

    for (a = 0; a < 8; a++)
    {
        SER = dat2 >> 7;
        dat2 <<= 1;

        SRCLK = 0;
        _nop_();
        _nop_();
        SRCLK = 1; //Transition from low to high level
    }

    for (a = 0; a < 8; a++)
    {
        SER = dat3 >> 7;
        dat3 <<= 1;

        SRCLK = 0;
        _nop_();
        _nop_();
        SRCLK = 1; //Transition from low to high level
    }

    for (a = 0; a < 8; a++)
    {
        SER = dat4 >> 7;
        dat4 <<= 1;

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

// 8*8 Dot Matrix:(0,0)
//************************************************//
// 1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//D0 1 POS01
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//D1 0 POS02
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//D2 0 POS03
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//D3 0 POS04
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//D4 0 POS05
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//D5 0 POS06
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//D6 0 POS07
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//D7 0 POS08
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//C0 0 POS09
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//C1 0 POS10
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//C2 0 POS11
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//C3 0 POS12
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//C4 0 POS13
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//C5 0 POS14
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//C6 0 POS15
// 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 *//C7 0 POS16
//*************************************************//
//B0 B1 B2 B3 B4 B5 B6 B7 A0 A1 A2 A3 A4 A5 A6 A7
// 0  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1
// N  N  N  N  N  N  N  N  N  N  N  N  N  N  N  N
// E  E  E  E  E  E  E  E  E  E  E  E  E  E  E  E
// G  G  G  G  G  G  G  G  G  G  G  G  G  G  G  G
// 1  2  3  4  5  6  7  8  9  1  1  1  1  1  1  1
//                            0  1  2  3  4  5  6
// (A,B,C,D)
// (0xff,0xfe,0x00,0x01)

void main()
{
    HC595Send4Byte(0xff, 0xfe, 0x00, 0x01);
    while (1)
    {
    }
}