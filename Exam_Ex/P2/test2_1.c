#include <detpic32.h>

int main(void){
    // Config I/O
    TRISB = TRISB | 0x0009;

    T3CONbits.TCKPS = 1; // 1:2
    PR3 = 35714; // Fout = 20MHz / (2 * (35714 + 1)) = 280 Hz
    TMR3 = 0; // Reset timer T3 count register
    T3CONbits.TON = 1; // Enable timer T3

    OC2CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC2CONbits.OCTSEL = 1;// Use timer T2 as the time base for PWM generation
    OC2RS = 12500; // Ton constant
    OC2CONbits.ON = 1; // Enable OC2 module
    int value;

    while(1){
        // 250 us
        resetCoreTimer();
        while(readCoreTimer()< 5000); // 1 ms => 20000 | 1 us => 20 | 250 us => 20 * 250 = 5000
        value = PORTB & 0x0009;
        if (value == 0x1){
            setPWM(25);
        }
        if (value == 0x8){
            setPWM(70);
        }

    }

}

void setPWM(int dutycycle){
    if(dutycycle >= 0 && dutycycle <= 100){
        OC2RS = ((PR3 + 1) * dutycycle)/100;
    }
}
