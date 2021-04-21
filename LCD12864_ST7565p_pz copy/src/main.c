
#include "reg52.h"
#include "st7565.h"

void Delay10ms(unsigned int c)   //误差 0us
{
    unsigned char a,b;
    for(;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}

void main(void)
{
    uchar i,j;
    Lcd12864_Init();
    Lcd12864_ClearScreen();

//-----------------------128*64-----------------------//
// COM63
// COM62
// ...
// COM02
// COM01
// COM00
//      SEG000 SEG001 SEG002 ... SEG126 SEG127 ... SEG131
//-----------------------128*64-----------------------//

    //0xA0,Narmal(SEG0-SEG131) 0xA1,Reverse(SEG131-SEG0);
    LcdSt7565_WriteCmd(0xA0);//ADC Select (Segment Driver Direction Select)

    //0xC0,Narmal(COM0→COM63) 0xC8,Reverse(COM63→COM0);
    LcdSt7565_WriteCmd(0xC8);//Common Output Mode Select

    //--从左到右,从上到下,慢慢刷黑屏幕--//
    for (i=0; i<8; i++)
    {
        LcdSt7565_WriteCmd(0xB0+i); //设置Y坐标，具体可以看清屏函数里面的说明

        LcdSt7565_WriteCmd(0x10);   //设置X坐标，具体可以看清屏函数里面的说明
        LcdSt7565_WriteCmd(0x00);//0 ... 127
        for (j=0; j<128; j++)
        {
            LcdSt7565_WriteData(0xFF);  //如果设置背景为白色时，清屏选择0XFF
            Delay10ms(2);	            //延时一下
        }
    }

    //0xA0,Narmal(SEG0-SEG131) 0xA1,Reverse(SEG131-SEG0);
    LcdSt7565_WriteCmd(0xA1);//ADC Select (Segment Driver Direction Select)

    //0xC0,Narmal(COM0→COM63) 0xC8,Reverse(COM63→COM0);
    LcdSt7565_WriteCmd(0xC0);//Common Output Mode Select

    //--从右到左,从下到上,慢慢刷白屏幕--//
    for (i=0; i<8; i++)
    {
        LcdSt7565_WriteCmd(0xB0+i); //设置Y坐标，具体可以看清屏函数里面的说明

        LcdSt7565_WriteCmd(0x10);   //设置X坐标，具体可以看清屏函数里面的说明
        LcdSt7565_WriteCmd(0x04);//0 ... 127(0x04) 128(0x03) 129(0x02) 130(0x01) 131(0x00)
        for (j=0; j<128; j++)
        {
            LcdSt7565_WriteData(0x00);  //如果设置背景为白色时，清屏选择0XFF
            Delay10ms(2);			    //延时一下
        }
    }	

    // for (i=0; i<8; i += 2)
    // {
    //     Lcd12864_Write16CnCHAR(0, i,"中文测试字符输出");
    // }

    //0xA0,Narmal(SEG0-SEG131) 0xA1,Reverse(SEG131-SEG0);
    LcdSt7565_WriteCmd(0xA0);//ADC Select (Segment Driver Direction Select)

    //0xC0,Narmal(COM0→COM63) 0xC8,Reverse(COM63→COM0);
    LcdSt7565_WriteCmd(0xC8);//Common Output Mode Select

    Lcd12864_Write16CnCHAR(0, 0,"中文测试字符输出");
    Lcd12864_Write16CnCHAR(0, 2,"常用汉字显示测试");

    while(1)
    {

    }
}