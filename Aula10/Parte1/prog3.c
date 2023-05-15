#include <detpic32.h>

int main(void){
    // Configure UART2 (115200, N, 8, 1)
    U2BRG = 10
    U2MODEbits.PDSEL = 0b00;   
    U2MODEbits.STSEL = 0;      
    U2MODEbits.BRGH = 0;
    U2STAbits.URXEN = 1;       
    U2STAbits.UTXEN = 1;
    U2MODEbits.ON = 1;
    while(1)
    {
        putstr("String de teste\n");
        delay(1000); // wait 1 s
    }
    return 0;
}

void putc(char byte){
    // wait while UART2 UTXBF == 1
    while(U2STAbits.UTXBF == 1);
    // Copy "byte" to the U2TXREG register
    U2TXREG = byte;
}

void putstr(char *str){
    // use putc() function to send each charater ('\0' should not be sent)
    while(*str != '\0'){
        putc(*str);
        str++;
    }
} 

void delay(int ms){
    for(;ms>0,ms--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}