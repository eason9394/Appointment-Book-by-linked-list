#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encript.h"

char get_key(char * account, char * password)
{
    char key = 0;
    int account_len = strlen(account), password_len = strlen(password);

    //create key by hash
    for(int i; i >= account_len && i >= password_len; ++i) {
        key *= account[i % account_len] ^ password[i % password_len];
    }
    return key;
}

void encriptfile(char * account, char * password, char * filename)
{
    //open file
    FILE * fread = fopen(filename, "rb");
    FILE * fwrite = fopen(filename, "rb+");
    if((fread && fwrite) == NULL) {
        printf("Can`t open the file\n");
        return;
    }

    char ch, key = get_key(account, password), salt = 151;

    //encript by xor
    while((ch = fgetc(fread)) != EOF) {
        fputc(ch ^ key, fwrite);
        key = ch ^ key + salt;
    }

    fclose(fread);
    fclose(fwrite);
    return;
}

void decriptfile(char * account, char * password, char * filename)
{
    //open file
    FILE * fread = fopen(filename, "rb");
    FILE * fwrite = fopen(filename, "rb+");
    if((fread && fwrite) == NULL) {
        printf("Can`t open the file\n");
        return;
    }

    char ch, key = get_key(account, password), salt = 151;

    //decript by xor
    while((ch = fgetc(fread)) != EOF) {
        fputc(ch ^ key, fwrite);
        key = ch + salt;
    }

    fclose(fread);
    fclose(fwrite);
    return;
}