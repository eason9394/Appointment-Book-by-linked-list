/*****
    還沒做好帳號的連動
    但帳號系統應該是好了?
*****/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "MD5.h"
#include "account.h"
#include "encrypt.h"


#define WHO_LEN 30
#define WHAT_LEN 20
#define WHEN_LEN 20
#define WHERE_LEN 20
#define MAX_REC 10
#define KEY_LEN 10


// The record type
typedef struct 
{
    char who[WHO_LEN];
    char what[WHAT_LEN];
    char when[WHEN_LEN];
    char where[WHERE_LEN];
}Record;

typedef struct Node{
        Record data;
        struct Node *next;
}Node;
/** These are the function prototypes **/

Node *ReadFromFile (Node *,int *, char *);
int menu(void);
Node *EnterRecord (Node *, int *);
void ViewDay (Node *);
void ViewWeek (Node *, int);
Node *Modify (Node *, int);
Node *Delete (Node *, int *);
int Search (Node *, int);
void Quit (Node *, int, char *);
void ShowDates (Node *, char *);
int strContain (char *, char *, int); // return 1 if the first string
                                      // is contained in the second
                                      // string

// Start of a main program
int main (void)
{
    Node *head=NULL;
    int quit = 0, choice, count=0;
    char fileName[100], current_user[WHO_LEN] = "";

    int login_status = account_system(current_user);
    
    if(login_status){
        snprintf(fileName, sizeof(fileName), "%s.txt", current_user);
        decriptfile(fileName);
        head=ReadFromFile (head, &count, current_user);
        encriptfile(fileName);

        printf("%d records read from file\n", count);

            while (! quit) {
            choice = menu();  // get a choice
            system("cls");
            switch (choice) { // process according to the choice
                case 1: head=EnterRecord(head, &count);
                        break;
                case 2: ViewDay(head);
                        break;
                case 3: ViewWeek(head, count);
                        break;
                case 4: head=Modify(head, count);
                        break;
                case 5: head=Delete(head, &count);
                        break;
                case 6: printf("Search --- record at %d\n",Search(head, count));
                        break;
                case 7: account_system(current_user);
                        break;
                case 9: Quit(head, count, current_user);
                        quit = 1;
                        break;
                default: printf("Please enter a choice 1-7 or 9 to quit\n");
            }
        }
    }


    while (head != NULL) {
        Node * temp = head;
        head=head->next;
        free(temp);
    }
    
    return 0;
}

Node *ReadFromFile(Node *head, int *count, char *username) {
    FILE *filePointer;
    char filename[WHO_LEN + 4];
    char buffer[256];

    snprintf(filename, sizeof(filename), "%s.txt", username);
    printf("Opening file: %s\n", filename);

    filePointer = fopen(filename, "r");
    if (filePointer == NULL) {
        printf("The file doesn't exist.\n");
        return head;
    }

    // 跳過第一行（密碼）
    if (fgets(buffer, sizeof(buffer), filePointer) == NULL) {
        fclose(filePointer);
        return head;
    }

    while (!feof(filePointer)) {
        Node *node = malloc(sizeof(Node));
        if (node == NULL) {
            printf("Memory allocation failed.\n");
            fclose(filePointer);
            return head;
        }
        node->next = NULL;

        // 讀取四行資料
        if (fgets(buffer, sizeof(buffer), filePointer)) {
            buffer[strcspn(buffer, "\n")] = '\0';
            strncpy(node->data.who, buffer, WHO_LEN - 1);
            node->data.who[WHO_LEN - 1] = '\0';
        } else break;

        if (fgets(buffer, sizeof(buffer), filePointer)) {
            buffer[strcspn(buffer, "\n")] = '\0';
            strncpy(node->data.what, buffer, WHAT_LEN - 1);
            node->data.what[WHAT_LEN - 1] = '\0';
        } else break;

        if (fgets(buffer, sizeof(buffer), filePointer)) {
            buffer[strcspn(buffer, "\n")] = '\0';
            strncpy(node->data.when, buffer, WHEN_LEN - 1);
            node->data.when[WHEN_LEN - 1] = '\0';
        } else break;

        if (fgets(buffer, sizeof(buffer), filePointer)) {
            buffer[strcspn(buffer, "\n")] = '\0';
            strncpy(node->data.where, buffer, WHERE_LEN - 1);
            node->data.where[WHERE_LEN - 1] = '\0';
        } else break;

        // 加入鏈結
        if (head == NULL) {
            head = node;
        } else {
            Node *current = head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = node;
        }
        (*count)++;
    }

    fclose(filePointer);
    return head;
}


