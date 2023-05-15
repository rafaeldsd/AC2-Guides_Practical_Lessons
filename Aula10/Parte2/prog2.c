#include <detpic32.h>

int main(void){
    // Configure UART1 (115200, N, 8, 1)
    configUART1(115200, 'N', 8, 1);

    while(1)
    {
        // Send character using putc()
        putc1("0x5A");
        delay(10); // wait 10 ms
    }
    return 0;
}

void putc1(char byte){
    // wait while UART1 UTXBF == 1
    while(U1STAbits.UTXBF == 1);
    // Copy "byte" to the U1TXREG register
    U1TXREG = byte;
}

void putstr(char *str){
    // use putc() function to send each charater ('\0' should not be sent)
    while(*str != '\0'){
        putc(*str);
        str++;
    }
}

void configUART1(unsigned int baud, char parity, unsigned int dataBits, unsigned int stopbits){
    // Configure UART1:
    // 1 - Configure BaudRate Generator
    if (baud >= 600 || baud <= 115200){
        U1BRG = ((PBCLK + 8 * baud) / (16 * baud)) - 1;
    }
    else{
        U1BRG = 10;
    }
    // 2 – Configure number of data bits, parity and number of stop bits
    if(parity == 'N'){
        U1MODEbits.PDSEL = 0b00;   // 8 data bits and no parity
    }
    else if(parity == 'E'){
        U1MODEbits.PDSEL = 0b01;   // 8 data bits and even parity
    }
    else if(parity == 'O'){
        U1MODEbits.PDSEL = 0b10;   // 8 data bits and odd parity
    }
    else{
        U1MODEbits.PDSEL = 0b00;   // 8 data bits and no parity
    }

    if(stopbits == 1 || stopbits == 2){
        U1MODEbits.STSEL = stopbits - 1;      // stop bits
    }
    else{
        U1MODEbits.STSEL = 0;      // 1 stop bit
    }
    U1MODEbits.BRGH = 0;       // divide by 16, 0 -> 16, 1 -> 4
    // 3 – Enable the trasmitter and receiver modules (see register U1STA)
    U1STAbits.URXEN = 1;       // 1 – Enable UART1 receiver (enables reception)
    U1STAbits.UTXEN = 1;       // 1 – Enable UART1 transmitter (enables trasmission)
    // 4 – Enable UART2 (see register U1MODE)
    U1MODEbits.ON = 1;         // 1 – Enable UART1 (enables UART2)

}

void delay(int ms){
    for(;ms>0,ms--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}