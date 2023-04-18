#include <detpic32.h>

void delay(int ms);

void main(void){
    // Configure RE0 as output
    TRISEbits.TRISE0 = 0;
    LATEbits.LATE0 = 0;

    // Configure RD8 as input
    TRISDbits.TRISD8 = 1;

    while(1){
        // Read RD8 and assign it to RE0
        if(PORTDbits.RD8 == 1){
            LATEbits.LATE0 = 1;
            delay(3000);
        }
        else{
            LATEbits.LATE0 = 0;
        }
    }
}