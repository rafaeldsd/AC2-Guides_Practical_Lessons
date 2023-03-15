#include <detpic32.h>

void delay(int ms);

int main(void){
    unsigned char segment;
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
    TRISB = TRISB & 0x80FF;
    TRISD = TRISD & 0xFF9F;
    while(1){
        segment = 1;
        int i;
        for(i = 0; i < 7; i++){
            LATB = (LATB & 0x80FF) | (segment << 8);
            delay(500);
            segment = segment << 1;
        }
        LATDbits.LATD6 = !LATDbits.LATD6;
        LATDbits.LATD5 = !LATDbits.LATD5;
    }
    return 0;
}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}