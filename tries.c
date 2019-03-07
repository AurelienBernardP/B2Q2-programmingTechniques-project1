#include <stdlib.h>
#include <stdbool.h>

#include "tries.h"


struct node_t{
    char letter;
    bool isEndOfWord;
    Node* left;
    Node* right;
    Node* middle; 
};

Node* initTrie(void){
    return NULL;
}

Node* newNode(char newLetter, bool isEnd){
    Node* newNode = malloc(sizeof(Node));
    if(!newNode) return NULL;

    newNode->letter = newLetter;
    newNode->isEndOfWord = isEnd;
    newNode->left = newNode->right = newNode->middle = NULL;
    return newNode;
}

void insertWord(Node* head, char* word, size_t index){
    if(!word) return;
    
    char newLetter = word[index];
    if(!head){
        if(word[index+1] == '\0'){
            head = newNode(newLetter, true);
        }else{
            head = newNode(newLetter, false);
        }
    }
    if(!head)
        return;
    if(newLetter == '\0')
        return;

    if(newLetter < head->letter)
        head->left = insertWord(head->left, word, index);
    if(newLetter > head->letter)
        head->right = insertWord(head->right, word, index);
    if(newLetter == head->letter)
        head->middle = insertWord(head->middle, word, ++index);

    return;  
}

bool isWordInTrie(Node* head, char* word, size_t index){
    if(!head || !word)
        return false;
    
    char letter = word[index];
    if('\0' == letter == head->letter || (word[index+1] == '\0' && head->isEndOfWord && word[index] == head->letter))
        return true;

    if(letter < head->letter){
        return isWordInTrie(head->left, word, index);
    }
    if(letter > head->letter){
        return isWordInTrie(head->right, word, index);
    }
    if(letter == head->letter){
        return isWordInTrie(head->middle, word, ++index);
    }
        return true;///will never reach this condition
}

void destroyTrie(Node* head){
    if(!head) return;

    destroyTrie(head->left);
    destroyTrie(head->middle);
    destroyTrie(head->right);
    free(head);
}
