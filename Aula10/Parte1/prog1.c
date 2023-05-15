#include <detpic32.h>

int main(void)
{
    // Configure UART2:
    // 1 - Configure BaudRate Generator
    U2BRG = 10                  // ((20000000 + 8 * 115200) / (16 * 115200)) - 1;
    // 2 – Configure number of data bits, parity and number of stop bits
    // (see U2MODE register)
    U2MODEbits.PDSEL = 0b00;   // 8 data bits and no parity
    U2MODEbits.STSEL = 0;      // 1 stop bit
    U2MODEbits.BRGH = 0;       // divide by 16, 0 -> 16, 1 -> 4
    // 3 – Enable the trasmitter and receiver modules (see register U2STA)
    U2STAbits.URXEN = 1;       // 1 – Enable UART2 receiver (enables reception)
    U2STAbits.UTXEN = 1;       // 1 – Enable UART2 transmitter (enables trasmission)
    // 4 – Enable UART2 (see register U2MODE)
    U2MODEbits.ON = 1;         // 1 – Enable UART2 (enables UART2)

    return 0;
} 