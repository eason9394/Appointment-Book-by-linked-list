#define WHOLEN 20
#define WHATLEN 20
#define WHENLEN 12
#define WHERELEN 20

typedef struct node
{
    //int phone;        i don't remeber rather we need phone number or not
    char who[WHOLEN];
    char what[WHATLEN];
    char when[WHENLEN];
    char where[WHERELEN];
    struct node * nextNodePtr;
}Node;

typedef struct list{
    Node * head;
    Node * end;
    int count;
}List;

void newList(List**);
void insert(List**, const char*, const char*, const char*, const char*);
void deleteNode(List**, int);

