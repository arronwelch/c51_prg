#include <REG52.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit SCK = P3^6;//system clock
sbit SDA = P3^4;//data input/output
sbit RST = P3^5;//chip enable

code u8 rtc_time_set[7]={0,0,0,4,4,7,21};//2021-4-4-0-0-0 Sunday
code u8 write_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};
code u8 read_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};

u8 l_tmpdate[7];

//dynamic 8-seg  LED display BIT select
sbit LSA = P2 ^ 2;
sbit LSB = P2 ^ 3;
sbit LSC = P2 ^ 4;

#define GPIO_SEGCODE P0 //dynamic 8-seg LED display DECODE select GPIO

u8 code NsegCode[17] = 
// '0'  '1'   '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'
 {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
// 'A'  'b'   'C'   'd'   'E'   'F'  '-'
  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71,0x40};

u8 SegDisplayData[8]={0};

void delay(u16 t)
{
    while (t--)
        ;
}

void DigDisplay()
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        switch (i)
        {
        case (0):{LSA = 0;LSB = 0;LSC = 0;}break;
        case (1):{LSA = 1;LSB = 0;LSC = 0;}break;
        case (2):{LSA = 0;LSB = 1;LSC = 0;}break;
        case (3):{LSA = 1;LSB = 1;LSC = 0;}break;
        case (4):{LSA = 0;LSB = 0;LSC = 1;}break;
        case (5):{LSA = 1;LSB = 0;LSC = 1;}break;
        case (6):{LSA = 0;LSB = 1;LSC = 1;}break;
        case (7):{LSA = 1;LSB = 1;LSC = 1;}break;
        default:break;
        }
        GPIO_SEGCODE =  NsegCode[SegDisplayData[i]];
        delay(100); //delay 1ms
        GPIO_SEGCODE = 0X00;  //clear
    }
}

u8* DEC2BCD(u8 *p)
{
    u8 i,dat[7];
    for(i=0;i<7;i++)
    {
        dat[i]=16*(*p)/10+(*p)%10;
    }
    return dat;
}

void Write_Ds1302_Byte(u8 byte)
{
    u8 i;
    for(i=0;i<8;i++)
    {
        SCK = 0;
        _nop_();
        SDA = byte&0X01;//LSB
        _nop_();
        SCK = 1;
        _nop_();
        byte >>=1;
    }
}

void Write_Ds1302(u8 addr,u8 dat)
{
    RST = 0;
    _nop_();
    SCK = 0;
    _nop_();
    RST = 1;
    _nop_();
    Write_Ds1302_Byte(addr);
    Write_Ds1302_Byte(dat);
    RST = 0;
}

u8 Read_Ds1302_Byte()
{
    u8 i,byte;
    for(i=0;i<8;i++)
    {
        if(SDA)
        byte |=0X80;
        SCK = 0;
        _nop_();
        SCK = 1;
        _nop_();        
    }
    return byte;
}

u8 Read_Ds1302(u8 addr)
{
    u8 dat;
    RST = 0;
    _nop_();
    SCK = 0;
    _nop_();
    RST = 1;
    _nop_();
    Write_Ds1302_Byte(addr);
    dat = Read_Ds1302_Byte();
    RST = 0;

    return dat;
}

void Read_RTC(void)
{
    u8 i,*addr,dat[7];
    addr = read_rtc_address;
    for(i=0;i<8;i++,addr++)
    {
        l_tmpdate[i]=Read_Ds1302(*addr);
    }
}

void Set_RTC(void)
{
    u8 i,*addr,*dat;
    dat = DEC2BCD(rtc_time_set);

    Write_Ds1302(0x8e,0x00);//diable DS1302 write protection

    addr = write_rtc_address;
    for(i=0;i<8;i++,addr++)
    {
        Write_Ds1302(*addr,*dat);
    }
    Write_Ds1302(0x8e,0x80);
}

void main()
{
    u8 *dat;
    Set_RTC();
    while (1)
    {
        Read_RTC();
        SegDisplayData[7]= l_tmpdate[0]%16;
        SegDisplayData[6]= l_tmpdate[0]|0x0f;

        SegDisplayData[5]= 16;

        SegDisplayData[4]= l_tmpdate[1]%16;
        SegDisplayData[3]= l_tmpdate[1]|0x0f;
        
        SegDisplayData[2]= 16;

        SegDisplayData[1]= l_tmpdate[2]%16;
        SegDisplayData[0]= l_tmpdate[2]|0x0f;

        DigDisplay();
    }
}