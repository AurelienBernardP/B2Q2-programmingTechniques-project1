#include <stdio.h>
#include <stdlib.h>
#include "tries.h"
#include "dictionary.h"


void testDico(Root* dico, char* path){
    if(!dico || !path){
        printf("error");
        return;
    }

    FILE* fp = fopen(path, "r");
    if(!fp){
        printf("error");
        return;
    }
    int hit = 0, words = 0, miss = 0;
    char word[200] = {'\0'};
    while(fgets(word,200, fp)){
        for(int i = 0; i<200; i++){
            if(word[i] == '\n')
                word[i] = '\0';
        }
        words++;
        if(isWordInTrie(dico, word)){
            hit++;
        }else{
            miss++;
        }
    }
    printf("got %d word\n", words);
    printf("got %d hits\n", hit);
    printf("got %d miss\n", miss);
    fclose(fp);
    return;
}

int main(int argc, char** argv){

    if(argc != 3){
        printf("Usage: %s <dictionary file path> <word puzzle path>", argv[0]);
        return EXIT_FAILURE;
    }
    
    Root* dictionary = initDictionary(argv[1]);
    if(!dictionary) return EXIT_FAILURE;
    testDico(dictionary, argv[1]);

    FILE* fp = fopen("newDico.txt","w");
    
    printTrie(dictionary, fp);
    printf("is word in trie %d\n",isWordInTrie(dictionary,"AA"));


    destroyFromRoot(dictionary);

    return EXIT_SUCCESS;
}