int main(void){
    int counter = 0;
    while(1){
        resetCoreTimer();
        while(readCoreTimer() < 200000);
        printInt(counter++, 10 | 4 << 16); // Ver nota1
        putChar('\r'); // cursor regressa ao inicio da linha
    }
    return 0;
}

// clock frequency = 20 MHz (20,000,000 Hz)
// The counter is incremented every 0.1s because 200,000/ 20,000,000 = 0.1s