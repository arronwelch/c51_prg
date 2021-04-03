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



//dynamic 8-seg  LED display BIT select
sbit LSA = P2 ^ 2;
sbit LSB = P2 ^ 3;
sbit LSC = P2 ^ 4;

#define GPIO_SEGCODE P0 //dynamic 8-seg LED display DECODE select GPIO

u8 code NsegCode[16] = 
// '0'  '1'   '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'
 {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
// 'A'  'b'   'C'   'd'   'E'   'F'
  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

u8 SegDisplayData[4]={0};

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
        GPIO_SEGCODE = SegDisplayData[i];
        delay(100); //delay 1ms
        GPIO_SEGCODE = 0X00;  //clear
    }
}

void Set_RTC(void)
{
    u8 i,*p,tmp;
    DEC2BCD(rtc_time_set);

}

void main()
{

    while (1)
    {

    }
}