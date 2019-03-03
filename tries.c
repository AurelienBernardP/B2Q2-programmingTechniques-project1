#include <stdlib.h>
#include <stdbool.h>

#include "tries.h"


struct node_t{
    char letter;
    //int code;  I don't think we need this in our application
    Node* left;
    Node* right;
    Node* middle; 
};

Node* initTrie(void){
    return NULL;
}

Node* newNode(char newLetter){
    Node* newNode = malloc(sizeof(Node));
    if(!newNode) return NULL;

    newNode->letter = newLetter;
    newNode->left = newNode->right = newNode->middle = NULL;
    return newNode;
}

Node* insertWord(Node* head, char* word, size_t index){
    if(!word) return head;
    
    char newLetter = word[index];
    if(!head)
        head = newNode(newLetter);
    if(!head)
        return head;
    if(newLetter == '\0')
        return head;

    if(newLetter < head->letter)
        head->left = insertWord(head->left, word, index);
    if(newLetter > head->letter)
        head->right = insertWord(head->right, word, index);
    if(newLetter == head->letter)
        head->middle = insertWord(head->middle, word, ++index);

    return head;  
}

/*    I think isWordInTrie returns true is the word that we are looking for is a prefix of a word in the trie 
**      i will think about it and fix it if it's the case.
**
**
*/
bool isWordInTrie(Node* head, char* word, size_t index){
    if(!head || !word)
        return false;
    
    char letter = word[index];
    if('\0' == letter)
        return true;

    if(letter < head->letter)
        return isWordInTrie(head->left, word, index);
    if(letter > head->letter)
        return isWordInTrie(head->right, word, index);
    if(letter == head->letter)
        return isWordInTrie(head->middle, word ++index);
}

void destroyTrie(Node* head){
    if(!head) return;

    detroyTrie(head->left);
    detroyTrie(head->middle);
    detroyTrie(head->right);
    free(head);
}
