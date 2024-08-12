
//Encrypt letter 'A' using letter 'X'
#include <stdio.h>


#define KEY 'X'
#define CLEARTEXT 'A'

void printchar(char c, char *text){
    printf("%s = %c (0x%x)\n", text, c, c);
}

int main(){
    char cleartext, key, ciphertext, newcleartext;

    key = key;
    cleartext = CLEARTEXT;

    printchar(key, "key");
    printchar(cleartext, "cleartext");

    ciphertext = cleartext ^ key; //XOR
    printchar(ciphertext, "ciphertext");

    newcleartext = ciphertext ^ key;
    printchar(newcleartext, "newcleartext");

    return 0;
}