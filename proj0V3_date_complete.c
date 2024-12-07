/*****
	Project 0 Version 4
	
	This prototype provides almost all functions except
	for delete and modify.
*****/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WHO_LEN 15
#define WHAT_LEN 20
#define WHEN_LEN 12
#define WHERE_LEN 20
#define MAX_REC 10
#define KEY_LEN 10

// The record type
typedef struct {
        char who[WHO_LEN];
        char what[WHAT_LEN];
        char when[WHEN_LEN];
        char where[WHERE_LEN];
        }
        Record;

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
    char fileName[WHO_LEN];

    head=ReadFromFile (head,&count, fileName);
    printf("%d records read from file\n", count);

    while (! quit) {
        choice = menu();  // get a choice
        switch (choice) { 
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
            case 6: 
                    printf("Search --- record at %d\n",Search(head, count));
                    break;
            case 9: Quit(head, count, fileName);
                    quit = 1;
                    break;
            default: printf("Please enter a choice 1-6 or 9 to quit\n");
        }
    }
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current=current->next;
        free(temp);
    }
}

Node *ReadFromFile (Node *head, int * count, char buff[])//buff is filename
{
    FILE * filePointer;
    int i;
    Record *temp=malloc(sizeof(Record));
    Node *rear=NULL;

    printf("ReadFromFile -- ask the file name from user");
    printf(" and process the file\n");
    printf(" count is %d\n", * count);

    printf("Please enter a file name to open/save(filename.txt): ");
    scanf("%s", buff);//try to find the filename or save the filename

    printf("Opening file: %s ....\n", buff);
    if( (filePointer = fopen(buff,"r")) == NULL){//if file doesn't exist
        printf("The file doesn't exist.\n");
        return head;
    }else{
        while (fgets(temp->who, WHO_LEN, filePointer) != NULL) {//fgets WHO_LEN the length
            //strlen include(\n);
            Node *node=malloc(sizeof(Node));
            node->next=NULL;
            //first node?
            if(head != NULL){
                rear->next=node;
            }else{
                head=node;
            }
            rear=node;

            strcpy(node->data.who, temp->who);
            node->data.who[strlen(node->data.who)-1] = '\0';//remove the newline from who
            fgets(node->data.what, WHAT_LEN, filePointer);
            node->data.what[strlen(node->data.what)-1] = '\0';//remove the newline from what
            fgets(node->data.when, WHEN_LEN, filePointer);
            node->data.when[strlen(node->data.when)-1] = '\0';//remove the newline from when
            fgets(node->data.where, WHERE_LEN, filePointer);
            node->data.where[strlen(node->data.where)-1] = '\0';//remove the newline from where
            (* count) ++ ;//increase the number of file
        }
    }

    Node *current=head;
    while(current != NULL){
        printf("Who: %s\n", current->data.who);
        printf("What: %s\n", current->data.what);
        printf("When: %s\n", current->data.when);
        printf("Where: %s\n", current->data.where); 
        current=current->next;   
    }

    free(temp);
    fclose(filePointer);
    return head;
}

int menu(void)
{
    int pick;

    printf("***************************************\n");
    printf("*      Appointment Book Services      *\n");
    printf("*      -------------------------      *\n");
    printf("*   1. Enter Record       4. Modify   *\n");
    printf("*   2. View Day           5. Delete   *\n");
    printf("*   3. View Week          6. Search   *\n");
    printf("*   9. Quit                           *\n");
    printf("***************************************\n");
    printf("\nPlease enter a choice:");

    scanf("%d", &pick); // get a choice from the user
    getchar();//

    return pick; // pass the value back
}

