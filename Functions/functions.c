#include <detpic32.h>

void delay (int ms){
    for(;ms>0;ms--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
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

// ADC

while (1){
    AD1CON1bits.ASAM = 1; // Start conversion
    while( IFS1bits.AD1IF == 0 ); // Wait while conversion not done (AD1IF == 0)
    int *p = (int *)(&ADC1BUF0);
    int i, V, sum = 0;
    for( i = 0; i < 16; i++ ) {
        sum += p[i*4];
    }
    sum = sum / 4;
    V = (sum * 33 + 511) / 1023;
    printInt( V, 16 | 3 << 16); // Read conversion result (ADC1BUF0 value) and print it
    putChar('\n');
    delay(250); // 250 ms
    IFS1bits.AD1IF = 0; // Reset AD1IF
}