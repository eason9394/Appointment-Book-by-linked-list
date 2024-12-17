#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "MD5.h"
#include <unistd.h> // for access() and unlink()
#include "account.h"
#include "encrypt.h"


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
    //hashed_password[32] = '\0';
}



// Function to register a new account
void register_account() {
    struct Account new_account;
    printf("Enter new username: ");
    scanf("%49s", new_account.username);
    fflush(stdin);

    // 檢查使用者檔案是否已存在
    char user_file[60];
    snprintf(user_file, sizeof(user_file), "%s.txt", new_account.username);

    FILE *file = fopen(user_file, "r");
    if (file) {
        fclose(file);
        printf("Account already exists.\n");
        return;
    }

    // 獲取密碼並進行雜湊
    char password[50];
    printf("Enter new password(Do not input blank): ");
    scanf("%49s", password);
    hash_password(password, new_account.hashed_password);

    // 創建使用者的 txt 檔案，寫入雜湊後的密碼
    file = fopen(user_file, "w");
    if (!file) {
        perror("Error creating user file");
        return;
    }
    fprintf(file, "%s\n", new_account.hashed_password);
    fclose(file);
    encriptfile(user_file);

    printf("Account created successfully.\n");
}

// Function to log in to an existing account
void login(int *login_status, char *current_user) {
    struct Account account;
    char buffer[256];  // 增加緩衝區
    
    printf("Enter username: ");
    scanf("%s", account.username);
    
    char user_file[60];
    snprintf(user_file, sizeof(user_file), "%s.txt", account.username);
    decriptfile(user_file);

    FILE *file = fopen(user_file, "r");
    if (!file) {
        printf("Account not found.\n");
        return;
    }

    // 只讀取第一行，最多讀取255個字符
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        printf("Error reading user data.\n");
        fclose(file);
        encriptfile(user_file);
        return;
    }
    fclose(file);
    encriptfile(user_file);

    // 去除換行符並複製到account.hashed_password
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(account.hashed_password, buffer, sizeof(account.hashed_password) - 1);
    account.hashed_password[sizeof(account.hashed_password) - 1] = '\0';

    char password[50];
    char input_hashed_password[33];
    printf("Enter password: ");
    scanf("%49s", password);
    hash_password(password, input_hashed_password);

    if (strcmp(account.hashed_password, input_hashed_password) == 0) {
        printf("Login successful.\n");
        *login_status = 1;
        strcpy(current_user, account.username);
    } else {
        printf("input %s\n", input_hashed_password);
        printf("stored %s\n", account.hashed_password);
        printf("Incorrect password.\n");
    }
    
}


// Function to modify a password
void modify_password(const char *current_user) {
    char new_password[50];
    char new_hashed_password[33];
    printf("Enter your new password (Do not input blank) :");
    scanf("%49s", new_password);
    hash_password(new_password, new_hashed_password);

    // 組合使用者檔案名
    char user_file[60];
    snprintf(user_file, sizeof(user_file), "%s.txt", current_user);

    // 讀取現有檔案的內容
    FILE *file = fopen(user_file, "r");
    if (!file) {
        printf("Error opening user file.\n");
        return;
    }

    // 跳過第一行（舊的雜湊密碼）
    char line[256];
    fgets(line, sizeof(line), file);

    // 讀取剩餘內容
    char remaining_data[1024] = "";
    while (fgets(line, sizeof(line), file)) {
        strcat(remaining_data, line);
    }
    fclose(file);

    // 以寫入模式打開檔案，寫入新的雜湊密碼和剩餘內容
    file = fopen(user_file, "w");
    if (!file) {
        printf("Error writing to user file.\n");
        return;
    }
    fprintf(file, "%s\n", new_hashed_password);
    fputs(remaining_data, file);
    fclose(file);

    printf("Password updated successfully.\n");
}

void delete_account(const char *current_user, int *login_status) {
    // 確認刪除操作
    char confirm;
    printf("Are you sure you want to delete your account? (y/n): ");
    scanf(" %c", &confirm);
    if (confirm != 'y' && confirm != 'Y') {
        printf("Account deletion canceled.\n");
        return;
    }

    // 組合使用者檔案名
    char user_file[60];
    snprintf(user_file, sizeof(user_file), "%s.txt", current_user);

    // 刪除檔案
    if (remove(user_file) == 0) {
        printf("Account deleted successfully.\n");
        // 注銷登入狀態
        *login_status = 0;
        // 清空 current_user，這裡由於是 const 指標，不能修改，但在 account_system 中已經清空
    } else {
        printf("Error deleting account.\n");
    }
}


// Main function
int account_system(char *current_user) {
    int login_status = 0;
    int choice;
    do {
        //printf("\nAccount System Menu:\n");
        printMenu(login_status);
        printf("\033[1;36mPlease enter your choice: \033[0m");
        scanf("%d", &choice);
        getchar(); // 清除緩衝區中的換行符
        system("cls");

        if (login_status) {
            switch (choice) {
                case 1:
                    modify_password(current_user);
                    break;
                case 2:
                    delete_account(current_user, &login_status);
                    break;
                case 3:
                    login_status = 0;
                    current_user[0] = '\0';
                    printf("Logged out successfully.\n");
                    break;
                case 4:
                    printf("Exiting Account System.\n\n");
                    return login_status;
                default:
                    printf("Invalid choice.\n");
            }
        } else {
            switch (choice) {
                case 1:
                    register_account();
                    break;
                case 2:
                    login(&login_status, current_user);
                    break;
                case 3:
                    printf("Exiting Account System.\n");
                    return login_status;
                default:
                    printf("Invalid choice.\n");
            }
        }
    } while (1);
}


void printMenu(int login_status)
{
    if (login_status) {
        printf("\n\n");
        printf("\033[1;34m=========================================\033[0m\n");
        printf("\033[1;32m        Account Management Menu          \033[0m\n");
        printf("\033[1;34m=========================================\033[0m\n");
        printf("\033[1;33m  1. Modify Password\n");
        printf("\033[1;33m  2. Delete Account\n");
        printf("\033[1;33m  3. Logout\n");
        printf("\033[1;33m  4. Exit\n");
        printf("\033[1;34m=========================================\033[0m\n");
        return;
    }
    printf("\033[1;34m=========================================\033[0m\n");
    printf("\033[1;32m              Account Menu               \033[0m\n");
    printf("\033[1;34m=========================================\033[0m\n");
    printf("\033[1;33m  1. Register\n");
    printf("\033[1;33m  2. Login\n");
    printf("\033[1;33m  3. Exit\n");
    printf("\033[1;34m=========================================\033[0m\n");
}