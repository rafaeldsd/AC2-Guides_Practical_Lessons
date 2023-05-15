#include <detpic32.h>

void configUART(unsigned int baud, char parity, unsigned int dataBits, unsigned int stopbits);
    // Configure UART2:
    // 1 - Configure BaudRate Generator
    if (baud >= 600 || baud <= 115200){
        U2BRG = ((PBCLK + 8 * baud) / (16 * baud)) - 1;
    }
    else{
        U2BRG = 10;

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