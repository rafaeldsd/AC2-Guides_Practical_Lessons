#include <detpic32.h>

void delay(int ms);

int main(void){
    // Configure ports RE06 to RE2 as outputs
    LATE = LATE & 0xFF87;
    TRISE = TRISE & 0xFF87;
    int counter = 0;

    while(1){
        LATE = (LATE & 0xFF87) | (counter << 3);
        // wait 250ms
        delay(250);
        counter++;
        // increment counter in module 10
        if (counter > 9){
            counter = 0;
        }
    }
    return 0;
}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}