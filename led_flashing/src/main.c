#include <REG52.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit led = P0 ^ 0;

void delay()
{
	while (i--)
	{
		/* code */
	}
	
}

void Delay500ms() //@11.973MHz
{
    unsigned char i, j, k;

    i = 23;
    j = 192;
    k = 86;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

void Delay1000ms() //@11.973MHz
{
    unsigned char i, j, k;

    i = 46;
    j = 127;
    k = 177;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

void main()
{
    led = 1;
    Delay1000ms();
    led = 0;
    Delay1000ms();
}