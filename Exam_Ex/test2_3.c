#include <detpic32.h>

static char DS4, DS3, DS2, DS1;

int main(void){
    // Config RB0-3 as input
    TRISB = TRISB | 0x000F;
    // Config RE4 as output
    TRISEbits.TRISE4 = 0;
    LATEbits.LATE4 = 1;

    // Config UART 9600 bps, odd parity, 8 data bits, 2 stop bits
    U2BRG = ((PBCLK + 8 * 9600) / (16 * 9600)) - 1;
	U2MODEbits.BRGH = 0;
	U2MODEbits.STSEL = 1;
	U2MODEbits.PDSEL = 0b10;
	U2STAbits.UTXEN = 1;
	U2STAbits.URXEN = 1;
	U2MODEbits.ON = 1;

    IPC8bits.U2IP = 1;
	IFS1bits.U2RXIF = 0;
	IEC1bits.U2RXIE = 1;

    EnableInterrupts();

    while(1){
        DS4 = (PORTB & 0x0008) >> 3;
		DS3 = (PORTB & 0x0004) >> 2;
		DS2 = (PORTB & 0x0002) >> 1;
		DS1 = PORTB & 0x0001;	
    }

}

void putc(char c)
{
	while(U2STAbits.UTXBF == 1);
	U2TXREG = c;
}

void puts(char *str)
{
	while(*str != '\0')
	{
		putc(*str);
		str++;
	}
}

void _int_(32) isr_uart(){
    if (IFS1bits.U2RXIF == 1)
	{
		while(U2STAbits.URXDA == 0);
		char val = U2RXREG;
		
		if (val == 'T')
			LATEbits.LATE4 = !LATEbits.LATE4;
			
		if (val == 'P')
		{
			puts("DipSwitch=");
			putc(DS4);
			putc(DS3);
			putc(DS2);
			putc(DS4);
			putc('\n');
		}
	}
		
	IFS1bits.U2RXIF = 0;
}