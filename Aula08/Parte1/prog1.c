#include <detpic32.h>

int main(void){
    // Configure Timer T3 (2 Hz with interrupts disabled)
    T3CONbits.TCKPS = 7; // 1:256 prescaler (i.e fin = 78,125 KHz)
    PR3 = 39062 ; // Fout = 20MHz / (256 * (39061 + 1)) = 1.99997...
    TMR3 = 0; // Clear timer T3 count register
    T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the timer configuration sequence)
    while(1){
        while(IFS0bits.T3IF == 0);  // Wait while T3IF = 0
        IFS0bits.T3IF = 0;          // Reset timer T3 interrupt flag
        putChar('.');
    }
    return 0;
}

/*

Prescaler => 1:256

Fout = 20MHz / (256 * (⌈x⌉ + 1)) = 2 Hz

X = ⌈39061,5⌉ = 39062


*/