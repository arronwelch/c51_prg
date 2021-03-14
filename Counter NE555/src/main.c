#include <REG52.H>

typedef unsigned char u8;
typedef unsigned int u16;

//dynamic 8-seg LED display BIT select
sbit LSA = P2 ^ 2;
sbit LSB = P2 ^ 3;
sbit LSC = P2 ^ 4;

#define GPIO_SEGCODE P0 //dynamic 8-seg LED display DECODE select GPIO

u8 code NsegCode[16] = 
// '0'  '1'   '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'
 {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
// 'A'  'b'   'C'   'd'   'E'   'F'
  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

unsigned long TimeCount;//unit,second
unsigned long Freq;
u8 SegDisplayData[8]={0};

sbit buzzer = P1 ^ 5;
sbit NE555 = P3 ^ 5;

void delay(u16 i)
{
    while (i--);
}

void CounterTimer_Config()
{
    TMOD=0X51;//counter1 mode 1:16 bit counter,timer0 mode 1:16 bit timer

    TH0=0X3C;//setting Timer0 init value:50ms(2^16-50000 = 0x3CB0)
    TL0=0XB0;

    TH1=0;
    TL1=0;

    ET0=1;//enable Timer0 interrupt allow
    ET1=1;//enable Counter1 interrupt allow

    EA=1;//enable Total interruption
    
    TR0=1;//enable Timer0
    TR1=1;//enable Counter1
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

void main()
{
    GPIO_SEGCODE = 0X00;  //clear
    TimeCount=0;
    Freq=0;
    CounterTimer_Config();//init Counter/Timer0 and Counter/Timer1
    while (1)
    {
        if ((TR1==0) && (TR0==0))
        {
            Freq=Freq+(TH1<<8)+TL1;

            SegDisplayData[0]=NsegCode[Freq%1000000/100000];        //100,000
            SegDisplayData[1]=NsegCode[Freq%100000/10000];        //10,000
            SegDisplayData[2]=NsegCode[Freq%10000/1000];        //1,000
            SegDisplayData[3]=NsegCode[Freq%1000/100];        //100
            SegDisplayData[4]=NsegCode[Freq%100/10];        //10
            SegDisplayData[5]=NsegCode[Freq%10/1];        //1

            Freq=0;
            TH1=0;
            TL1=0;

            TH0=0X3C;//setting Timer0 init value:50ms(2^16-50000 = 0x3CB0)
            TL0=0XB0;
            TimeCount=0;

            TR0=1;
            TR1=1;
        }
        DigDisplay();
        buzzer = NE555;
    }
}

void Timer0() interrupt 1
{
    TH0=0X3C;//setting Timer0 init value:50ms(2^16-50000 = 0x3CB0)
    TL0=0XB0;
    TimeCount++;
    if (TimeCount==20)//1s
    {
        TimeCount=0;
        TR0=0;
        TR1=0;
    }
}

void Counter1() interrupt 3
{
    Freq=Freq+65536;
}