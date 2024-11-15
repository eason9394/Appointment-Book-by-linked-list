/*
***************2024/11/16***************
    1.finish linked list function
        -newList
        -insert
        -deleteNode
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