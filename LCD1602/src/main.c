
#include "reg52.h"
#include "LCD1602.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 Disp[16] = " Pechin Science ";

void main(void)
{
    u8 i;
    // u8 dat = 1;

    LcdInit();
    // dat = dat + 0x30;//transform to ASCII
    // LcdWriteData(dat);

    // LcdWriteData("1");

    for ( i = 0; i < 16; i++)
    {
        LcdWriteData(Disp[i]);
    }
    
    while (1);
}