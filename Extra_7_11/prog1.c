#include <detpic32.h>

volatile int value;

int main(void){
    // Config I/O
    TRISE = TRISE & 0xFFF0;
    TRISB = TRISB & 0x80FF;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    // Config Timer2
    T2CONbits.TCKPS = 5; // 1:4 prescaler | 20MHz / (X * (65536)) = 100 Hz | X = 4
    PR2 = 49999; // Fout = 20MHz / (4 * (X + 1)) = 100 Hz | X = 49999
    TMR2 = 0; // Clear timer T2 count register
    T2CONbits.TON = 1; // Enable timer T2 

    // Config Interrupts
    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag 

    EnableInterrupts(); 

    while(1){
        char c;
        c = getChar();
        if (c == '0'){
            LATE = (LATE & 0xFFF0) | 0x0001;
            value = 0;
        }
        else if (c == '1'){
            LATE = (LATE & 0xFFF0) | 0x0002;
            value = 1;
        }
        else if (c == '2'){
            LATE = (LATE & 0xFFF0) | 0x0004;
            value = 3;
        }
        else if (c == '3'){
            LATE = (LATE & 0xFFF0) | 0x0008;
            value = 4;
        }
        else{
            LATE = (LATE & 0xFFF0) | 0x000F;
            value = 0xFF;
            delay(1000);
            LATE = (LATE & 0xFFF0) | 0x0000;
            LATB = LATB & 0x80FF;
        }
    }
    return 0;
}

void send2displays(unsigned char value){
    int display7codes[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };
    static int displayFlag = 0;
    unsigned char dl = value & 0x0F;
    unsigned char dh = value >> 4;

    if (displayFlag == 0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | display7codes[dl] << 8;
    }
    else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | display7codes[dh] << 8;
    }
    displayFlag = !displayFlag;
}

void _int_(8) isr_T2(void){
    send2displays(value);
    IFS0bits.T2IF = 0;
}