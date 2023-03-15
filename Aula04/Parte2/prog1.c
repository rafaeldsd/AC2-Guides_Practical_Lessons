#include <detpic32.h>

void delay(int ms);

int main(void) {
    // Configure port RB8-RB14, RD5-RD6 as output
    TRISB = TRISB & 0x80FF;
    TRISD = TRISD & 0xFF9F;
    LATB = LATB & 0x80FF;
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
    
    while(1){
        char c = getChar();
        // Convert to upper case
        if(c >= 'a' && c <= 'g'){
            c -= 0x20;
        }
        // Check if character is in the range A-G
        if(c>= 'A' && c <= 'G'){
            c -= 'A'; 
            int value = (0x100 << c);
            LATB = (LATB & 0x80FF) | value;  
        }
        else {
            LATB = LATB & 0x80FF;
        }
    }
    return 0;
}