void delay(unsigned int ms){
    resetCoreTimer();
    // while(readCoreTimer() < K * ms); K=20*10**6*t, where t is the time in seconds
    while(readCoreTimer() < 20000000 * ms); // for 1 seconde, K=20*10**6 = 20000000
}