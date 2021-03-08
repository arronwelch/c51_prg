#include <REG52.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

#define GPIO_LED_DISPLAY P0
#define GPIO_MATRIX_KEY P1

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
u8 code NsegCode[17] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
                        //  'A'  'b'   'C'   'd'   'E'   'F' clear
                        0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x00};
u8 column;
u8 row;
u8 coordinates;

void delay(u16 t) //10us
{
    while (t--)
        ;
}

// P1^0 C0 1 0 1 1 1 0 0 0 0 0 0
// P1^1 C1 1 1 0 1 1 0 0 0 0 0 0
// P1^2 C2 1 1 1 0 1 0 0 0 0 0 0
// P1^4 C3 1 1 1 1 0 0 0 0 0 0 0
// P1^5 R0 0 0 0 0 0 1 1 0 1 1 1
// P1^6 R1 0 0 0 0 0 1 1 1 0 1 1
// P1^7 R2 0 0 0 0 0 1 1 1 1 0 1
// P1^8 R3 0 0 0 0 0 1 1 1 1 1 0

//the HC6800-EM3 V3.0 dontuse
//     C0 C1 C2 C3
// R0   0  1  2  3
// R1   4  5  6  7
// R2   8  9  A  b
// R3   C  d  E  F
//actual is this(hardware note BUG):
//     R0 R1 R2 R3
// C0   0  1  2  3
// C1   4  5  6  7
// C2   8  9  A  b
// C3   C  d  E  F

void MatrixKey()
{
    u8 loop_delay = 0;
    GPIO_MATRIX_KEY = 0X0F;
    if (GPIO_MATRIX_KEY != 0X0F)
    {
        delay(1000); //10ms
        if (GPIO_MATRIX_KEY != 0X0F)
        {
            switch (GPIO_MATRIX_KEY)
            {
            case (0x0E): //column = 0
                column = 0;
                break;
            case (0x0D): //column = 1
                column = 1;
                break;
            case (0x0B): //column = 2
                column = 2;
                break;
            case (0x07): //column = 3
                column = 3;
                break;
            default:
                break;
            }
            GPIO_MATRIX_KEY = 0XF0;
            switch (GPIO_MATRIX_KEY)
            {
            case (0xE0): //row = 0
                row = 0;
                break;
            case (0xD0): //row = 1
                row = 1;
                break;
            case (0xB0): //row = 2
                row = 2;
                break;
            case (0x70): //row = 3
                row = 3;
                break;
            default:
                break;
            }
            coordinates = column * 4 + row;
            while ((loop_delay < 50) && (GPIO_MATRIX_KEY != 0XF0))
            {
                loop_delay++;
                delay(1000); //delay 10ms
            }
        }
    }
}

void main()
{
    while (1)
    {
        MatrixKey();
        GPIO_LED_DISPLAY = ~NsegCode[coordinates];
    }
}