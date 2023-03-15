#include <detpic32.h>

void delay(int ms);
void send2displays(unsigned char value);

int main(void){
    // configure RB8-RB14 as outputs
    TRISB = TRISB & 0x80FF;
    // configure RD5-RD6 as outputs
    TRISD = TRISD & 0xFF9F;

    while (1){
        send2displays(0x15);
        delay(200); // wait 0.2s
    }
    
}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void send2displays(unsigned char value){
    static const char disp7Scodes[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };

    // select display high
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;

    // send digit_high (dh) to display : dh = value >> 4
    unsigned char dh = value >> 4;
    LATB = (LATB & 0x80FF) | (disp7Scodes[dh] << 8);

    // select display low
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;

    // send digit_low (dl) to display : dl = value & 0x0F
    unsigned char dl = value & 0x0F;
    LATB = (LATB & 0x80FF) | (disp7Scodes[dl] << 8);
}