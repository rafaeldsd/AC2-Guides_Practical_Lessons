#include <detpic32.h>

volatile int counter = 0;

int main(void){
    // Configure Timer T3 (2 Hz with interrupts enabled)
    T3CONbits.TCKPS = 7; // 1:256 prescaler (i.e fin = 78,125 KHz)
    PR3 = 39062 ; // Fout = 20MHz / (256 * (39061 + 1)) = 1.99997...
    TMR3 = 0; // Clear timer T2 count register
    T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the timer configuration sequence)
    
    IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1; // Enable timer T3 interrupts
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag 

    EnableInterrupts();

    while(1);
    return 0;
}

void _int_(12) isr_T3(void){
    counter++;
    if(counter == 2){
        putChar('.');
        counter = 0;
    }
    IFS0bits.T3IF = 0;          // Reset timer T3 interrupt flag
}

/*

Prescaler => 1:256

Fout = 20MHz / (256 * (⌈x⌉ + 1)) = 2 Hz

X = ⌈39061,5⌉ = 39062


*/