#include <detpic32.h>

int main(void){

    // Config I/O
    TRISB = TRISB | 0x00F;
    TRISEbits.TRISE4 = 0;
    LATEbits.TRISE4 = 1;
    
    // Config UART
    U2BRG = ((PBCLK + 8 * 9600)/(16*9600));
    U2MODEbits.BRGH = 0; 
    U2MODEbits.PDSEL = 0b10;
    U2MODEbits.STSEL = 1;
    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;
    U2STAbits.UART = 1;

    // Config Interrupts
    IPC8bits.U2IP = 1;
    IFS1bits.U2RXIF = 0;
	IEC1bits.U2RXIE = 1;
	U2STAbits.URXISEL = 00;

    EnableInterrupts();

    while(1);

    return 0;
}

void putc(char byte2send){
    while (U2STAbits.UTXBF == 1);
    U2TXREG = byte2send;
} 

void putStr(char *str){
    while(*str != '\0'){
        putc(*str);
        str++;
    }
}

void _int_(32) isr_uart2(){
    if(IFS1bits.U2RXIF = 1){
        while (U2STAbits.URXDA == 0);
        char val = U2RXREG;

        if (val == 'T'){
            LATEbits.TRISE4 = !LATEbits.TRISE4;
        }
        if(val == 'P'){
            putStr("Dip-Switch=");
			putc((PORTB & 0x000F) + '0');
			putc('\n');
        }
    }
    IFS1bits.U2RXIF = 0;
}