#include <REG52.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

//Common GND
// ---a---
// |     |
// f     b
// |--g--|
// e     c
// |     |
// ---d---  *dp
// dp g f e d c b a
// 0  0 1 1 1 1 1 1
//  '0'  '1'   '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'
u8 code NsegCode[16] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
                        //  'A'  'b'   'C'   'd'   'E'   'F'
                        0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

void main()
{
    P0 = ~NsegCode[2]; //Common positive
    while (1)
    {
    }
}