#include <REG52.H>

typedef unsigned char u8;
typedef unsigned int u16;

//dynamic 8-seg LED display BIT select
sbit LSA = P2 ^ 2;
sbit LSB = P2 ^ 3;
sbit LSC = P2 ^ 4;

#define GPIO_SEGCODE P0 //dynamic 8-seg LED display DECODE select GPIO
#define GPIO_TRAFFIC P1 //traffic light GPIO

//sidewalk N-S
sbit RED_SW_NS = P1^0;//red in north-sourth sidewalk
sbit GRN_SW_NS = P1^1;//green in north-sourth sidewalk

//Roadway N-S
sbit RED_RW_NS = P1^2;//red in north-sourth Roadway
sbit YLW_RW_NS = P1^3;//yellow in north-sourth Roadway
sbit GRN_RW_NS = P1^4;//green in north-sourth Roadway

//Roadway E-W
sbit RED_RW_EW = P1^5;//red
sbit YLW_RW_EW = P1^6;//yellow
sbit GRN_RW_EW = P1^7;//green

//sidewalk E-W
sbit RED_SW_EW = P3^0;//red
sbit GRN_SW_EW = P3^1;//green

u8 code NsegCode[16] = 
// '0'  '1'   '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'
 {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
// 'A'  'b'   'C'   'd'   'E'   'F'
  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

u8 Second=0;
u8 SegDisplayData[8]={0};

void delay(u16 i)
{
    while (i--);
}

void Timer0Init()
{
    TMOD|=0X01;//mode 1:16 bit timwer

    TH0=0XFC;//1ms
    TL0=0X18;//2^16-1000

    ET0=1;
    EA=1;
    TR0=1;
}

void Timer1Init()
{
    TMOD|=0X10;//select timer1 mode 1:16 bit timwer,TR0 enable

    TH1=0XFC;//init value,delay 1ms
    TL1=0X18;//2^16-1000=64536=0xFC18

    ET1=1;//Timer1 interrupt enable
    EA=1;//enable interrupt gate
    TR1=1;//enable Timer1
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
    Second = 0;
    Timer0Init();
    while (1)
    {
        if(Second == 70)
        {
            Second = 0;
        }
        else if (Second < 30)
        {
            SegDisplayData[0]=0X00;
            SegDisplayData[1]=0X00;
            SegDisplayData[2]=NsegCode[(30-Second)/10];
            SegDisplayData[3]=NsegCode[(30-Second)%10];

            SegDisplayData[4]=0X00;
            SegDisplayData[5]=0X00;
            SegDisplayData[6]=SegDisplayData[2];
            SegDisplayData[7]=SegDisplayData[3];
            DigDisplay();

            GPIO_TRAFFIC=0XFF;
            GRN_SW_EW=1;
            RED_SW_EW=1;

            GRN_SW_NS=0;
            GRN_RW_NS=0;
            RED_SW_EW=0;
            RED_RW_EW=0;            
        }
        else if (Second<35)
        {
            SegDisplayData[0]=0X00;
            SegDisplayData[1]=0X00;
            SegDisplayData[2]=NsegCode[(35-Second)/10];
            SegDisplayData[3]=NsegCode[(35-Second)%10];

            SegDisplayData[4]=0X00;
            SegDisplayData[5]=0X00;
            SegDisplayData[6]=SegDisplayData[2];
            SegDisplayData[7]=SegDisplayData[3];
            DigDisplay();

            GPIO_TRAFFIC=0XFF;
            GRN_SW_EW=1;
            RED_SW_EW=1;

            GRN_SW_NS=0;
            YLW_RW_NS=0;
            RED_SW_EW=0;
            RED_RW_EW=0;     
        }
        else if (Second<65)
        {
            SegDisplayData[0]=0X00;
            SegDisplayData[1]=0X00;
            SegDisplayData[2]=NsegCode[(65-Second)/10];
            SegDisplayData[3]=NsegCode[(65-Second)%10];

            SegDisplayData[4]=0X00;
            SegDisplayData[5]=0X00;
            SegDisplayData[6]=SegDisplayData[2];
            SegDisplayData[7]=SegDisplayData[3];
            DigDisplay();

            GPIO_TRAFFIC=0XFF;
            GRN_SW_EW=1;
            RED_SW_EW=1;

            RED_SW_NS=0;
            RED_RW_NS=0;
            GRN_SW_EW=0;
            GRN_RW_EW=0;     
        }
        else if (Second<70)
        {
            SegDisplayData[0]=0X00;
            SegDisplayData[1]=0X00;
            SegDisplayData[2]=NsegCode[(70-Second)/10];
            SegDisplayData[3]=NsegCode[(70-Second)%10];

            SegDisplayData[4]=0X00;
            SegDisplayData[5]=0X00;
            SegDisplayData[6]=SegDisplayData[2];
            SegDisplayData[7]=SegDisplayData[3];
            DigDisplay();

            GPIO_TRAFFIC=0XFF;
            GRN_SW_EW=1;
            RED_SW_EW=1;

            RED_SW_NS=0;
            RED_RW_NS=0;
            GRN_SW_EW=0;
            YLW_RW_EW=0;     
        }
    }
}

void Timer0() interrupt 1
{
    static u16 i;
    TH0=0XFC;//2^16-1000
    TL0=0X18;
    i++;
    if (i==1000)//1s
    {
        i=0;
        Second++;
    }
}