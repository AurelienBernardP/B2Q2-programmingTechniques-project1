#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

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

char obtLetter(Node* node){
    return node->right->letter;
}
/*
void printTrie(Node* head, char* word){
    if(!head) return;

    if(!head->left)
        printTrie(head->left);
    if(!head->middle){
        if(head->letter == '\0'){
            printf("%s\n", word);
            printTrie(head->middle,word);
        }else{
            printTrie(head->middle, (strcat(word, grid->puzzle[line][col-1])));
        char* tmp = word[strlen(word)-strlen(grid->puzzle[line][col-1])];//Does that work ?
        tmp = 0;
    
        }
    }
    if(!head->right)
        printTrie(head->right);
}
*/

void printTrie(Node* head, char* word, size_t wordSize, size_t index, FILE* stream){
    if(!stream || !word || wordSize < index){
        fprintf(stderr, "Error uninitialized arguments or word too long");
        return;
    }

    if(head->left != NULL){
        printTrie(head->left, word, wordSize, index, stream);// we DO NOT put the letter in the word and get to the left;
        for(size_t i = index; word[i] != '\0' && i < wordSize ; i++){
            word[i] = '\0';
        }
    }

    if(head->middle != NULL){
        word[index] = head->letter;
        //if we find the end of a word(ie, null char of true in the endOfWord field), we print the word in the filestream
        if(head->isEndOfWord || head->middle->letter == '\0'){
            fprintf(stream, "%s\n",word);
        }
        //word is printed in the filestream
        
        printTrie(head->middle, word, wordSize, ++index, stream);// we DO put the letter in the word and get to the middle;
        for(size_t i = index; word[i] != '\0' && i < wordSize ; i++){
            word[i] = '\0';
        }
    }

    if(head->right != NULL){
        printTrie(head->right, word, wordSize, index, stream);// we DO NOT put the letter in the word and get to the left;
        for(size_t i = index; word[i] != '\0' && i < wordSize ; i++){
            word[i] = '\0';
        }
    }

return;
}

static Node* newNode(char newLetter, bool isEnd){
    Node* newNode = malloc(sizeof(Node));
    if(!newNode) return NULL;

    newNode->letter = newLetter;
    newNode->isEndOfWord = isEnd;
    newNode->left = newNode->right = newNode->middle = NULL;
    return newNode;
}

Node* insertWord(Node* head, char* word, size_t index){
    if(!word) return head;

    char newLetter = word[index];
    if(!head){
            head = newNode(newLetter, false);
    }
    if(word[index+1] == '\0'){
        head->isEndOfWord = true;
    }
    if(!head || newLetter == '\0')
        return head;

    if(newLetter < head->letter) // on va a chaque fois verifier les 3 conditions -> à changer
        head->left = insertWord(head->left, word, index);
    if(newLetter > head->letter)
        head->right = insertWord(head->right, word, index);
    if(newLetter == head->letter)
        head->middle = insertWord(head->middle, word, ++index);
    
    return head;
}

bool isWordInTrie(Node* head, char* word, size_t index){
    if(!head || !word)
        return false;
    
    char letter = word[index];
    if((('\0' == letter) && ('\0'== head->letter)) || (word[index+1] == '\0' && head->isEndOfWord && word[index] == head->letter))
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