Node *EnterRecord (Node *head, int * count)
{
    int i;  // for proof printing only;
    Node *temp=malloc(sizeof(Node));
    temp->next=NULL;

    printf("\nEnterRecord -- to enter the who/what/when/where\n");

    printf("Please enter WHOM you have an appointment with: ");
    scanf("%s",temp->data.who);

    printf("Please enter WHAT the event is: ");
    scanf("%s",temp->data.what);

    printf("Please enter WHEN (yyyymmdd): ");
    scanf("%s",temp->data.when);

    printf("Please enter WHERE you have an appointment at: ");
    scanf("%s",temp->data.where);

    (*count) ++ ;
    
    Node *current=head;
    if(current == NULL){//head = NULL;
        head=temp;
    }else{
        while(current->next != NULL){
            current = current->next;
            printf("Who: %s\n", current->data.who);// these lines below are for testing only
            printf("What: %s\n", current->data.what);
            printf("When: %s\n", current->data.when);
            printf("Where: %s\n", current->data.where);
        }
        current->next = temp;
    }
    return head;

}

/*void ViewDay (Node *head)
{
    char date[WHEN_LEN];

    printf("\nViewDay -- to show the appointments of a given day\n");
    printf("Please enter the day (yyyymmdd) to view: ");
    scanf ("%s", date);

    ShowDates (head,date);

    return ;
}*/
void ViewDay(Node *head)
{
    char date[WHEN_LEN];

    printf("\nViewDay -- to show the appointments of a given day\n");
    printf("Please enter the day (yyyymmdd) to view: ");
    scanf("%s", date);

    // 解析輸入日期
    struct tm timeinfo = {0};
    sscanf(date, "%4d%2d%2d", &timeinfo.tm_year, &timeinfo.tm_mon, &timeinfo.tm_mday);

    // 修正年份和月份格式
    timeinfo.tm_year -= 1900; // tm_year 從 1900 年開始的偏移量
    timeinfo.tm_mon -= 1;     // tm_mon 是 0-11 表示月份 (1-12月)

    // 格式化日期和星期幾
    char day_of_week[10];
    strftime(day_of_week, sizeof(day_of_week), "%A", &timeinfo);

    // 顯示日期與星期幾
    printf("Appointments on %s (%s):\n", date, day_of_week);

    // 顯示該日期的約會
    ShowDates(head, date);

    return;
}

void ShowDates(Node *head, char date[]) {
    if (head == NULL) {
        printf("No appointments found.\n");
        return;
    }

    printf("Appointments for %s:\n",date);

    Node *current = head;
    int found = 0;

    while (current != NULL) {
        if (strncmp(date, current->data.when, 8) == 0) {
            printf("Who: %s\n", current->data.who);
            printf("What: %s\n", current->data.what);
            printf("When: %s\n", current->data.when);
            printf("Where: %s\n\n", current->data.where);
            found = 1;
        }
        current = current->next;
    }
    if(!found)printf("No appointments found for the date %s.\n\n", date);
}

void ViewWeek(Node *head, int count) {
    char date[WHEN_LEN];
    printf("\nViewWeek -- to show the appointments of a given week\n");
    printf("Please enter the day (yyyymmdd) to view: ");
    scanf("%s", date); // yyyymmdd

    // 解析輸入日期
    struct tm timeinfo = {0};
    sscanf(date, "%4d%2d%2d", &timeinfo.tm_year, &timeinfo.tm_mon, &timeinfo.tm_mday); // str->int

    // 修正年份和月份格式
    timeinfo.tm_year -= 1900; // tm_year 從 1900 年開始的偏移量
    timeinfo.tm_mon -= 1;     // tm_mon 是 0-11 表示月份 (1-12月)

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
    fflush(stdin);

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
            printf("new_when:");
            scanf("%[^\n]%*c",current->data.when);//input until reach the \n
            fflush(stdin);
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
    return (i+1);//the target index
}

void Quit (Node *head, int count, char fileName[])
{
    int i;
    FILE * fp;
    Node *current=head;

    if (head == NULL){
        printf("No records to save.\n");
        return;
    }

    printf("\nQuit --- It saves the current records ");
    printf("to the file and terminates the program.\n");

    printf("Saving to file: %s ... \n", fileName);
    fp = fopen ( fileName, "w");//data transmit
    while(current != NULL){
        fprintf(fp, "%s\n%s\n%s\n%s\n", current->data.who,
                                        current->data.what,
                                        current->data.when,
                                        current->data.where );
    current=current->next;
}
    fclose(fp);
    printf("The records saved successfully.");
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

