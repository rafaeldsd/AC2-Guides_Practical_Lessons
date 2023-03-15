#include <detpic32.h>

void delay(int ms);

int main(void){
    static const char disp7Scodes[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };
    
    // Configure RB0 to RB3 as inputs
    TRISB = TRISB | 0x000F;
    // Configure RB8 to RB14 and RD5 to RD6 as outputs
    TRISB = TRISB & 0x80FF;
    TRISD = TRISD & 0xFF9F;
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;

    while(1){
        // Read RB0 to RB3
        int value = PORTB & 0x000F;
        // Convert to 7 segments code
        int code = disp7Scodes[value];
        // Send to display
        LATB = (LATB & 0x80FF) | (code << 8);
    }
    return 0;
}