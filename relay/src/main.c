#include <REG52.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit relay = P1 ^ 4;

void main()
{
    relay = 0;
    while (1)
    {
    }
}