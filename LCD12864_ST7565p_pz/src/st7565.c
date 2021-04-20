
#include "st7565.h"

void delay_ms(uint t)
{
    uint i,j;
    for ( i = 0; i < 99; i++)
    {
        for ( j = 0; j < t; j++);
    }
}

void LcdSt7565_WriteCmd(uchar cmd)
{
    LCD12864_CS = 0;
    LCD12864_RS = 0;//select command mode
    LCD12864_RD = 1;//disable read
    LCD12864_WR = 0;//enable write
    _nop_();
    _nop_();

    DATA_PORT = cmd;
    _nop_();
    _nop_();

    LCD12864_WR = 1;//disable write
}

void LcdSt7565_WriteData(uchar dat)
{
    LCD12864_CS = 0;
    LCD12864_RS = 1;//select data mode
    LCD12864_RD = 1;//disable read
    LCD12864_WR = 0;//enable write
    _nop_();
    _nop_();

    DATA_PORT = dat;
    _nop_();
    _nop_();

    LCD12864_WR = 1;//disable write
}

void Lcd12864_Init(void)
{
    LCD12864_RSET = 0;
    delay_ms(1);//delay
    LCD12864_CS = 0;
    LCD12864_RSET = 1;

    LcdSt7565_WriteCmd(0xE2);//reset st7565
    delay_ms(1);

    //0xA0,Narmal(SEG0-SEG131) 0xA1,Reverse(SEG131-SEG0);
    LcdSt7565_WriteCmd(0xA1);//ADC Select (Segment Driver Direction Select)

    //0xC0,Narmal(COM0→COM63) 0xC8,Reverse(COM63→COM0);
    LcdSt7565_WriteCmd(0xC8);//Common Output Mode Select

    //0xA6,normal:RAM Data “H” 0xA7,reverse:RAM Data “L”
    LcdSt7565_WriteCmd(0xA6);//Display Normal/Reverse

    //0xA4,Normal display mode;0xA5,Display all points ON
    LcdSt7565_WriteCmd(0xA4);//Display All Points ON/OFF

    //0xA2,1/9 bias; 0xA3,1/7 bias
    LcdSt7565_WriteCmd(0xA2);//LCD Bias Set

    //The Booster Ratio (Double Byte Command)
    LcdSt7565_WriteCmd(0xF8);//Booster Ratio Select Mode Set
    LcdSt7565_WriteCmd(0x01);//5x

    //The Electronic Volume (Double Byte Command)
	LcdSt7565_WriteCmd(0x81);//Set the V0 output voltage electronic volume register
	LcdSt7565_WriteCmd(0x23);//0x00-0x3F

    //V0 Voltage Regulator Internal Resistor Ratio Set
    LcdSt7565_WriteCmd(0x25);//Select internal resistor ratio(Rb/Ra) mode
    
    //Power control set
    LcdSt7565_WriteCmd(0x2F);//Select internal power supply operating mode
    delay_ms(1);

     //Display start line set
     LcdSt7565_WriteCmd(0x40);//Sets the display RAM display start line address

     //Display ON/OFF
     LcdSt7565_WriteCmd(0xAF);
     delay_ms(1);
}

void Lcd12864_ClearScreen(void)
{
    uchar page,x;

    for ( page = 0; page < 8; page++)
    {
        //Page address set
        LcdSt7565_WriteCmd(0xB0+page);//Sets the display RAM page address
        
        //0-131 0x00,0x83 132bit ST7565 default
        //4-131 0x04,0x83 128bit LCD12864 use address!
        //Column address set upper bit[7:4]
        LcdSt7565_WriteCmd(0x10+0x00);
        //Column address set lower bit[3:0]
        LcdSt7565_WriteCmd(0x00+0x04);

        for(x=0;x<128;x++)
        {
            LcdSt7565_WriteData(0x00);//clear screen dots(128*64)
        }
    }
}


#ifdef  CHAR_CODE

#include"charcode.h"

uchar Lcd12864_Write16CnCHAR(uchar x, uchar y, uchar *cn)
{
	uchar j, x1, x2, wordNum;

	//--Y的坐标只能从0到7，大于则直接返回--//
	if(y > 7)
	{
		return 0;
	}

	//--X的坐标只能从0到128，大于则直接返回--//
	if(x > 128)
	{
		return 0;
	}
	y += 0xB0;	   //求取Y坐标的值
	//--设置Y坐标--//
	LcdSt7565_WriteCmd(y);
	while ( *cn != '\0')	 //在C语言中字符串结束以‘\0’结尾
	{	
	
		//--设置Y坐标--//
		LcdSt7565_WriteCmd(y);

		x1 = (x >> 4) & 0x0F;   //由于X坐标要两句命令，分高低4位，所以这里先取出高4位
		x2 = x & 0x0F;          //去低四位
		//--设置X坐标--//
		LcdSt7565_WriteCmd(0x10 + x1);   //高4位
		LcdSt7565_WriteCmd(0x04 + x2);	//低4位

        wordNum=8;
        for (j=0; j<32; j++) //写一个字
        {		
            if (j == 16)	 //由于16X16用到两个Y坐标，当大于等于16时，切换坐标
            {
                //--设置Y坐标--//
                LcdSt7565_WriteCmd(y + 1);
                //--设置X坐标--//
                LcdSt7565_WriteCmd(0x10 + x1);  //高4位
                LcdSt7565_WriteCmd(0x04 + x2);	//低4位
            }
            LcdSt7565_WriteData(CN16CHAR[wordNum].Msk[j]);
        }
        x += 16;
        wordNum--;
        if(wordNum==0)
        {
            wordNum=8;
        }
	}	//while结束
	return 1;
}

#endif


