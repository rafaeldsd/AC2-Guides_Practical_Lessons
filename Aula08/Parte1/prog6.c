/*
    Timer 1:

    prescaler => 1:64

    Fout = 20MHz / (64 * (⌈x⌉ + 1)) = 5 Hz

    ⌈x⌉ = 62499
    ---------------------------------------------
    Timer 3:

    prescaler => 1:16

    Fout = 20MHz / (16 * (⌈x⌉ + 1)) = 25 Hz

    ⌈x⌉ = 49999

*/

#include <detpic32.h>

int main(void){

    //Configure ports RD0 and RD2 as outputs
    TRISE = TRISE & 0xFFF5;
    LATE = LATE & 0xFFF5;

    // Configure Timer T1 (5 Hz with interrupts enabled)
    T1CONbits.TCKPS = 2;        // 1:64 prescaler (i.e fin = 312,5 KHz)
    PR1 = 62499 ;               // Fout = 20MHz / (64 * (62499 + 1)) = 5 Hz
    TMR1 = 0;                   // Clear timer T2 count register
    T1CONbits.TON = 1;          // Enable timer T1 (must be the last command of the timer configuration sequence)
    
    // Configure Timer T3 (25 Hz with interrupts enabled)
    T3CONbits.TCKPS = 4;        // 1:16 prescaler (i.e fin = 1,25 MHz)
    PR3 = 49999 ;               // Fout = 20MHz / (16 * (49999 + 1)) = 25 Hz
    TMR3 = 0;                   // Clear timer T2 count register
    T3CONbits.TON = 1;          // Enable timer T3 (must be the last command of the timer configuration sequence)

    IPC1bits.T1IP = 2;          // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1;          // Enable timer T1 interrupts
    IFS0bits.T1IF = 0;          // Reset timer T1 interrupt flag

    IPC3bits.T3IP = 2;          // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1;          // Enable timer T3 interrupts
    IFS0bits.T3IF = 0;          // Reset timer T3 interrupt flag
 
    EnableInterrupts();

    while(1);
    return 0;
}

void _int_(4) isr_T1(void)
{
    LATEbits.LATE1 = !LATEbits.LATE1;
    IFS0bits.T1IF = 0;          // Reset timer T1 interrupt flag
}

void _int_(12) isr_T3(void)
{
    LATEbits.LATE3 = !LATEbits.LATE3;
    IFS0bits.T3IF = 0;          // Reset timer T3 interrupt flag
}