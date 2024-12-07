#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MD5.h"

#define ACCOUNT_FILE "accounts.txt"

struct Account {
    char username[50];
    char hashed_password[33]; // 32 hex characters + null terminator
};

void account_system();
void delete_account();
void modify_password();
void login();
void register_account();
int update_file(struct Account [], int);
int find_account(const char *, struct Account *);
int save_account(const struct Account *);
void hash_password(const char *, char *);

