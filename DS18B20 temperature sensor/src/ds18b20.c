#include "ds18b20.h"

void Delay1ms(uint y)
{
    uint x;
    for(;y>0;y--)
    {
        for(x=110;x>0;x--);
    }
}

uchar Ds18b20Init()
{
    uchar i=0;
    DQ = 0;//1.pull down
    i=70;
    while(i--);//2.delay 642us
    DQ = 1;//3.pull up
    i=0;
    while(DQ)
    {
        Delay1ms(1);//delay 5ms,exit loop
        i++;
        if (i>5)
        {
            return 0;//init fail
        }
    }
    return 1;//init pass
}

void Ds18b20WriteByte(uchar dat)
{
    uchar i,j;
    for(j=0;j<8;j++)
    {
        DQ = 0;
        i++;
        DQ = dat & 0x01;
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
        DQ = 1;
        i++;
        i++;
        bi = DQ;
        byte = (byte>>1) | (bi<<7);
        i=4;
        i--;
    }
    return byte;
}

void Ds18b20ChangeTemp()
{
    Ds18b20Init();
    Delay1ms(1);
    Ds18b20WriteByte(0xcc);
    Ds18b20WriteByte(0x44);
}

void Ds18b20ReadTempCom()
{
    Ds18b20Init();
    Delay1ms(1);
    Ds18b20WriteByte(0xcc);
    Ds18b20WriteByte(0xbe);
}

int Ds18b20ReadTemp()
{
    int temp=0;
    uchar tmh,tml;
    Ds18b20ChangeTemp();
    Ds18b20ReadTempCom();
    tml = Ds18b20ReadByte();
    tmh = Ds18b20ReadByte();
    temp = tmh;
    temp<<=8;
    temp|=tml;
    return temp;
}