int menu(void) {

    int pick;

    printf("\033[1;34m=========================================\033[0m\n");
    printf("\033[1;32m            Appointment Book             \033[0m\n");
    printf("\033[1;34m=========================================\033[0m\n");
    printf("\033[1;33m  1. Enter Record       4. Modify\n");
    printf("\033[1;33m  2. View Day           5. Delete\n");
    printf("\033[1;33m  3. View Week          6. Search\n");
    printf("\033[1;33m  7. Account System     9. Quit\n");
    printf("\033[1;34m=========================================\033[0m\n");
    printf("\033[1;36mPlease enter your choice: \033[0m");
    scanf("%d", &pick); // get a choice from the user
    getchar();//

    return pick; // pass the value back
}

Node *EnterRecord (Node *head, int * count)
{
    int i;  // for proof printing only;
    Node *temp=malloc(sizeof(Node));
    if (temp == NULL) {
        printf("Memory allocation failed.\n");
        return head;
    }
    temp->next=NULL;

    printf("\nEnterRecord -- to enter the who/what/when/where\n");

    printf("Please enter WHOM you have an appointment with: ");
    scanf("%s",temp->data.who);
    fflush(stdin);

    printf("Please enter WHAT the event is: ");
    scanf("%[^\n]%*c",temp->data.what);
    fflush(stdin);

    char date[WHEN_LEN];
    while (1) { // 重複要求輸入直到輸入有效日期
        printf("Please enter WHEN (yyyymmdd): ");
        scanf("%s", date);

        // 檢查日期是否有效
        struct tm timeinfo = {0};
        sscanf(date, "%4d%2d%2d", &timeinfo.tm_year, &timeinfo.tm_mon, &timeinfo.tm_mday);
        timeinfo.tm_year -= 1900; // 修正年份
        timeinfo.tm_mon -= 1;     // 修正月份 (1-12 -> 0-11)
        fflush(stdin);

        struct tm original = timeinfo; // 保存原始輸入的時間結構
        if (mktime(&timeinfo) != -1 && 
            timeinfo.tm_year == original.tm_year &&
            timeinfo.tm_mon == original.tm_mon &&
            timeinfo.tm_mday == original.tm_mday) {
            // 日期有效，保存日期
            strcpy(temp->data.when, date);
            break;
        } else {
            printf("Invalid date! Please try again.\n");
        }
    }

    printf("Please enter WHERE you have an appointment at: ");
    scanf("%[^\n]%*c",temp->data.where);
    fflush(stdin);

    (*count) ++ ;
    
    Node *current=head;
    if(current == NULL){//head = NULL;
        head=temp;
    }else{
        while(current->next != NULL){
            current = current->next;
        }
        current->next = temp;
    }
    return head;
}

void ViewDay(Node *head)
{
    char date[WHEN_LEN];
    struct tm timeinfo = {0}; // 移到函數開始處，統一使用這個變數

    printf("\nViewDay -- to show the appointments of a given day\n");

    while (1) { // 重複要求輸入直到輸入有效日期
        printf("Please enter the day (yyyymmdd) to view: ");
        scanf("%s", date);

        // 解析輸入日期
        if (sscanf(date, "%4d%2d%2d", &timeinfo.tm_year, &timeinfo.tm_mon, &timeinfo.tm_mday) != 3) {
            printf("Invalid date format! Please try again.\n");
            continue;
        }

        // 修正年份和月份格式
        timeinfo.tm_year -= 1900; // tm_year 從 1900 年開始的偏移量
        timeinfo.tm_mon -= 1;     // tm_mon 是 0-11 表示月份 (1-12月)

        struct tm original = timeinfo; // 保存原始輸入的時間結構

        // 使用 mktime 驗證日期
        if (mktime(&timeinfo) != -1 &&
            timeinfo.tm_year == original.tm_year &&
            timeinfo.tm_mon == original.tm_mon &&
            timeinfo.tm_mday == original.tm_mday) {
            // 日期有效
            break;
        } else {
            printf("Invalid date! Please try again.\n");
        }
    }

    // 格式化日期和星期幾
    char formatted_date[WHEN_LEN];
    char day_of_week[10]; // 儲存星期幾的名稱

    // 格式化日期 (yyyymmdd)
    strftime(formatted_date, sizeof(formatted_date), "%Y%m%d", &timeinfo);
    // 格式化星期幾
    strftime(day_of_week, sizeof(day_of_week), "%A", &timeinfo); // 完整星期名稱

    // 顯示日期與星期幾
    printf("%s (%s):\n", formatted_date, day_of_week);

    // 顯示該日期的約會
    ShowDates(head, formatted_date);

    return;
}

void ShowDates(Node *head, char date[]) {
    if (head == NULL) {
        printf("No appointments found.\n");
        return;
    }
    Node *current = head;
    int found = 0;

    while (current != NULL) {
        if (strncmp(date, current->data.when, 8) == 0) {
            printf("Who: %s\n", current->data.who);
            printf("What: %s\n", current->data.what);
            printf("When: %s\n", current->data.when);
            printf("Where: %s\n", current->data.where);
            found = 1;
        }
        current = current->next;
    }
    if(!found)  printf("No appointments found for the date %s.\n", date);
}

