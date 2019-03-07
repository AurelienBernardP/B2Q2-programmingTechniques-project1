#ifndef __BOARD_HANDLER__
#define __BOARD_HANDLER__

#include "tries.h"
typedef struct{
    char* **puzzle;// matrix of pointers to char arrays
    size_t size;
    Node* found;
}Board;// name to be changed

Board* initBoard(char* path);

void destroyBoard(Board* board);

#endif