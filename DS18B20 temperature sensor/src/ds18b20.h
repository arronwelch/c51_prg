#ifndef _DS18B20_H_  
#define _DS18B20_H_  

typedef unsigned char uchar;
typedef unsigned int uint;

void delay_us(uchar n);
bit DS18B20_init();
void write_byte(uchar dat);
uchar read_byte();
uint read_temper ();
uint temper_change();
  
#endif  