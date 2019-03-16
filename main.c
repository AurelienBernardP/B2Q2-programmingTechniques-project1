#include <stdio.h>
#include <stdlib.h>

#include "tries.h"
#include "dictionary.h"
#include "grid.h"

int main(int argc, char** argv){

 /*   if(argc != 3){
        printf("Usage: %s <dictionary file path> <word puzzle path>\n", argv[0]);
        return EXIT_FAILURE;
    }
*/
    Root* dictionary = initDictionary("tmp_dico.txt");
    if(!dictionary) return EXIT_FAILURE;

    Grid* grid = initGrid("tmp_grid.txt");
    if(!grid) return EXIT_FAILURE;

    findAllWords(grid,dictionary);


    printTrie(grid->found, stdout);


    destroyFromRoot(dictionary);
    destroyGrid(grid);
    
    return EXIT_SUCCESS;
}
