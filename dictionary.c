#include <stdlib.h>
#include <stdio.h>

#include "tries.h"
#include "dictionary.h"

const size_t LONGEST_WORD_SIZE = 200;

Root* initDictionary(char* path){
    if(!path) return NULL;

    FILE* fp = fopen(path, "r");
    if(!fp){
        printf("Error opening the dictionary file\n");
        return NULL;
    }

    Root* root = initRoot();
    char word[200] = {'\0'};

    while(fgets(word, LONGEST_WORD_SIZE ,fp) != NULL)
        insertWord(root, word);

    fclose(fp);
    return root;
}