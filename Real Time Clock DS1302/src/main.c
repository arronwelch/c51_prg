#include <REG52.H>
#include <INTRINS.H>

sbit SCK = P3^6;
sbit SDA = P3^4;
sbit RST = P3^5;

sbit LS138A = P2^2;
sbit LS138B = P2^3;
sbit LS138C = P2^4;

bit ReadRTC_Flag;

unsigned char l_tmpdate[7]={0,0,12,15,5,3,8};
unsigned char l_tmpdisplay[8];

code unsigned char write_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};
code unsigned char read_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};

code unsigned char table[11]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};
//0-9,'-'


void Write_Ds1302_Byte(unsigned char temp);
void Write_Ds1302(unsigned char address,unsigned char dat);
unsigned char Read_Ds1302(unsigned char address);
void Read_RTC(void);
void Set_RTC(void);
void InitTimer0(void);



void main()
{
    InitTimer0();
    Set_RTC();
    while (1)
    {
        if(ReadRTC_Flag)
        {
            ReadRTC_Flag =0;
            Read_RTC();

            l_tmpdisplay[0] = l_tmpdate[2]/16;
            l_tmpdisplay[1] = l_tmpdate[2]&0x0f;
            l_tmpdisplay[2] = 10;//'-'
            l_tmpdisplay[3] = l_tmpdate[1]/16;
            l_tmpdisplay[4] = l_tmpdate[1]&0x0f;
            l_tmpdisplay[5] = 10;//'-'
            l_tmpdisplay[6] = l_tmpdate[0]/16;
            l_tmpdisplay[7] = l_tmpdate[0]&0x0f;
        }
    }
}

void InitTimer0(void)
{
    TMOD |= 0X01;//timer0 mode1 16bit
    TH0 = 0xef;
    TL0 = 0xf0;
    ET0 = 1;
    TR0 = 1;
    EA = 1;
}

void Write_Ds1302_Byte(unsigned char temp)
{
    unsigned char i;
    for(i=0;i<8;i++)
    {
        SCK = 0;
        SDA = temp&0x01;//LSB
        temp >>= 1;
        SCK = 1;
    }
}

void Write_Ds1302(unsigned char address,unsigned char dat)
{
    RST = 0;
    _nop_();
    SCK = 0;
    _nop_();
    RST = 1;
    _nop_();
    Write_Ds1302_Byte(address);
    Write_Ds1302_Byte(dat);
    RST = 0;
}

unsigned char Read_Ds1302(unsigned char address)
{
    unsigned char i,temp=0x00;
    RST = 0;
    _nop_();
    _nop_();
    SCK = 0;
    _nop_();
    _nop_();
    RST = 1;
    _nop_();
    _nop_();
    Write_Ds1302_Byte(address);
    for(i=0;i<8;i++)
    {
        if (SDA)
            temp|=0X80;
        SCK = 0;
        temp >>= 1;//only save bit0 to bit6,ignore bit7
        _nop_();
        _nop_();
        _nop_();
        SCK = 1;
    }
    RST = 0;
    _nop_();
    _nop_();
    RST = 0;
    SCK = 0;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    SCK = 1;
    _nop_();
    _nop_();
    SDA = 0;
    _nop_();
    _nop_();
    SDA = 1;
    _nop_();
    _nop_();
    return temp;
}

void Read_RTC(void)
{
    unsigned char i,*p;
    p=read_rtc_address;
    for(i=0;i<7;i++)
    {
        l_tmpdate[i]=Read_Ds1302(*p);
        p++;
    }
}

void Set_RTC(void)
{
    unsigned char i,*p,tmp;
    for(i=0;i<7;i++)
    {
        tmp=l_tmpdate[i]/10;
        l_tmpdate[i]=l_tmpdate[i]%10;
        l_tmpdate[i]=l_tmpdate[i]+16*tmp;
    }
    Write_Ds1302(0x8e,0x00);//disable write Protection

    p=write_rtc_address;
    for(i=0;i<7;i++)//Seconds Minutes Hour(24) Date Month DayofWeek Year
    {
        Write_Ds1302(*p,l_tmpdate[i]);
        p++;
    }
    Write_Ds1302(0x8e,0x80);//disable write Protection
}

void tim(void) interrupt 1 using 1
{
    static unsigned char i,num;
    TH0 = 0xf5;
    TL0 = 0xe0;

    P0 = table[l_tmpdisplay[i]];

    switch (i)
    {
    case 0:LS138A=0;LS138B=0;LS138C=0;break;
    case 1:LS138A=0;LS138B=0;LS138C=1;break;
    case 2:LS138A=0;LS138B=1;LS138C=0;break;
    case 3:LS138A=0;LS138B=1;LS138C=1;break;
    case 4:LS138A=1;LS138B=0;LS138C=0;break;
    case 5:LS138A=1;LS138B=0;LS138C=1;break;
    case 6:LS138A=1;LS138B=1;LS138C=0;break;
    case 7:LS138A=1;LS138B=1;LS138C=1;break;
    }

    i++;
    if (i==8)
    {
        i=0;
        num++;
        if (10==num)
        {
            ReadRTC_Flag = 1;
            num=0;
        }
    }
}
