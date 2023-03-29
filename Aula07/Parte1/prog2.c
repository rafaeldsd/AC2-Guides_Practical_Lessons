#include <detpic32.h>

void delay (int ms);
void send2displays(unsigned char value);
int toBcd(unsigned char value);

volatile unsigned char voltage = 0; // Global variable

int main(void) {

    unsigned int cnt = 0;      // Counter variable

    // Configure displays
    TRISB = TRISB & 0x80FF;    // Configure RB8-RB14 as outputs
    TRISD = TRISD & 0xFF9F;    // Configure RD5-RD6 as outputs

    // Configure A/D module
    TRISBbits.TRISB4 = 1;       // RB4 digital output disconnected
    AD1PCFGbits.PCFG4= 0;       // RB4 configured as analog input
    AD1CON1bits.SSRC = 7;       // Conversion trigger selection bits: in this mode 
                                // an internal counter ends sampling and starts conversion
    AD1CON1bits.CLRASAM = 1;    // Stop conversions when the 1st A/D converter interrupt is generated. 
                                // At the same time, hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16;      // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 8-1;     // Interrupt is generated after N samples
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
    
    while(1){
        if(cnt == 0){ // 0, 200 ms, 400 ms, ... (5 samples/second)
            AD1CON1bits.ASAM = 1; // Start conversion
        }
        
        send2displays(voltage); // Send "voltage" value to displays
        cnt = (cnt + 1) % 20;  // Increment "cnt" variable (module 20)
        
        delay(10);  // Wait 10 ms
    }
    return 0;
}

void delay(int ms){
    for(;ms>0;ms--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

void send2displays(unsigned char value){
    static const char disp7Scodes[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };
    static char displayFlag = 0;    // static variable: doesn't loose its value between calls to function

    unsigned char digit_low = value & 0x0F;
    unsigned char digit_high = value >> 4;
    // if "displayFlag" is 0 then send digit_low to display_low
    if(displayFlag == 0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | (disp7Scodes[digit_low] << 8);
    }
    // else send digit_high to display_high
    else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | (disp7Scodes[digit_high] << 8);
    }
    // toggle "displayFlag" variable
    displayFlag = !displayFlag;
}

int toBcd(unsigned char value){
    return ((value / 10) << 4) + (value % 10);
}

// Interrupt Handler
void _int_(27) isr_adc(void){   // Replace VECTOR by the A/D vector number - see "PIC32 family data sheet" (pages 74-76)
    // Read 8 samples (ADC1BUF0, ..., ADC1BUF7) and calculate average
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
    