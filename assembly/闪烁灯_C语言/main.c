#include "reg51.h"

sbit P0_0 = P0^0;

void delay(void)
{
	unsigned char x,y,z;
	for(x=0;x<250;x++)
	{
		for(y=0;y<250;y++)
		{
			for(z=0;z<10;z++);
		}
	}
}


void main(void)
{
	while(1)
	{
		P0_0 = 1;
		delay();
		P0_0 = 0;
		delay();
	}

}