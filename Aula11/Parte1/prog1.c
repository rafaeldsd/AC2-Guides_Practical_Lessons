#include <detpic32.h>

int main(void)
{
    // Configure UART2: 115200, N, 8, 1
    U2BRG = 10                  // ((20000000 + 8 * 115200) / (16 * 115200)) - 1;
    U2MODEbits.PDSEL = 0b00;    // 8 data bits and no parity
    U2MODEbits.STSEL = 0;       // 1 stop bit
    U2MODEbits.BRGH = 0;        // divide by 16, 0 -> 16, 1 -> 4
    U2STAbits.URXEN = 1;        // 1 – Enable UART2 receiver (enables reception)
    U2STAbits.UTXEN = 1;        // 1 – Enable UART2 transmitter (enables trasmission)
    U2MODEbits.ON = 1;          // 1 – Enable UART2 (enables UART2)

    // Configure UART2 interrupts, with RX interrupts enabled  and TX interrupts disabled: 
    // enable U2RXIE
    IEC1bits.U2RXIE = 1;        // enable interrupts
    // disable U2TXIE 
    IEC1bits.U2TXIE = 0;        // disable interrupts
    // set UART2 priority level (register IPC8)
    IPC8bits.U2IP = 1;          // priority 1
    // clear Interrupt Flag bit U2RXIF (register IFS1)
    IFS1bits.U2RXIF = 0;        // clear interrupt flag
    // define RX interrupt mode (URXISEL bits)
    U2STAbits.URXISEL = 00;     
    // Enable global Interrupts
    EnableInterrupts();

    while(1);
    return 0;
} 
