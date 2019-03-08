#ifndef __TRIES_HANDLER__
#define __TRIES_HANDLER__

#include <stdbool.h>
#include <stdlib.h>

typedef struct node_t Node;

Node* initTrie(void);

char obtLetter(Node* node);

Node* insertWord(Node* head, char* word, size_t index);

//void printTrie(Node* head);

bool isWordInTrie(Node* head, char* word, size_t index);

void destroyTrie(Node* head);

#endif