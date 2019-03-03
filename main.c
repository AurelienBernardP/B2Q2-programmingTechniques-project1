#include <stdio.h>
#include <stdlib.h>

#include "tries.h"
#include "dictionary.h"

int main(int argc, char** argv){

    if(argc != 3){
        printf("Usage: %s <dictionary file path> <word puzzle path>", argv[0]);
        return EXIT_FAILURE;
    }
    
    Node* dictionary = initDictionary(argv[1]);
    if(!dictionary) return EXIT_FAILURE;

    destroyTrie(dictionary);

    return EXIT_SUCCESS;
}