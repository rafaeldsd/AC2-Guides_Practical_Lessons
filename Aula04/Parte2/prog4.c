#include <detpic32.h>

void delay(int ms);

int main(void){
    unsigned char segment;
    LATD = (LATD & 0xFF9F) | 0x0040;
    TRISB = TRISB & 0x80FF;
    TRISD = TRISD & 0xFF9F;
    while(1){
        segment = 1;
        int i;
        for(i = 0; i < 7; i++){
            LATB = (LATB & 0x80FF) | (segment << 8);
            delay(100); // 10 Hz
            // delay(20); // 50 Hz
            //delay(10); // 100 Hz
            segment = segment << 1;
        }
        LATD = LATD ^ 0x0060;
    }
    return 0;
}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}