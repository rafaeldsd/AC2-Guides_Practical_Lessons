#include <detpic32.h>

int main(void){
    // Configure I/O
    TRISCbits.TRISC14 = 0;
    TRISDbits.TRISD0 = 1;

    // Configure Timer3
    T3CONbits.TCKPS = 2 ; // 1:4 prescaler 
    PR3 = 49999; // Fout = 20MHz / (4 * (49999 + 1)) = 100 Hz
    TMR3 = 0; // Clear timer T3 count register
    T3CONbits.TON = 1; // Enable timer T3

    OC1CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC1CONbits.OCTSEL = 1;// Use timer T3 as the time base for PWM generation
    setPWM(10);
    // setPWM(65);
    // set PWM(80);
    OC1CONbits.ON = 1; // Enable OC1 module

    IPC3bits.T3IP = 2;          // Interrupt priority 2
    IEC0bits.T3IE = 1;          // Enable timer T3 interrupts
    IFS0bits.T3IF = 0;          // Reset timer T3 interrupt flag

    EnableInterrupts();

    while (1){
        // Read the value of port RD0 and write it on port RC14
        LATCbits.LATC14 = PORTDbits.RD0;
    }

    return 0;
}

void _int_(12) isr_T3(void){
    IFS0bits.T3IF = 0;          // Reset timer T3 interrupt flag
}

void setPWM(unsigned int dutyCycle){
    if(dutyCycle >= 0 && dutyCycle <= 100){
        OC1RS = ((PRX + 1)* dutyCycle)/100; // Determine OC1RS as a function of "dutyCycle"
    }
} 