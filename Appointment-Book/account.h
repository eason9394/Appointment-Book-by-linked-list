#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MD5.h"



struct Account {
    char username[50];
    char hashed_password[33]; // 32 hex characters + null terminator
};

int account_system(char *);
void delete_account(const char *, int *);
void modify_password(const char *);
void login(int *, char *);
void register_account();
void hash_password(const char *, char *);
void printMenu(int);
