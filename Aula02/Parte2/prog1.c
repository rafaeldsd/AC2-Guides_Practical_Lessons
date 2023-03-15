int main(void){
    int cnt1 = 0, cnt5 = 0, cnt10 = 0; // cnt1 variable increment every 1Hz, cnt5 every 5Hz, cnt10 every 10Hz
    while(1){
        delay(100); // 10Hz = 0.1s

        putChar('\r'); 
        printInt(cnt10++, 10 | 4 << 16); // print cnt1 in decimal with 4 digits
        putChar('\t');
        if (cnt10 % 5 == 0) // if cnt1 is multiple of 5
        {
            printInt(cnt5++, 10 | 4 << 16); // print cnt5 in decimal with 4 digits
            putChar('\t');
        }
        if (cnt10 % 10 == 0) // if cnt1 is multiple of 10
        {
            printInt(cnt10++, 10 | 4 << 16); // print cnt10 in decimal with 4 digits
            putChar('\t');
        }
        
    }
    return 0;
}

void delay(int ms){
    for (; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000000);
    }
}