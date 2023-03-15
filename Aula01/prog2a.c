int main(void){
    char c;
    do{
        c = getChar();
        putChat(c);
    } while (c != '\n');
    return 0;
}