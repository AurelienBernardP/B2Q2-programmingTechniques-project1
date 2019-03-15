#include <stdlib.h>
#include <stdio.h>

#include "tries.h"
#include "dictionary.h"

const size_t LONGEST_WORD_SIZE = 200;

Node* initDictionary(char* path){
    if(!path) return NULL;

    FILE* fp = fopen(path, "r");
    if(!fp){
        printf("Error opening the dictionary file\n");
        return NULL;
    }

    Node* head = initTrie();
    char word[200] = {'\0'};

    while(fgets(word, LONGEST_WORD_SIZE ,fp) != NULL)
        head = insertWord(head, word);

    fclose(fp);
    return head;
}