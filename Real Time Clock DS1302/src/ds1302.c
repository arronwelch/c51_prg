#include "reg52.h"
#include "intrins.h"//声明void _nop_(void);

sbit CE = P3^5;
sbit SCLK = P3^6;
sbit IO = P3^4;

typedef unsigned char u8;
typedef unsigned int u16;

//Seconds,Minutes,Hour,Date,Month,Day,Year
u8 RTCRegAddrW[7]={0X80,0X82,0X84,0X86,0X88,0X8A,0X8C};
u8 RTCRegAddrR[7]={0X81,0X83,0X85,0X87,0X89,0X8B,0X8D};

u8 Ds130Time[7]={0,0,12,25,3,4,21};

bit Ds1302SetTime()
{
    u8 i;
    Ds1302Write(0x8e,0x00);//disable write protection
    if (0x00!=Ds1302Read(0x8e))
    {
        return 0;
    }
    else
    {
        for(i=0;i<8;i++)
        {
            Ds1302Write(RTCRegAddrW[i],Ds130Time[i]);
        }
        return 1;
    }
}

u8 Ds1302Read(u8 addr)
{
    u8 i,dat;
    CE = 0;
    _nop_();
    SCLK = 0;
    _nop_();
    CE = 1;
    _nop_();
    for(i=0;i<8;i++)
    {
        IO = addr&0x01;//LSB
        addr>>=1;
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }
    for(i=0;i<8;i++)
    {
        dat = (dat>>1)|(IO<<7);
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }
    CE = 0;
    _nop_();
    SCLK = 0;
    _nop_();
    SCLK = 1;
    _nop_();
    return dat;
}

void Ds1302Write(u8 addr,u8 dat)
{
    u8 i;
    CE = 0;
    _nop_();
    SCLK = 0;
    _nop_();
    CE = 1;
    _nop_();
    for(i=0;i<8;i++)
    {
        IO = addr&0x01;//LSB
        addr>>=1;
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }    
    for(i=0;i<8;i++)
    {
        IO = dat&0x01;//LSB
        addr>>=1;
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }
    CE = 0;
    _nop_();
    SCLK = 0;
    _nop_();
    SCLK = 1;
    _nop_();
}


