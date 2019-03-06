#include <stdlib.h>
#include <stdio.h>

#include "tries.h"

const size_t LONGEST_WORD_SIZE = 200;

Node* initDictionary(char* path){
    if(!path) return NULL;

    FILE* fp = fopen(path, 'r');
    if(!fp){
        printf("Error opening the dictionary file\n");
        return NULL;
    }

    Node* head = initTrie();
    char word[LONGEST_WORD_SIZE];

    while(fgets(word, LONGEST_WORD_SIZE ,fp) != NULL){
        insertWord(head, word, 0);
    }
    fclose(fp);
    return head;
}