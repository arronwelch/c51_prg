#include <REG52.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

#define LED P0

void delay(u16 i)
{
	while (i--)
		;
}

void main()
{
	u8 i;
	LED = 0x01; //0000 0001
	delay(50000);
	while (1)
	{
		//for ( i = 0; i < 8; i++)
		//{
		//    LED=(0x01<<i);//0x01
		//    Delay500ms();
		//}
		for (i = 0; i < 7; i++)
		{
			LED = _crol_(LED, 1); //0000 0010
			delay(50000);		  //0000 0100
		}						  //1000 0000

		for (i = 0; i < 7; i++)
		{
			LED = _cror_(LED, 1); //0100 0000
			delay(50000);		  //0010 0000
		}						  //0000 0001
	}
}