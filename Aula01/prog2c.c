int main(void){
    int value;
    while(1){
        printStr("\nIntroduza um inteiro (sinal e módulo): ");
        value = readInt10();
        printStr("\nValor em base 10 (signed): ");
        printInt10(value);
        printStr("\nValor em base 2: ");
        printInt(value, 2);
        printStr("\nValor em base 16: ");
        printInt(value, 16);
        printStr("\nValor em base 10 (unsigned): ");
        printInt(value, 10);
        printStr("\nValor em base 10 (unsigned), formatado: ");
        printInt(value, 10 | 5 << 16); 
    }
    return 0;
}