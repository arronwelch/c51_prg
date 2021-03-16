#ifndef _DS18B20_H
#define _DS18B20_H

#include <reg52.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

sbit DQ = P3^7;
int Ds18b20ReadTemp();

#endif