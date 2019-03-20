#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tries.h"
#include "dictionary.h"
#include "grid.h"

int main(int argc, char** argv){

    if(argc != 3){
        printf("Usage: %s <dictionary file path> <word puzzle path>\n", argv[0]);
        return EXIT_FAILURE;
    }
    clock_t start, end, end1, end2, end3;
    start = clock();


    Root* dictionary = initDictionary(argv[1]);
    if(!dictionary) return EXIT_FAILURE;
    end1 = clock();
    printf("Init dict = %lf \n", ((double)  (end1 - start)) / CLOCKS_PER_SEC);

    Grid* grid = initGrid(argv[2]);
    if(!grid) return EXIT_FAILURE;
    end2 = clock();
    printf("Init grid = %lf \n", ((double)  (end2 - start)) / CLOCKS_PER_SEC);

    findAllWords(grid,dictionary);
    end3 = clock();
    printf("Parcours = %lf \n", ((double)  (end3 - start)) / CLOCKS_PER_SEC);

    printTrie(getWordsFound(grid), stdout);

    destroyFromRoot(dictionary);
    destroyGrid(grid);
    
    end = clock();
    printf("Time spent = %lf \n", ((double)  (end - start)) / CLOCKS_PER_SEC);

    return EXIT_SUCCESS;
}
