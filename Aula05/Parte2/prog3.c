#include <detpic32.h>


void delay(int ms);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);

int main(void){
    // Configure RB8-RB14 as outputs
    TRISB = TRISB & 0x80FF;
    // Configure RD5-RD6 as outputs
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    // Configure RE0-RE8 as outputs
    TRISE = TRISE & 0xFF00;
    LATE = LATE & 0xFF00;

    // Configure RB0 as input
    TRISBbits.TRISB0 = 1;

    int counter = 0;
    while(1){
        int i = 0;
        if (PORTBbits.RB0 == 0){
            do{
                send2displays(counter);
                LATE = (LATE & 0xFF00) | toBcd(counter);
                delay(10); // wait 10ms => 100Hz
            }while(++i < 50); // repeated 50 times => 2Hz
        }
        if (PORTBbits.RB0 == 1)
        {
            do{
                send2displays(counter);
                LATE = (LATE & 0xFF00) | toBcd(counter);
                delay(10); // wait 10ms => 100Hz
            }while(++i < 20); // repeated 20 times => 5Hz 
        }
        
        
        if (PORTBbits.RB0 == 0 && counter > 0){
            counter--;
        }
        else if (PORTBbits.RB0 == 0 && counter == 0)
        {
            counter = 59;
        }
        
        else{
            counter++;
        }

        if (counter == 60){
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

void send2displays(unsigned char value){
    static const char disp7Scodes[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };
    static char displayFlag = 0;    // static variable: doesn't loose its value between calls to function

    unsigned char digit_low = value & 0x0F;
    unsigned char digit_high = value >> 4;
    // if "displayFlag" is 0 then send digit_low to display_low
    if(displayFlag == 0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | (disp7Scodes[digit_low] << 8);
    }
    // else send digit_high to display_high
    else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | (disp7Scodes[digit_high] << 8);
    }
    // toggle "displayFlag" variable
    displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value){
    return ((value / 10) << 4) + (value % 10);
}