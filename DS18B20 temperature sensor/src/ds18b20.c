#include "ds18b20.h"
#include "intrins.h"

void delay_us(uchar t)
{
    while (t--)
    {
        _nop_();
    }
}

uchar Ds18b20Init()
{
    DQ = 0;
    delay_us(480);
    DQ = 1;
    delay_us(60);
    if(!DQ)
    {
        delay_us(240);
        return 1;
    }
    else
    {
        return 0;
    }
}

void Ds18b20WriteByte(uchar dat)
{
    uchar i,j;
    for(j=0;j<8;j++)
    {
        DQ = 0;
        i++;
        DQ=dat&0x01;
        i=6;
        while(i--);//delay 68us
        DQ = 1;
        dat>>=1;
    }
}

uchar Ds18b20ReadByte()
{
    uint i,j;
    uchar bi,byte;
    for(j=8;j>0;j--)
    {
        DQ = 0;
        i++;
        i++;
        DQ = 1;
        i++;
        i++;
        bi = DQ;
        byte = (byte>>1) | (bi<<7);
        i=4;
        while(i--);
    }
    return byte;
}

void Ds18b20ChangeTemp()
{
    Ds18b20Init();
    Ds18b20WriteByte(0xcc);
    Ds18b20WriteByte(0x44);
}

void Ds18b20ReadTempCom()
{
    Ds18b20Init();
    Ds18b20WriteByte(0xcc);
    Ds18b20WriteByte(0xbe);
}

float Ds18b20ReadTemp()
{
    uchar tmh,tml;
    Ds18b20ChangeTemp();
    Ds18b20ReadTempCom();
    tml = Ds18b20ReadByte();
    tmh = Ds18b20ReadByte();
    return ((tmh << 8) + tml) * 0.0625;
}