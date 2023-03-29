#include <detpic32.h>

int main(void) {
    // Configure RD11 as output
    TRISDbits.TRISD11 = 0;
    LATDbits.LATD11 = 1;    // Set RD11=1

    // Configure A/D module
    TRISBbits.TRISB4 = 1;       // RB4 digital output disconnected
    AD1PCFGbits.PCFG4= 0;       // RB4 configured as analog input
    AD1CON1bits.SSRC = 7;       // Conversion trigger selection bits: in this mode 
                                // an internal counter ends sampling and starts conversion
    AD1CON1bits.CLRASAM = 1;    // Stop conversions when the 1st A/D converter interrupt is generated. 
                                // At the same time, hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16;      // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 1-1;     // Interrupt is generated after N samples
                                // (replace N by the desired number of consecutive samples)
    AD1CHSbits.CH0SA = 4;       // replace x(4) by the desired input
                                // analog channel (0 to 15)
    AD1CON1bits.ON = 1;         // Enable A/D converter
                                // This must the last command of the A/D configuration sequence
    
    // Configure interrupt system
    IPC6bits.AD1IP = 2; // configure priority of A/D interrupts to 2
    IEC1bits.AD1IE = 1; // enable A/D interrupts
    IFS1bits.AD1IF = 0; // reset A/D interrupt flag
    EnableInterrupts(); // Global Interrupt Enable
    
    AD1CON1bits.ASAM = 1; // Start conversion
    while(1){
        LATDbits.LATD11 = 0;    // Reset RD11 (LATD11 = 0)
    }
    return 0;
}

// Interrupt Handler
void _int_(27) isr_adc(void){   // Replace VECTOR by the A/D vector number - see "PIC32 family data sheet" (pages 74-76)
    volatile int adc_value;
    adc_value = ADC1BUF0;   // Read ADC1BUF0 value to "adc_value"
    AD1CON1bits.ASAM = 1;   // Start conversion
    IFS1bits.AD1IF = 0;     // Reset AD1IF flag
    LATDbits.LATD11 = 1;    // Set RD11 (LATD11 = 1)
}
    