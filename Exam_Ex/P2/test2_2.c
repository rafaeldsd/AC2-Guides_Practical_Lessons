#include <detpic32.h>

volatile int voltage = 0; // Global variable  

int main(void){
    // Configure I/O
    TRISB = TRISB & 0x80FF;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    // Configure ADC 
    TRISBbits.TRISB4 = 1; // RB4 digital output disconnected
    AD1PCFGbits.PCFG4= 0; // RB4 configured as analog input
    AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this mode an internal counter ends sampling and starts conversion
    AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter interrupt is generated. At the same time, hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 2-1; // Interrupt is generated after N samples
    AD1CHSbits.CH0SA = 4; // replace x by the desired input analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converter
    
    // Config Timer 2
    T2CONbits.TCKPS = 2; // 1:4 prescaler 
    PR2 = 41666; // Fout = 20MHz / (4 * (41666 + 1)) = 120 Hz
    TMR2 = 0; // Clear timer T2 count register
    T2CONbits.TON = 1; // Enable timer T2  

    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag

    int i;
    int value;
    while(1){
        delay(100); // 10 Hz => 100 ms
        AD1CON1bits.ASAM = 1; // Start conversion
        while( IFS1bits.AD1IF == 0 ); // Wait while conversion not done

        int *p = (int *)(&ADC1BUF0);
        for(i = 0; i < 2; i++){
            value += p[i*4];
        }
        value = value / 2;
        voltage = voltageConversion(value);
        IFS1bits.AD1IF = 0;
		IFS0bits.T2IF = 0;
    }
}

unsigned char toBcd(unsigned char value){
    return ((value / 10) << 4) + (value % 10); 
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

void delay(int ms){
    for(;ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

int voltageConversion(int value){
    int temp = (((3.3 * value) + 511)/1023);
    // 0 - 3.3 | 0 - 45 => 45/3.3
    temp = (temp*(45/3.3))+20;
    return temp;
}

void _int_(8) isr_adc(void) {
    send2displays(toBcd(voltage));
    IFS0bits.T2IF = 0;
}

