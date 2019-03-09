#include <stdio.h>
#include <stdlib.h>

#include "tries.h"
#include "dictionary.h"
#include "grid.h"

int main(int argc, char** argv){

    if(argc != 3){
        printf("Usage: %s <dictionary file path> <word puzzle path>", argv[0]);
        return EXIT_FAILURE;
    }

    Node* dictionary = initDictionary(argv[1]);
    if(!dictionary) return EXIT_FAILURE;

    Grid* grid = initGrid(argv[2]);
    if(!grid) return EXIT_FAILURE;

    findAllWords(grid,dictionary);

    char tmpWord[200] = {'\0'};
    FILE* fp = fopen("newDico.txt", "w");
    if(!fp){
        printf("File can't be opened.\n");
        return EXIT_FAILURE;
    }
    printTrie(grid->found, tmpWord, 200, 0, fp);
    fclose(fp);

    destroyTrie(dictionary);
    destroyGrid(grid);
    return EXIT_SUCCESS;
}
