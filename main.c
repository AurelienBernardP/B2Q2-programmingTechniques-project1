#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tries.h"
#include "dictionary.h"
#include "grid.h"

//UPON et G
//G
//G
//H et Z

int main(int argc, char** argv){

    if(argc != 3){
        printf("Usage: %s <dictionary file path> <word puzzle path>\n", argv[0]);
        return EXIT_FAILURE;
    }
    clock_t start;

    double end2sum = 0, end3sum = 0, end1sum = 0, end4sum = 0;
    double endsum;
    for(size_t i = 0; i < 10; i++){
    start = clock();

    Root* dictionary = initDictionary(argv[1]);
    if(!dictionary) 
        return EXIT_FAILURE;
    end1sum += (((double)  (clock() - start)) / (double) CLOCKS_PER_SEC);

    Grid* grid = initGrid(argv[2]);
    if(!grid){
        destroyFromRoot(dictionary);
        return EXIT_FAILURE;
    }
    end2sum += ((double)  (clock() - start)) / CLOCKS_PER_SEC;
    
    if(findAllWords(grid,dictionary)){
        destroyFromRoot(dictionary);
        destroyGrid(grid);
        return EXIT_FAILURE;
    }
    end3sum += ((double)  (clock() - start)) / CLOCKS_PER_SEC;

    printTrie(getWordsFound(grid), stdout);

    end4sum += (((double)  (clock() - start)) / CLOCKS_PER_SEC);

    destroyFromRoot(dictionary);
    destroyGrid(grid);
    endsum += (((double)  (clock() - start)) / CLOCKS_PER_SEC);
    
    }

    printf("Init dictionary: %lf\n", end1sum/10.0);
    printf("Init grid: %lf\n", end2sum/10.0);
    printf("Parcours grid & dict: %lf\n", end3sum/10.0);
    printf("Print: %lf\n", end4sum/10.0);

    printf("Total moyenne: %lf \n", endsum/10.0);

    return EXIT_SUCCESS;
}
