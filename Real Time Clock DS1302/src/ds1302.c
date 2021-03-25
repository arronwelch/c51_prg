#include "reg52.h"
#include "intrins.h"//声明void _nop_(void);

void Ds1302Read(uchar addr)
{
    CE = 0;
    _nop_();

}