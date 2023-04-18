#include <detpic32.h>


// Functions

void delay(int ms) {
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void send2displays (unsigned char value){
    static const char disp7Scodes[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };
    static char displayFlag = 0;
    unsigned char dl = value & 0x0F;
    unsigned char dh = value >> 4;

    if (displayFlag=0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB= (LATB & 0x80FF) | (disp7Scodes[dl] << 8);
    }
    else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB= (LATB & 0x80FF) | (disp7Scodes[dh] << 8);
    }
    displayFlag = !displayFlag;
}

unsigned char toBCD (unsigned char value){
    return ((value / 10) << 4)+(value % 10);
}

// ADC Config

void configure_ADC (void){
    RISBbits.TRISB4 = 1;       // RB4 digital output disconnected
    AD1PCFGbits.PCFG4= 0;       // RB4 configured as analog input
    AD1CON1bits.SSRC = 7;       // Conversion trigger selection bits: in this mode 
                                // an internal counter ends sampling and starts conversion
    AD1CON1bits.CLRASAM = 1;    // Stop conversions when the 1st A/D converter interrupt is generated. 
                                // At the same time, hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16;      // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = X-1;     // Interrupt is generated after N samples
                                // (replace N by the desired number of consecutive samples)
    AD1CHSbits.CH0SA = 4;       // replace x(4) by the desired input
                                // analog channel (0 to 15)
    AD1CON1bits.ON = 1;         // Enable A/D converter
                                // This must the last command of the A/D configuration sequence
}

// Interrupt Handler

void _int_(27) isr_adc(void){   // Replace VECTOR by the A/D vector number - see "PIC32 family data sheet" (pages 74-76)
    // Read 8 samples
    int *p = (int *)(&ADC1BUF0);
    int i;
    int average = 0;
    for(i = 0; i < 8; i++){
        average += p[i*4];
    }
    average = average / 8;
    voltage = (average * 33 + 511) / 1023;    // Calculate voltage amplitude
    // Convert voltage amplitude to decimal and store the result in the global variable "voltage"
    int value = toBcd(voltage);
    voltage = value;

    // Reset AD1IF flag
    IFS1bits.AD1IF = 0;
}

/*
delay:
for:        ble     $a0,$0,endfor
            li      $v0,resetCoreTimer
            syscall

while:      li      $v0,readCoreTimer
            syscall
            blt     $v0,20000,while
            
            addi    $a0,$a0,-1
            j       for
endfor:
            jr      $ra
            
*/