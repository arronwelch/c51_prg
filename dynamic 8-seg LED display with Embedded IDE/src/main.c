#include <REG52.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit LSA = P2 ^ 2;
sbit LSB = P2 ^ 3;
sbit LSC = P2 ^ 4;

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
//  '0'  '1'   '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'
u8 code NsegCode[16] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
                        //  'A'  'b'   'C'   'd'   'E'   'F'
                        0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

void delay(u16 t)
{
    while (t--)
        ;
}

void DigDisplay()
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        switch (i)
        {
        case (0):
        {
            LSA = 0;
            LSB = 0;
            LSC = 0;
        }
        break;
        case (1):
        {
            LSA = 1;
            LSB = 0;
            LSC = 0;
        }
        break;
        case (2):
        {
            LSA = 0;
            LSB = 1;
            LSC = 0;
        }
        break;
        case (3):
        {
            LSA = 1;
            LSB = 1;
            LSC = 0;
        }
        break;
        case (4):
        {
            LSA = 0;
            LSB = 0;
            LSC = 1;
        }
        break;
        case (5):
        {
            LSA = 1;
            LSB = 0;
            LSC = 1;
        }
        break;
        case (6):
        {
            LSA = 0;
            LSB = 1;
            LSC = 1;
        }
        break;
        case (7):
        {
            LSA = 1;
            LSB = 1;
            LSC = 1;
        }
        break;
        default:
            break;
        }
        P0 = NsegCode[1+i];
        delay(100); //delay 1ms
        P0 = 0X00;  //clear
    }
}

void DigDisplay1()
{
    u8 i;
    for (i = 0; i < 8; i++)
    {   
        LSA = i&0x01;
        LSB = (i&0x02)>>1;
        LSC = (i&0x04)>>2;
        P0 = NsegCode[1+i];
        delay(100); //delay 1ms
        P0 = 0X00;  //clear
    }
}

void main()
{
    while (1)
    {
        DigDisplay1();
    }
}