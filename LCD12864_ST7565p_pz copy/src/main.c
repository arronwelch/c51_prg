
#include "reg52.h"
#include "st7565.h"


void main(void)
{
    uchar i=0;
    Lcd12864_Init();
    Lcd12864_ClearScreen();

    // for (i=0; i<8; i += 2)
    // {
    //     Lcd12864_Write16CnCHAR(0, i,"中文测试字符输出");
    // }

    Lcd12864_Write16CnCHAR(0, i,"中文测试字符输出");
    Lcd12864_Write16CnCHAR(0, i+2,"常用汉字显示测试");

    while(1)
    {

    }
}