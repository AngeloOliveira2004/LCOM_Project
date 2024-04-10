#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    int size;
} DoublyLinkedList;

DoublyLinkedList* create_doubly_linked_list();

Node* createNode(int data);


void initializeList(DoublyLinkedList* list);

void insertAtBeginning(DoublyLinkedList* list, int data);
void insertAtEnd(DoublyLinkedList* list, int data);
void insertAtPosition(DoublyLinkedList* list, int data, int position);

void printList(DoublyLinkedList* list);

