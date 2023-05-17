#include <detpic32.h>

void _int_(VECTOR_UART2) isr_uart2(void)
{
    if (IFS1bits.U2RXIF == 1)
    {
        // Read character from FIFO (U2RXREG)
        char c = U2RXREG;
        // Send the character using putc()
        putc(c);
        // Clear UART2 Rx interrupt flag
        IFS1bits.U2RXIF = 0;
    }
} 
void putc(char byte){
    // wait while UART2 UTXBF == 1
    while(U2STAbits.UTXBF == 1);
    // Copy "byte" to the U2TXREG register
    U2TXREG = byte;
}