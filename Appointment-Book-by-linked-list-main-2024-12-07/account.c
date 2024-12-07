#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MD5.h"

#define ACCOUNT_FILE "accounts.txt"

struct Account {
    char username[50];
    char hashed_password[33]; // 32 hex characters + null terminator
};

// Function to hash a password
void hash_password(const char *password, char *hashed_password) {
    uint8_t digest[16];
    MD5_CTX context;

    MD5_Init(&context);
    MD5_Update(&context, (uint8_t *)password, strlen(password));
    MD5_Final(digest, &context);

    for (int i = 0; i < 16; i++) {
        sprintf(&hashed_password[i * 2], "%02x", digest[i]); // Convert to hex
    }
}

// Function to save an account to the file
int save_account(const struct Account *account) {
    FILE *file = fopen(ACCOUNT_FILE, "a"); // Append mode
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    fprintf(file, "%s,%s\n", account->username, account->hashed_password);
    fclose(file);
    return 1;
}

// Function to find an account by username
int find_account(const char *username, struct Account *found_account) {
    FILE *file = fopen(ACCOUNT_FILE, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char file_username[50], file_hashed_password[33];
        if (sscanf(line, "%49[^,],%32s", file_username, file_hashed_password) == 2) {
            if (strcmp(file_username, username) == 0) {
                strcpy(found_account->username, file_username);
                strcpy(found_account->hashed_password, file_hashed_password);
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

// Function to update the entire file (helper for modify/delete)
int update_file(struct Account accounts[], int count) {
    FILE *file = fopen(ACCOUNT_FILE, "w");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s\n", accounts[i].username, accounts[i].hashed_password);
    }

    fclose(file);
    return 1;
}

// Function to register a new account
void register_account() {
    struct Account account;
    char password[50];

    printf("Enter username: ");
    scanf("%[^\n]%*c", account.username);
    fflush(stdin);

    // Check if username already exists
    struct Account existing_account;
    if (find_account(account.username, &existing_account)) {
        printf("Username already exists. Please try again.\n");
        return;
    }

    printf("Enter password: ");
    scanf("%49s", password);

    // Hash the password
    hash_password(password, account.hashed_password);

    // Save the account
    if (save_account(&account)) {
        printf("Account successfully registered!\n");
    } else {
        printf("Error saving account. Please try again.\n");
    }
}

// Function to log in to an existing account
void login() {
    char username[50], password[50], hashed_password[33];
    struct Account account;

    printf("Enter username: ");
    scanf("%[^\n]%*c", username);
    fflush(stdin);

    // Find the account
    if (!find_account(username, &account)) {
        printf("Account not found. Please register first.\n");
        return;
    }

    printf("Enter password: ");
    scanf("%49s", password);

    // Hash the entered password and compare
    hash_password(password, hashed_password);
    if (strcmp(hashed_password, account.hashed_password) == 0) {
        printf("Login successful!\n");
    } else {
        printf("Incorrect password.\n");
    }
}

// Function to modify a password
void modify_password() {
    char username[50], old_password[50], new_password[50], hashed_old[33], hashed_new[33];
    struct Account account;

    printf("Enter username: ");
    scanf("%[^\n]%*c", username);
    fflush(stdin);

    if (!find_account(username, &account)) {
        printf("Account not found.\n");
        return;
    }

    printf("Enter old password: ");
    scanf("%49s", old_password);
    hash_password(old_password, hashed_old);

    if (strcmp(hashed_old, account.hashed_password) != 0) {
        printf("Incorrect password.\n");
        return;
    }

    printf("Enter new password: ");
    scanf("%49s", new_password);
    hash_password(new_password, hashed_new);

    // Update account password in memory
    strcpy(account.hashed_password, hashed_new);

    // Load all accounts and update the file
    struct Account accounts[100];
    int count = 0;
    FILE *file = fopen(ACCOUNT_FILE, "r");

    if (!file) {
        perror("Error opening file");
        return;
    }

    while (fscanf(file, "%49[^,],%32s\n", accounts[count].username, accounts[count].hashed_password) == 2) {
        count++;
    }

    fclose(file);

    for (int i = 0; i < count; i++) {
        if (strcmp(accounts[i].username, username) == 0) {
            strcpy(accounts[i].hashed_password, hashed_new);
            break;
        }
    }

    if (update_file(accounts, count)) {
        printf("Password successfully updated.\n");
    } else {
        printf("Error updating password.\n");
    }
}

// Function to delete an account
void delete_account() {
    char username[50], password[50], hashed_password[33];
    struct Account account;

    printf("Enter username: ");
    scanf("%[^\n]%*c", username);
    fflush(stdin);

    if (!find_account(username, &account)) {
        printf("Account not found.\n");
        return;
    }

    printf("Enter password: ");
    scanf("%49s", password);
    hash_password(password, hashed_password);

    if (strcmp(hashed_password, account.hashed_password) != 0) {
        printf("Incorrect password.\n");
        return;
    }

    // Load all accounts and update the file
    struct Account accounts[100];
    int count = 0;
    FILE *file = fopen(ACCOUNT_FILE, "r");

    if (!file) {
        perror("Error opening file");
        return;
    }

    while (fscanf(file, "%49[^,],%32s\n", accounts[count].username, accounts[count].hashed_password) == 2) {
        count++;
    }

    fclose(file);

    // Remove the account
    int index = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(accounts[i].username, username) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Error: Account not found during deletion.\n");
        return;
    }

    for (int i = index; i < count - 1; i++) {
        accounts[i] = accounts[i + 1];
    }

    count--;

    if (update_file(accounts, count)) {
        printf("Account successfully deleted.\n");
    } else {
        printf("Error deleting account.\n");
    }
}

// Main function
void account_system() {
    int choice;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Modify Password\n");
        printf("4. Delete Account\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        fflush(stdin);
        
        switch (choice) {
            case 1:
                register_account();
                break;
            case 2:
                login();
                break;
            case 3:
                modify_password();
                break;
            case 4:
                delete_account();
                break;
            case 5:
                printf("Exiting...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
