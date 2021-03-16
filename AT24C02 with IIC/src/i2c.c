#include "i2c.h"

void Delay10us()		//@12.000MHz
{
	unsigned char i;

	i = 27;
	while (--i);
}

void I2cStart()
{
    SDA = 1;
    Delay10us();
    SCL = 1;
    Delay10us();
    SDA = 0;
    Delay10us();
    SCL = 0;
    Delay10us();
}

void I2cStop()
{
    SDA = 0;
    Delay10us();
    SCL = 1;
    Delay10us();
    SDA = 1;
    Delay10us();
}

unsigned char I2cSendByte(unsigned char dat)
{
    unsigned char a=0,b=0;
    for(a=0;a<8;a++)
    {
        SDA=dat>>7;
        dat=dat<<1;
        Delay10us();
        SCL = 1;
        Delay10us();
        SCL = 0;
        Delay10us();
    }
    SDA = 1;
    Delay10us();
    SCL = 1;
    Delay10us();
    while (SDA)
    {
        b++;
        if (b>200)
        {
            SCL = 0;
            Delay10us();
            return 0;//transimt fail
        }
    }
    SCL = 0;
    Delay10us();
    return 1;//transimt complete
}

unsigned char I2cReadByte()
{
    unsigned char a=0,dat=0;
    SDA = 1;
    Delay10us();
    for(a=0;a<8;a++)
    {
        SCL=1;
        Delay10us();
        dat<<=1;
        dat|=SDA;
        Delay10us();
        SCL=0;
        Delay10us();
    }
    return dat;
}

void At24c02Write(unsigned char addr,unsigned char dat)
{
    I2cStart();
    I2cSendByte(0xA0);
    I2cSendByte(addr);
    I2cSendByte(dat);
    I2cStop();
}

unsigned char At24c02Read(unsigned char addr)
{
    unsigned char dat=0;
    I2cStart();
    I2cSendByte(0xA0);
    I2cSendByte(addr);
    I2cStart();
    I2cSendByte(0xA1);
    dat = I2cReadByte();
    I2cStop();
    return dat;
}