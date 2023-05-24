#include <detpic32.h>

void delay (int ms){
    for(;ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer<20000);
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
        LATB = (LATB && 0x80FF) | display7codes[dl] << 8;
    }
    else {
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB && 0x80FF) | display7codes[dh] << 8;
    }
    displayFlag = !displayFlag;
}

unsigned char toBCD (unsigned char value){
    return ((value / 10) << 4) + (value % 10);
}

void voltageConversion(int average){
    return ((average*33 + 511)/ 1023);
}