void ViewWeek(Node *head, int count) {
    char date[WHEN_LEN];
    struct tm timeinfo = {0};
    int valid_date = 0;

    printf("\nViewWeek -- to show the appointments of a given week\n");

    while (!valid_date) {
        printf("Please enter the day (yyyymmdd) to view: ");
        scanf("%s", date); // yyyymmdd

        // 解析輸入日期
        if (sscanf(date, "%4d%2d%2d", &timeinfo.tm_year, &timeinfo.tm_mon, &timeinfo.tm_mday) != 3) {
            printf("Invalid date format! Please try again.\n");
            continue;
        }

        // 修正年份和月份格式
        timeinfo.tm_year -= 1900; // tm_year 從 1900 年開始的偏移量
        timeinfo.tm_mon -= 1;     // tm_mon 是 0-11 表示月份 (1-12月)

        // 使用 mktime 驗證日期
        struct tm original_timeinfo = timeinfo; // 保存原始時間結構
        if (mktime(&timeinfo) != -1 &&
            timeinfo.tm_year == original_timeinfo.tm_year &&
            timeinfo.tm_mon == original_timeinfo.tm_mon &&
            timeinfo.tm_mday == original_timeinfo.tm_mday) {
            valid_date = 1; // 日期有效
        } else {
            printf("Invalid date! Please try again.\n");
        }
    }

    // 日期有效，開始顯示該周的約會
    for (int i = 0; i < 7; ++i) {
        // 格式化當前日期
        char formatted_date[WHEN_LEN];
        char day_of_week[10]; // 儲存星期幾的名稱

        // 格式化日期 (yyyymmdd)
        strftime(formatted_date, sizeof(formatted_date), "%Y%m%d", &timeinfo);
        // 格式化星期幾
        strftime(day_of_week, sizeof(day_of_week), "%A", &timeinfo); // 完整星期名稱

        // 顯示日期與星期幾
        printf("%s (%s):\n", formatted_date, day_of_week);

        // 顯示該日期的約會
        ShowDates(head, formatted_date);

        // 增加一天
        timeinfo.tm_mday += 1;
        mktime(&timeinfo); // 自動處理日期進位（如月份變更，年份變更，閏年處理）
    }
}

Node *Modify (Node *head, int count)
{
    printf("\nModify -- to modify a record\n");
    int index;//the data which user wants to modify
    printf("There are %d records. Please enter the index of the record to modify (0 to %d): ", count, count-1);

    scanf("%d",&index);
    //fflush(stdin);

    if(index<0 || index>=count){
        printf("The data doesn't exist.Please enter the index between 0 to %d.\n",count-1);
        return head;
    }
    
    Node *current=head;
    int i=0;

    while(i<index){
        current=current->next;
        i++;
    }

    printf("\nCurrent record:\n");
    printf("Who: %s\n", current->data.who);
    printf("What: %s\n", current->data.what);
    printf("When: %s\n", current->data.when);
    printf("Where: %s\n", current->data.where);

    int demand=0;
    while(1){

        printf("Which data you want to modify?\n");
        printf("1.who 2.what 3.when 4.where 5.all 6.quit\n");

        scanf("%d",&demand);
        fflush(stdin);
        printf("Please enter the new values.Press Enter to save the new data.\n");

        if(demand==1){
            printf("new_who:");
            scanf("%[^\n]%*c",current->data.who);//input until reach the \n

        }else if(demand==2){
            printf("new_what:");
            scanf("%[^\n]%*c",current->data.what);//input until reach the \n 

        }else if(demand==3){
            printf("new_when:");
            scanf("%[^\n]%*c",current->data.when);//input until reach the \n

        }else if(demand==4){
           printf("new_where:");
           scanf("%[^\n]%*c",current->data.where);//input until reach the \n

        }else if(demand==5){
            printf("new_who:");
            scanf("%[^\n]%*c",current->data.who);//input until reach the \n
            fflush(stdin);
            printf("new_what:");
            scanf("%[^\n]%*c",current->data.what);//input until reach the \n
            fflush(stdin);

            char new_date[WHEN_LEN];
            while (1) { // 日期檢測循環
                printf("new_when:");
                scanf("%[^\n]%*c", new_date); // input until reach the \n

                struct tm timeinfo = {0};
                sscanf(new_date, "%4d%2d%2d", &timeinfo.tm_year, &timeinfo.tm_mon, &timeinfo.tm_mday);
                timeinfo.tm_year -= 1900;
                timeinfo.tm_mon -= 1;

                struct tm original = timeinfo;
                if (mktime(&timeinfo) != -1 &&
                    timeinfo.tm_year == original.tm_year &&
                    timeinfo.tm_mon == original.tm_mon &&
                    timeinfo.tm_mday == original.tm_mday) {
                    strcpy(current->data.when, new_date);
                    break;
                } else {
                    printf("Invalid date! Please try again.\n");
                }
            }

            printf("new_where:");
            scanf("%[^\n]%*c",current->data.where);//input until reach the \n

        }else if(demand == 6){
            break;
        }else{
            printf("Please enter the numner between 1~6.\n");
        }
        fflush(stdin);    
    }
    return head;
}

