#include <detpic32.h>

void setPWM(unsigned int dutyCycle){
    if(dutyCycle >= 0 && dutyCycle <= 100){
        OC1RS = ((PRX + 1)* dutyCycle)/100; // Determine OC1RS as a function of "dutyCycle"
    }
} 
