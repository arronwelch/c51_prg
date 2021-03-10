#include <REG52.H>

typedef unsigned char u8;
typedef unsigned int u16;

//L6219
//Stepper motor driver

sbit PH1 = P1 ^ 0; //1,OUT1A(source) to OUT1B(sink)
                   //0,OUT1B(source) to OUT1A(sink)
sbit PH2 = P1 ^ 1; //1,OUT2A(source) to OUT2B(sink)
                   //0,OUT2B(source) to OUT2A(sink)

// I0X  I1X    Current level
//  1    1    No current(BUG!!!leakage!!!)
//  0    1    Low current 1/3 IO max
//  1    0    Medium current 2/3 IO max
//  0    0    Maximum current IO max

sbit I01 = P1 ^ 2; //contol winding 1
sbit I11 = P1 ^ 3; //contol winding 1

sbit I02 = P1 ^ 4; //contol winding 2
sbit I12 = P1 ^ 5; //contol winding 2

#define SPEED 20000

void delay(u16 t)//10uS
{
  u8 i=10;
  while (i--)
  {
    while (t--);
  }

}

void main()
{    
    P1=0XFF;//clear
    while (1)
    {
      //4 phase
      PH1 = 1;I01 = 0;I11 = 0;PH2 = 1;I02 = 0;I12 = 0;delay(SPEED);//A+B+
      PH1 = 0;I01 = 0;I11 = 0;PH2 = 1;I02 = 0;I12 = 0;delay(SPEED);//A-B+
      PH1 = 0;I01 = 0;I11 = 0;PH2 = 0;I02 = 0;I12 = 0;delay(SPEED);//A-B-
      PH1 = 1;I01 = 0;I11 = 0;PH2 = 0;I02 = 0;I12 = 0;delay(SPEED);//A+B-
    }
}