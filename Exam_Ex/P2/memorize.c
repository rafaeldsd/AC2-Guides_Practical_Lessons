#include <detpic32.h>

void delay (int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while (readCoreTimer()<20000);
    }
}

void send2displays(unsigned char value){
    int display7codes[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };
    static int displayFlag = 0;
    unsigned char dl = value & 0x0F;
    unsigned char dh = value >> 4;

    if (displayFlag == 0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | (display7codes[dl] << 8);
    }
    else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | (display7codes[dh] << 8);
    }
    displayFlag = !displayFlag;
}

unsigned char toBCD (unsigned char value){
    return ((value / 10) << 4) + (value % 10);
}

int voltageConversion(int average){
    return (((average * 33) + 511) / 1023);
}

void PWM(int dutycycle){
    if(dutycycle >= 0 && dutycycle <= 100){
        OCXRS = ((PRY + 1) * dutycycle)/100
    }
}

void _int_(27)isr_adc(void){
    // ...
    IFS1bits.AD1IF = 0;     // Reset AD1IF flag
}

void _int_(4)isr_T1(void){
    // ...
    IFS0bits.T1IF = 0; // Reset timer T1 interrupt flag 
}

void _int_(8)isr_T2(void){
    // ...
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag 
}

void _int_(12)isr_T3(void){
    // ...
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag 
}

void _int_(32) isr_uart2(void)
{
    // ...
    IFS1bits.U2RXIF = 0;
}