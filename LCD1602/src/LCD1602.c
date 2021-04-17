
#include "LCD1602.h"


void Lcd1602_Delay1ms(uint c)
{
    uchar a,b;
    for(;c>0;c--)
    {
        for(b=199;b>0;b--)
        {
            for(a=1;a>0;a--);
        }
    }
}

#ifndef LCD1602_4PINS
void LcdWriteCom(uchar com)
{
    LCD1602_E = 0;
    LCD1602_RS = 0;//select send cmd mode
    LCD1602_RW = 0;//select write mode

    LCD1602_DATAPINS = com;//send cmd
    Lcd1602_Delay1ms(1);//Waiting for the data to stabilize

    LCD1602_E = 1;//write enable
    Lcd1602_Delay1ms(5);//keep time
    LCD1602_E = 0;
}
#else
void LcdWriteCom(uchar com)
{
    LCD1602_E = 0;
    LCD1602_RS = 0;//select send cmd mode
    LCD1602_RW = 0;//select write mode

    LCD1602_DATAPINS = com;//send cmd
    Lcd1602_Delay1ms(1);//Waiting for the data to stabilize

    LCD1602_E = 1;//write enable
    Lcd1602_Delay1ms(5);//keep time
    LCD1602_E = 0;
}
#endif

#ifndef LCD1602_4PINS
void LcdWriteData(uchar dat)
{
    LCD1602_E = 0;
    LCD1602_RS = 1;//select send data mode
    LCD1602_RW = 0;//select write mode

    LCD1602_DATAPINS = dat;//send cmd
    Lcd1602_Delay1ms(1);//Waiting for the data to stabilize

    LCD1602_E = 1;//write enable
    Lcd1602_Delay1ms(5);//keep time
    LCD1602_E = 0;
}
#else
void LcdWriteData(uchar dat)
{
    LCD1602_E = 0;
    LCD1602_RS = 1;//select send data mode
    LCD1602_RW = 0;//select write mode

    LCD1602_DATAPINS = dat;//send cmd
    Lcd1602_Delay1ms(1);//Waiting for the data to stabilize

    LCD1602_E = 1;//write enable
    Lcd1602_Delay1ms(5);//keep time
    LCD1602_E = 0;
}
#endif

#ifndef LCD1602_4PINS
void LcdInit()
{
    LcdWriteCom(0x38);//databus = 8bit,row_num = 2; 5*7 char mode
    LcdWriteCom(0x0c);//enable display,disable curser
    LcdWriteCom(0x06);//write new data,curser right shift
    LcdWriteCom(0x01);//clear ddram(write 20H)
    LcdWriteCom(0x80);//setting data point start address
}
#else
void LcdInit()
{

}
#endif