Node *Delete (Node *head, int * count)
{
    int index;//data you want to delete

    printf("\nDelete --  to delete a record\n");
    printf("Which data you want to delete?\n");
    printf("There are %d records. Please enter the index of the record to delete (0 to %d): ", *count, *count-1);

    scanf("%d",&index);
    if(index<0 || index>=*count){
        printf("The data doesn't exist.Please enter the index between 0 to %d.\n",*count-1);
        return head;
    }

    Node *current=head,*prev=NULL;
    Node *temp=NULL;
    for(int i=0;i<index;i++){ //delete index(th) Node
        prev=current;
        current=current->next;
    }

    temp=current;
    if(prev==NULL){
        head=current->next;
        free(temp);
    }else{
        prev->next=current->next;
        current=current->next;
        free(temp);
    }
    --(*count);

    Node *test=head;//for test
    while(test != NULL){
        printf("%s\n",test->data.who);
        printf("%s\n",test->data.what);
        printf("%s\n",test->data.when);
        printf("%s\n",test->data.where);
        test=test->next;
    }
    return head;
}

int Search (Node *head, int count)
{
    //似乎要第二次y才可以跳出?
    char key[KEY_LEN];
    int length, i=0;
    char YorN='N';

    printf("\nSearch --- to search a record by one of when/what/who/where\n");
    printf("Please enter a keyword (less than 10 characters): ");
    scanf ("%s", key);
    length = strlen (key);

    Node *current=head;

    while (YorN == 'n' || YorN == 'N') {
        while (current != NULL && i<count && (strContain(key, current->data.who, length) == 0) &&
                           (strContain(key, current->data.what, length) == 0) &&
                           (strContain(key, current->data.when, length) == 0) &&
                           (strContain(key, current->data.where, length) == 0) ){
            ++ i ;
            current=current->next;
        }
        if(current==NULL){
            printf("Not found.\n");
            return -1;//not found
        }
        if (i < count) {
            printf("Who: %s\n", current->data.who);
            printf("What: %s\n", current->data.what);
            printf("When: %s\n", current->data.when);
            printf("Where: %s\n", current->data.where);
            printf("\nIs this the record? (Y or N): ");
            scanf(" %c", &YorN);
        }
        if (i >= count && (YorN == 'N' || YorN == 'n')) {
            printf("Not found.\n");
            return -1;//not found
        }
        current=current->next;
    }
    return (i-1);//the target index7
}

void Quit(Node *head, int count, char *username) {
    // 1. 讀取原始密碼
    char password_file[60];
    char stored_password[65] = {0};
    snprintf(password_file, sizeof(password_file), "%s.txt", username);
    
    decriptfile(password_file);
    FILE *password_fp = fopen(password_file, "r");
    if (password_fp != NULL) {
        fgets(stored_password, sizeof(stored_password), password_fp);
        fclose(password_fp);
        encriptfile(password_file);
    }

    // 2. 寫入新檔案
    FILE *fp = fopen(password_file, "w");
    if (fp == NULL) {
        printf("Cannot open the file.\n");
        return;
    }

    // 3. 先寫入密碼
    if (strlen(stored_password) > 0) {
        stored_password[strcspn(stored_password, "\n")] = '\0';
        fprintf(fp, "%s\n", stored_password);
    }

    // 4. 寫入預約資料
    Node *current = head;
    while (current != NULL) {
        fprintf(fp, "%s\n%s\n%s\n%s\n", 
            current->data.who,
            current->data.what,
            current->data.when,
            current->data.where);
        current = current->next;
    }

    fclose(fp);
    encriptfile(password_file);
    printf("Records saved successfully.\n");

    // 5. 釋放記憶體
    current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}


int strContain (char * S1, char * S2, int length)
{
    int i;
    if (S1 == NULL || S2 == NULL) {
        return 0; 
    }

    if (length > strlen(S2)) {
        return 0; 
    }
    i = strlen(S2) - length;  // assume the length of S2 is greater
                              //than S1's

    for (; i>=0; --i)
        if(strncmp(S1, S2+i, length) == 0) return 1; // if found, then finish

    return 0; // not found
}