#ifndef __GRID_HANDLER__
#define __GRID_HANDLER__

#include "tries.h"
typedef struct{
    char* **puzzle;// matrix of pointers to char arrays
    size_t size;
    Root* found;
}Grid;// name to be changed

Grid* initGrid(char* path);

void destroyGrid(Grid* grid);

void findAllWords(Grid* grid, Root* dict);

#endif