
//Lcd12864. pz

#ifndef __ST7565_H__
#define __ST7565_H__

#include "reg52.h"
#include "intrins.h" //_nop_()

//---包含字库头文件
#define CHAR_CODE

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

sbit LCD12864_RSET  = P3^3;//reset
sbit LCD12864_CS    = P3^2;//chip select
#define DATA_PORT P0       //D7 - D0
sbit LCD12864_WR    = P2^7;// /write         LCDE //P2.7
sbit LCD12864_RD    = P2^5;// /read          WR   //P2.5
sbit LCD12864_RS    = P2^6;// (A0) data/cmd  RD   //P2.6

void delay_ms(uint t);
void LcdSt7565_WriteCmd(uchar cmd);
void LcdSt7565_WriteData(uchar dat);
void Lcd12864_Init(void);

void Lcd12864_ClearScreen(void);
uchar Lcd12864_Write16CnCHAR(uchar x, uchar y, uchar *cn);

#endif