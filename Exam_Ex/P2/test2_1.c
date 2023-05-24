#include <detpic32.h>

int main(void){
    // Config RB0 and RB3
    TRISB = TRISB | 0x0009;

    // Config Timer 3 (280 Hz)
    T3CONbits.TCKPS = 1; // 1:2 prescaler 
    PR3 = 35714; // Fout = 20MHz / (2 * (35714 + 1)) = 280 Hz
    TMR3 = 0; // Clear timer T2 count register
    T3CONbits.TON = 1; // Enable timer T2 (must be the last command of the timer configuration sequence)

    OC2CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC2CONbits.OCTSEL = 1;// Use timer T3 as the time base for PWM generation
    OC2CONbits.ON = 1; // Enable OC1 module

    while (1){
        // 250 us
        resetCoreTimer();
        while(readCoreTimer<5000);


        portVal = PORTB & 0x0009;
        switch (portValue){
            case 0x1: 
            setPWM(25);
            break;
            case 0x8:
            setPWM(75);
            break;
        }

    }
    return 0;
}


void setPWM(int dutycycle){
    if (dutycycle >= 0 && dutycycle <= 100)
        OC2RS= ((PR3+1)*dutycycle)/100;
}