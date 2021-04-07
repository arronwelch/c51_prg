#include <reg52.h>
#include <intrins.h>

typedef unsigned char u8;
typedef unsigned int u16;

//dynamic 8-seg LED display BIT select
sbit LSA = P2 ^ 2;
sbit LSB = P2 ^ 3;
sbit LSC = P2 ^ 4;

#define GPIO_SEGCODE P0 //dynamic 8-seg LED display DECODE select GPIO

u8 code NsegCode[17] = 
// '0'  '1'   '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'
 {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
// 'A'  'b'   'C'   'd'   'E'   'F'   'H'
  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71,0x76};

u8 SegDisplayData[8]={0};

sbit IR = P3 ^ 2;
sbit FP = P1^0;

u8 Time;
u8 IrValue[4];
u8 FLAG=0;

void delay(u16 i)//i=1 about 10us
{
    while (i--)
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

void IrInit()
{
    IT0 = 1; //Set Transition from high to low level to triger Interrupt
    EX0 = 1; //Enable INT0 Interrupt
    EA = 1;  //Enable Total Interrupt

    IR = 1;//init
}

void main()
{
    IrInit();
    while (1)
    {
        if (1 == FLAG)
        {
            SegDisplayData[0] = NsegCode[IrValue[2]/16];
            SegDisplayData[1] = NsegCode[IrValue[2]%16];
            SegDisplayData[2] = NsegCode[16];//'H'
            DigDisplay();
        }
    }
}

void ReadIr(void) interrupt 0
{
    u8 j,k;
    u16 err;
    FLAG=0;
    Time = 0;

    FP = 0;
    //delay(1);//4.417us
    delay(8);//16.42us
    //delay(80);//109.7us
    FP = 1;

    if(0 == IR)
    {
        err = 900;
        while ((0==IR)&&(err>0))//9ms Start Low
        {
            delay(10);//10us
            err--;
        }
        FP = 0;
        if(1 == IR)
        {
            err = 500;
            while ((1==IR)&&(err>0))//4.5ms Start High
            {
                delay(10);//10us
                err--;
            }
            FP = 1;
            for(k=0;k<4;k++)//4 group
            {
                IrValue[k]=0;//clear
                for(j=0;j<8;j++)//8 bit
                {
                    err = 50;
                    while ((0 == IR)&&(err>0))//510us
                    {
                        delay(10);
                        err--;
                    }
                    FP = 0;
                    err = 500;
                    while ((1 == IR)&&(err>0))//calculate High Level Time
                    {
                        delay(80);//0.1ms
                        Time++;
                        err--;
                        if(Time>30)//3ms
                        {
                            FLAG=0;
                            FP = 1;
                            return;//error
                        }
                    }
                    FP = 1;
                    IrValue[k]>>=1;
                    if (Time>=8)//bigger than 0.8ms(0.565 '0',1.69 '1')
                    {
                        IrValue[k]|=0x80;//LSB
                    }
                    Time = 0;//clear
                }
            }            
        }        
    }
    if(IrValue[2]!=~IrValue[3])
    {
        FLAG=0;
        FP = 1;
        return;//error
    }
    FLAG=1;
    FP = 1;
}