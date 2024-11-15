/*****
	Project 0 Version 4
	
	This prototype provides almost all functions except
	for delete and modify.
*****/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/** These are the function prototypes **/

void ReadFromFile (Record *, int *, char *);
int menu(void);
void EnterRecord (Record *, int *);
void ViewDay (Record [], int);
void ViewWeek (Record *, int);
void Modify (Record *, int);
void Delete (Record *, int *);
int Search (Record *, int);
void Quit (Record *, int, char *);
void ShowDates (Record *, int, char *);
int strContain (char *, char *, int); // return 1 if the first string
                                      // is contained in the second
                                      // string

// Start of a main program
int main (void)
{
    Record AppBook[MAX_REC];
    int quit = 0, choice, count=0;
    char fileName[WHO_LEN];

    ReadFromFile (AppBook, &count, fileName);
    printf("%d records read from file\n", count);

    while (! quit) {
        choice = menu();  // get a choice
        switch (choice) { // process according to the choice
            case 1: EnterRecord(AppBook, &count);
                    break;
            case 2: ViewDay(AppBook, count);
                    break;
            case 3: ViewWeek(AppBook, count);
                    break;
            case 4: Modify(AppBook, count);
                    break;
            case 5: Delete(AppBook, &count);
                    break;
            case 6: Search(AppBook, count);
                    printf("Search --- record at %d\n",Search(AppBook, count));
                    break;
            case 9: Quit(AppBook, count, fileName);
                    quit = 1;
                    break;
            default: printf("Please enter a choice 1-6 or 9 to quit\n");
        }
    }
}

