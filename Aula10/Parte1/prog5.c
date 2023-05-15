#include <detpic32.h>

int main(void){
    // Configure UART2 (115200, N, 8, 1)
    configUART(115200, 'N', 8, 1);

    while(1)
    {
        // Read character using getc()
        char c = getc();
        // Send character using putc()
        putc(c);
        delay(1000); // wait 1 s
    }
    return 0;
}
char getc(void){
    // Wait while URXDA == 0
    while(U2STAbits.URXDA == 0);
    // Return U2RXREG
    return U2RXREG;
} 

void putc(char byte){
    // wait while UART2 UTXBF == 1
    while(U2STAbits.UTXBF == 1);
    // Copy "byte" to the U2TXREG register
    U2TXREG = byte;
}

void putstr(char *str){
    // use putc() function to send each charater ('\0' should not be sent)
    while(*str != '\0'){
        putc(*str);
        str++;
    }
}

void configUART(unsigned int baud, char parity, unsigned int dataBits, unsigned int stopbits){
    // Configure UART2:
    // 1 - Configure BaudRate Generator
    if (baud >= 600 || baud <= 115200){
        U2BRG = ((PBCLK + 8 * baud) / (16 * baud)) - 1;
    }
    else{
        U2BRG = 10;
    }
    // 2 – Configure number of data bits, parity and number of stop bits
    if(parity == 'N'){
        U2MODEbits.PDSEL = 0b00;   // 8 data bits and no parity
    }
    else if(parity == 'E'){
        U2MODEbits.PDSEL = 0b01;   // 8 data bits and even parity
    }
    else if(parity == 'O'){
        U2MODEbits.PDSEL = 0b10;   // 8 data bits and odd parity
    }
    else{
        U2MODEbits.PDSEL = 0b00;   // 8 data bits and no parity
    }

    if(stopbits == 1 || stopbits == 2){
        U2MODEbits.STSEL = stopbits - 1;      // stop bits
    }
    else{
        U2MODEbits.STSEL = 0;      // 1 stop bit
    }
    U2MODEbits.BRGH = 0;       // divide by 16, 0 -> 16, 1 -> 4
    // 3 – Enable the trasmitter and receiver modules (see register U2STA)
    U2STAbits.URXEN = 1;       // 1 – Enable UART2 receiver (enables reception)
    U2STAbits.UTXEN = 1;       // 1 – Enable UART2 transmitter (enables trasmission)
    // 4 – Enable UART2 (see register U2MODE)
    U2MODEbits.ON = 1;         // 1 – Enable UART2 (enables UART2)

}

void delay(int ms){
    for(;ms>0,ms--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}