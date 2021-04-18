
#include "reg52.h"
#include "st7565.h"


void main(void)
{
    uchar i;
    Lcd12864_Init();
    Lcd12864_ClearScreen();

    while(1)
    {
        Lcd12864_Write16CnCHAR(0, i, "司公限有技科中普");
    }
}