void ReadFromFile (Record * Book, int * count, char buff[])//buff is filename
{
    FILE * filePointer;//create the file pointer 
    int i;

    printf("ReadFromFile -- ask the file name from user");
    printf(" and process the file\n");
    printf(" count is %d\n", * count);

    printf("Please enter a file name to open/save: ");
    scanf("%s", buff);//try to find the filename or save the filename

    printf("Opening file: %s ....\n", buff);
    if( (filePointer = fopen(buff,"r")) == NULL)//if file doesn't exist
        return;
    else
        while (fgets(Book[*count].who, WHO_LEN, filePointer) != NULL) {//fgets WHO_LEN the length
            Book[*count].who[strlen(Book[*count].who)-1] = '\0';//remove the newline from who
            fgets(Book[*count].what, WHAT_LEN, filePointer);
            Book[*count].what[strlen(Book[*count].what)-1] = '\0';//remove the newline from what
            fgets(Book[*count].when, WHEN_LEN, filePointer);
            Book[*count].when[strlen(Book[*count].when)-1] = '\0';//remove the newline from when
            fgets(Book[*count].where, WHERE_LEN, filePointer);
            Book[*count].where[strlen(Book[*count].where)-1] = '\0';//remove the newline from where
            (* count) ++ ;//increase the number of file
        }
     for (i = 0 ; i < *count ; ++i) {//data output
        printf("Who: %s\n", Book[i].who);
        printf("What: %s\n", Book[i].what);
        printf("When: %s\n", Book[i].when);
        printf("Where: %s\n", Book[i].where);
     }
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

void EnterRecord (Record Book[], int * count)
{
    int i;  // for proof printing only;

    printf("\nEnterRecord -- to enter the who/what/when/where\n");
    printf("Please enter WHOM you have an appointment with: ");
    scanf("%s",Book[*count].who);
    //gets(Book[*count].who);
    printf("Please enter WHAT the event is: ");
    scanf("%s",Book[*count].what);
    //gets(Book[*count].what);
    printf("Please enter WHEN (yymmddhhmm): ");
    scanf("%s",Book[*count].when);
    //gets(Book[*count].when);
    printf("Please enter WHERE you have an appointment at: ");
    scanf("%s",Book[*count].where);
    //gets(Book[*count].where);
    (*count) ++ ;

// these lines below are for testing only
    for (i = 0 ; i < *count ; ++i) {
        printf("Who: %s\n", Book[i].who);
        printf("What: %s\n", Book[i].what);
        printf("When: %s\n", Book[i].when);
        printf("Where: %s\n", Book[i].where);
     }
}

void ViewDay (Record Book[], int count)
{
    char date[WHEN_LEN];

    printf("\nViewDay -- to show the appointments of a given day\n");
    printf("Please enter the day (yyyymmdd) to view: ");
    scanf ("%s", date);

    ShowDates (Book, count, date);

    return ;
}

void ShowDates (Record Book[], int count, char date[])
{
    int i = 0;

    while (i < count) {
        while (i<count && (strncmp(date, Book[i].when,strlen(date)) != 0))//if i<count or can't match the date get out the loop
            ++ i ;
        if (i < count) {
            printf("Who: %s\n", Book[i].who);
            printf("What: %s\n", Book[i].what);
            printf("When: %s\n", Book[i].when);
            printf("Where: %s\n\n", Book[i].where);
        }
        i ++ ;
    }
    return ;
}

void ViewWeek (Record Book [], int count)//check the dates in all week(7 days)
{
    char date[WHEN_LEN];
    int i=0;

    printf("\nViewWeek -- to show the appointments of a given week\n");
    printf("Please enter the day (yyyymmdd) to view: ");
    scanf ("%s", date);

    do {
        ShowDates (Book, count, date);//yyyymmdd
        date[7] = date[7] + 1;    // increase one day per loop
        if ((date[7] - '0') > 9) {//char->int
            date [6] = date[6] + 1;
            date [7] = date[7] - 10;
        }
        ++ i;
    } while (i < 7);   // 7 days to increment
}

void Modify (Record Book [], int count)
{
    printf("\nModify -- to modify a record\n");
    int index;//the data which user wants to modify
    printf("There are %d records. Please enter the index of the record to modify (0 to %d): ", count, count-1);

    scanf("%d",&index);
    fflush(stdin);

    if(index<0 || index>=count){
        printf("The data doesn't exist.Please enter the index between 0 to %d.\n",count-1);
        return;
    }

    printf("\nCurrent record:\n");
    printf("Who: %s\n", Book[index].who);
    printf("What: %s\n", Book[index].what);
    printf("When: %s\n", Book[index].when);
    printf("Where: %s\n", Book[index].where);

    printf("Which data you want to modify?\n");
    printf("1.who 2.what 3.when 4.where 5.all\n");
    

    int demand;
    scanf("%d",&demand);
    fflush(stdin);
    printf("Please enter the new values.Press Enter to save the new data.\n");

    if(demand==1){
        printf("new_who:");
        scanf("%[^\n]%*c",Book[index].who);
    }else if(demand==2){
        printf("new_what:");
        scanf("%[^\n]%*c",Book[index].what); 
    }else if(demand==3){
        printf("new_when:");
        scanf("%[^\n]%*c",Book[index].when);
    }else if(demand==4){
        printf("new_where:");
        scanf("%[^\n]%*c",Book[index].where);
    }else if(demand==5){
        printf("new_who:");
        scanf("%[^\n]%*c",Book[index].who);
        fflush(stdin);
        printf("new_what:");
        scanf("%[^\n]%*c",Book[index].what); 
        fflush(stdin);
        printf("new_when:");
        scanf("%[^\n]%*c",Book[index].when);
        fflush(stdin);
        printf("new_where:");
        scanf("%[^\n]%*c",Book[index].where);
    }
    fflush(stdin);
}

void Delete (Record Book [], int * count)
{
    int index;//data you want to delete
    printf("\nDelete --  to delete a record\n");
    printf("Which data you want to delete?\n");
    printf("There are %d records. Please enter the index of the record to delete (0 to %d): ", *count, *count-1);
    scanf("%d",&index);
    if(index<0 || index>=*count){
        printf("The data doesn't exist.Please enter the index between 0 to %d.\n",*count-1);
        return;
    }
    for(int i=index;i<*count-1;i++){ //delete Book[index]
        Book[i]=Book[i+1];
    }
    --(*count);
    for(int i=0;i<*count;i++){
        printf("%s\n",Book[i].who);
        printf("%s\n",Book[i].what);
        printf("%s\n",Book[i].when);
        printf("%s\n",Book[i].where);
    }
}

int Search (Record Book [], int count)
{
    char key[KEY_LEN];
    int length, i=0;
    char YorN='N';

    printf("\nSearch --- to search a record by one of when/what/who/where\n");
    printf("Please enter a keyword (less than 10 characters): ");
    scanf ("%s", key);
    length = strlen (key);

    while (YorN == 'n' || YorN == 'N') {
        while (i<count && (strContain(key, Book[i].who, length) == 0) &&
                           (strContain(key, Book[i].what, length) == 0) &&
                           (strContain(key, Book[i].when, length) == 0) &&
                           (strContain(key, Book[i].where, length) == 0) )
            ++ i ;
        if (i < count) {
            printf("Who: %s\n", Book[i].who);
            printf("What: %s\n", Book[i].what);
            printf("When: %s\n", Book[i].when);
            printf("Where: %s\n\n", Book[i].where);
            printf("\nIs this the record? (Y or N): ");
            scanf(" %c", &YorN);
        }
        ++ i ;
        if (i >= count && (YorN == 'N' || YorN == 'n')) return -1;//not found
    }
    return (i-1);//the target index
}

void Quit (Record Book [], int count, char fileName[])
{
    int i;
    FILE * fp;

    printf("\nQuit --- It saves the current records ");
    printf("to the file and terminates the program.\n");

    printf("Saving to file: %s ... \n", fileName);
    fp = fopen ( fileName, "w");//data transmit
    for (i = 0; i<count; ++i)
        fprintf(fp, "%s\n%s\n%s\n%s\n", Book[i].who,
                                        Book[i].what,
                                        Book[i].when,
                                        Book[i].where );
}


int strContain (char * S1, char * S2, int length)
{
    int i;

    i = strlen(S2) - length;  // assume the length of S2 is greater
                              //than S1's

    for (; i>=0; --i)
        if(strncmp(S1, S2+i, length) == 0) return 1; // if found, then finish

    return 0; // not found
}

