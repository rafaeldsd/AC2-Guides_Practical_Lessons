#include <detpic32.h>

static volatile int counter = 0;

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

int main(void){
    // Configure ports, Timer T2, interrupts and external interrupt INT1
    TRISEbits.TRISE0 = 0;
    LATEbits.LATE0 = 0;
    TRISDbits.TRISD8 = 1;

    T2CONbits.TCKPS = 7; // 1:256 prescaler (i.e fin = 78,125 KHz)
    PR2 = 39062 ; // Fout = 20MHz / (256 * (39061 + 1)) = 1.99997...
    
    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag

    IPC1bits.INT1IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.INT1IE = 1; // Enable INT1 interrupts
    IFS0bits.INT1IF = 0; // Reset INT1 interrupt flag
    INTCONbits.INT1EP = 1; // INT1 falling edge event

    EnableInterrupts();
    while(1);
    return 0;
}

void _int_(8) isr_T2(void){
    // Timer T2 => 0.5 s
    if (counter == 6){
        LATEbits.LATE0 = 0;
        counter = 0;
        T2CONbits.TON = 0;
    }
    else{
        counter++;
    }
    IFS0bits.T2IF = 0;          // Reset timer T2 interrupt flag
}
void _int_(7) isr_INT1(void){
    LATEbits.LATE0 = 1;
    T2CONbits.TON = 1;	
	TMR2 = 0;
	IFS0bits.INT1IF = 0;
}
