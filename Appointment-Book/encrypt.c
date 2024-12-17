#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encrypt.h"

void encriptfile(const char * filename)
{
    int key = KEY;
    FILE * fread = fopen(filename, "rb");
    FILE * fwrite = fopen(filename, "rb+");
    if((fread == NULL) || (fwrite == NULL)) {
        return;
    }

    char ch;
    while((ch = fgetc(fread)) != EOF) {
        fputc(ch ^ key, fwrite);        // using xor since it simple
        key = ch ^ key;          // related to previous result, it`s harder to guess how it encript, same as salt
    }

    fclose(fread);
    fclose(fwrite);
    return;
}

void decriptfile(const char * filename)
{
    int key = KEY;
    FILE * fread = fopen(filename, "rb");
    FILE * fwrite = fopen(filename, "rb+");
    if((fread == NULL) || (fwrite == NULL)) {
        return;
    }

    char ch;
    while((ch = fgetc(fread)) != EOF) {
        fputc(ch ^ key, fwrite);
        key = ch;
    }

    fclose(fread);
    fclose(fwrite);
    return;
}