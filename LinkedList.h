typedef struct node
{
    int value;
    struct node * nextNodePtr;
}Node;

typedef struct list{
    Node * head;
    int count;
}List;

void newList(List**);
void insert(List**, int);
void deleteNode(List**, int);

