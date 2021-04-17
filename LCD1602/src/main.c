
#include "reg52.h"
#include "LCD1602.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 Disp[16] = " Pechin Science ";

void main(void)
{
    u8 i;
    u8 dat = 1;
    dat = dat + 0x30;//transform to ASCII

    LcdInit();
    for ( i = 0; i < 16; i++)
    {
        LcdWriteData(Disp[i]);
        //LcdWriteData(dat);
        //LcdWriteData('A');
    }
    LcdWriteCom(0x40+0x80);
    for ( i = 0; i < 16; i++)
    {
        //LcdWriteData(Disp[i]);
        //LcdWriteData(dat);
        LcdWriteData('B');
    }
    while (1);
}