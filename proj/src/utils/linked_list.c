#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>

// Function to create a new doubly linked list
DoublyLinkedList* create_doubly_linked_list() {
    DoublyLinkedList* newList = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    if (newList == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    newList->head = NULL;
    newList->tail = NULL;
    newList->size = 0;
    return newList;
}


Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void initializeList(DoublyLinkedList* list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void insertAtBeginning(DoublyLinkedList* list, int data) {
    Node* newNode = createNode(data);
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        newNode->next = list->head;
        list->head->prev = newNode;
        list->head = newNode;
    }
    list->size++;
}

void insertAtEnd(DoublyLinkedList* list, int data) {
    Node* newNode = createNode(data);
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    }
    list->size++;
}

void insertAtPosition(DoublyLinkedList* list, int data, int position) {
    if (position < 0 || position > list->size) {
        printf("Invalid position\n");
        return;
    }
    if (position == 0) {
        insertAtBeginning(list, data);
    } else if (position == list->size) {
        insertAtEnd(list, data);
    } else {
        Node* newNode = createNode(data);
        Node* current = list->head;
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }
        newNode->next = current->next;
        newNode->prev = current;
        current->next->prev = newNode;
        current->next = newNode;
        list->size++;
    }
}


void printList(DoublyLinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}
