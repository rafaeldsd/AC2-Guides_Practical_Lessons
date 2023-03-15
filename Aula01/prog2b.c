int main(void){
    char c;
    do {
        c = inkey();
        if( c != 0 )
        putChar( c );
        else
        putChar('.');
    } while( c != '\n' );
    return 0;
}