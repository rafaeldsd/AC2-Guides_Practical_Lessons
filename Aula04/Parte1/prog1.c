#include <detpic32.h>

void delay(int ms);

int main(void) {
    // Configure port RC14 as output
    TRISCbits.TRISC14 = 0;
    LATCbits.LATC14 = 0;
    
    while(1){
        // wait 0.5s
        delay(500);
        // toggle RB14 port value
        LATCbits.LATC14 = !LATCbits.LATC14;
    }
    return 0;
}

void delay(int ms) {
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}