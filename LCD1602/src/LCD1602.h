
#ifndef __LCD1602_H__
#define __LCD1602_H__

#define LCD1602_4PINS

#include "reg52.h"

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

#ifndef ulong
#define ulong unsigned long
#endif

#define LCD1602_DATAPINS P0
sbit LCD1602_E  = P2^7;// lcd1602 enable
sbit LCD1602_RW = P2^5;// read(1)/write(0)
sbit LCD1602_RS = P2^6;// send data(1)/cmd(0)

void Lcd1602_Delay1ms(uint c);
void LcdWriteCom(uchar com);
void LcdWriteData(uchar dat);
void LcdInit();

#endif