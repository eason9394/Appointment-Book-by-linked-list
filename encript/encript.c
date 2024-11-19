#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encript.h"

/*
 *removed, the key will build in the account system
 */
/* char get_key(char * account, char * password)
{
    char key = 0;
    int account_len = strlen(account), password_len = strlen(password);
    for(int i; i >= account_len && i >= password_len; ++i) {            // make sure i won`t over the length
        key *= account[i % account_len] ^ password[i % password_len];   // use hash to turn account&password into a simple char
    }
    return key;
} */

void encriptfile(char * filename, char key)
{
    FILE * fread = fopen(filename, "rb");
    FILE * fwrite = fopen(filename, "rb+");
    if((fread && fwrite) == NULL) {
        printf("Cannot open the file\n");
        return;
    }

    char ch, salt = 151;
    while((ch = fgetc(fread)) != EOF) {
        fputc(ch ^ key, fwrite);        // using xor since it simple
        key = ch ^ key + salt;          // related to previous result, it`s harder to guess how it encript, same as salt
    }

    fclose(fread);
    fclose(fwrite);
    return;
}

void decriptfile(char * filename, char key)
{
    FILE * fread = fopen(filename, "rb");
    FILE * fwrite = fopen(filename, "rb+");
    if((fread && fwrite) == NULL) {
        printf("Cannot open the file\n");
        return;
    }

    char ch, salt = 151;
    while((ch = fgetc(fread)) != EOF) {
        fputc(ch ^ key, fwrite);
        key = ch + salt;
    }

    fclose(fread);
    fclose(fwrite);
    return;
}