/*
***************2024/11/17***************
LinkedList.c Ver 2
    1.finish linked list function
        -add modify
        -add search
                        writer : 松 
****************************************
*/
#include "LinkedList.h"
#include <stdio.h>
#include <string.h>

// Initialize a new linked list
void newList(List **listptr) {
    *listptr = (List *)malloc(sizeof(List));
    if (*listptr == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for the list.\n");
        return;
    }

    (*listptr)->head = NULL;
    (*listptr)->end = NULL;
    (*listptr)->count = 0;
}

// Insert a new node with data into the list
void insert(List **listptr, const char *who, const char *what, const char *when, const char *where) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for a new node.\n");
        return;
    }

    // Copy the data into the new node
    strncpy(newNode->who, who, WHOLEN - 1);
    newNode->who[WHOLEN - 1] = '\0';
    strncpy(newNode->what, what, WHATLEN - 1);
    newNode->what[WHATLEN - 1] = '\0';
    strncpy(newNode->when, when, WHENLEN - 1);
    newNode->when[WHENLEN - 1] = '\0';
    strncpy(newNode->where, where, WHERELEN - 1);
    newNode->where[WHERELEN - 1] = '\0';

    newNode->nextNodePtr = NULL;

    // Add the node to the list
    if ((*listptr)->count == 0) {
        (*listptr)->head = newNode;
    } else {
        (*listptr)->end->nextNodePtr = newNode;
    }

    (*listptr)->end = newNode;
    (*listptr)->count++;
}

// Delete a node at a specific position in the list
//position is 1-based
void deleteNode(List **listptr, int position) {
    if ((*listptr)->count == 0) {
        printf("The list is empty. Nothing to delete.\n");
        return;
    }

    if (position < 1 || position > (*listptr)->count) {
        printf("Invalid position. Please provide a position between 1 and %d.\n", (*listptr)->count);
        return;
    }

    Node *current = (*listptr)->head;
    Node *previous = NULL;

    // Traverse to the node at the specified position
    for (int i = 1; i < position; i++) {
        previous = current;
        current = current->nextNodePtr;
    }

    // Remove the node
    if (previous == NULL) {
        // Deleting the head node
        (*listptr)->head = current->nextNodePtr;
    } else {
        previous->nextNodePtr = current->nextNodePtr;
    }

    // Update the end pointer if the last node is deleted
    if (current == (*listptr)->end) {
        (*listptr)->end = previous;
    }

    free(current);
    (*listptr)->count--;

    printf("Node at position %d deleted successfully.\n", position);
}

void modify(List* list, int theCategoryOfChangeStaff, const char* updateString, int theCategoryOfFindStaff, const char* keystring){
    if(list->count == 0){
        printf("There's no appointment was written yet.\n");
        return;
    }

    Node ** theChangingNodeptr = search(list, theCategoryOfFindStaff, keystring);

    if(theChangingNodeptr == NULL){
        printf("there's no such appointment record can be found in the file.\n");
        return;
    }

    switch (theCategoryOfChangeStaff){
    case who:                 //under the condition of changing who string
        strcpy((*theChangingNodeptr)->who, updateString);
        break;

    case what:                 //under the condition of changing what string
        strcpy((*theChangingNodeptr)->what, updateString);
        break;

    case when:                 //under the condition of changing when string
        strcpy((*theChangingNodeptr)->when, updateString);
        break;

    case where:                 //under the condition of changing where string
        strcpy((*theChangingNodeptr)->where, updateString);
        break;

    default:
        printf("There is no such storied data type, please try again later.\n");
        break;
    }
}

Node** search(List* list, int theCategoryOfFindStaff, const char* keystring){
    if(list->count == 0){
        printf("There's no appointment was written yet.\n");
        return;
    }
    
    Node * searchingNode = list->head;

    while (searchingNode != NULL){

        int check;

        switch (theCategoryOfFindStaff) {
            case who:                 //under the condition of changing who string
            if(strstr(searchingNode->who, keystring) != NULL){
                printf("Who : %s\n", searchingNode->who);
                printf("What : %s\n", searchingNode->what);
                printf("When : %s\n", searchingNode->when);
                printf("Where : %s\n", searchingNode->where);
                printf("Is this the node you want to change?\n");
                printf("For yes enter 1, as for no enter 0");
                scanf("%d", &check);
                if(check == 1)  return &searchingNode;
            }
            break;

        case what:                 //under the condition of changing what string
            if(strstr(searchingNode->what, keystring) != NULL){
                printf("Who : %s\n", searchingNode->who);
                printf("What : %s\n", searchingNode->what);
                printf("When : %s\n", searchingNode->when);
                printf("Where : %s\n", searchingNode->where);
                printf("Is this the node you want to change?\n");
                printf("For yes enter 1, as for no enter 0");
                scanf("%d", &check);
                if(check == 1)  return &searchingNode;
            }
            break;

        case when:                 //under the condition of changing when string
            if(strstr(searchingNode->when, keystring) != NULL){
                printf("Who : %s\n", searchingNode->who);
                printf("What : %s\n", searchingNode->what);
                printf("When : %s\n", searchingNode->when);
                printf("Where : %s\n", searchingNode->where);
                printf("Is this the node you want to change?\n");
                printf("For yes enter 1, as for no enter 0");
                scanf("%d", &check);
                if(check == 1)  return &searchingNode;
            }
            break;

        case where:                 //under the condition of changing where string
            if(strstr(searchingNode->where, keystring) != NULL){
                printf("Who : %s\n", searchingNode->who);
                printf("What : %s\n", searchingNode->what);
                printf("When : %s\n", searchingNode->when);
                printf("Where : %s\n", searchingNode->where);
                printf("Is this the node you want to change?\n");
                printf("For yes enter 1, as for no enter 0");
                scanf("%d", &check);
                if(check == 1)  return &searchingNode;
            }
            break;

        default:
            printf("There is no such storied data type, please try again later.\n");
            break;
        }

        searchingNode = searchingNode->nextNodePtr;
    }
    if(searchingNode == NULL){
        printf("There's no such appointment record in the file.\n");
        return NULL;
    }
}
