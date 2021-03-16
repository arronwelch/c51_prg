#include <REG52.H>

typedef unsigned char u8;
typedef unsigned int u16;

void USART_Init()
{
    TMOD |= 0x20;//Timer1 mode2

    //Timer1(TH1) in Mode2
    //Baudrate = Fosc / (N * (256 – TH1))
    //TH1 = 256 – (Fosc / (384 * Baudrate)) //If( SMOD==0 in PCON register)
    //TH1 = 256 – (Fosc / (192 * Baudrate)) //If( SMOD==1 in PCON register)
    
    TH1 = 0xF3;//baud rate = 4800 bps
    TL1 = 0xF3;//baud rate = 4800 bps

    PCON |= 0x80;//(Power Mode Control):SMOD=1,double baud rate using timer1
    TR1 = 1;//Timer 1 run control bit

    SCON = 0x50;//01,mode1(8-bit UART);SM2=0;REN(Receive enable)=1
    ES = 1;//Enable serial port interrupt
    EA = 1;//Enable Interrupts bit
}


void main()
{
    USART_Init();
    while (1);
}

void Usart() interrupt 4
{
    u8 ReceiveData;
    ReceiveData=SBUF;
    RI = 0;//clear Receive Interrupt flag
    SBUF = ReceiveData;
    while (!TI);
    TI = 0;//clear Transmit Interrupt flag
}