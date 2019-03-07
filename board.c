#include<stdlib.h>
#include<stdio.h>


#include"board.h"//name to be changed
#include"tries.c"

const size_t MAX_CHAR_PER_SQR = 10;
const size_t MAX_CHAR_PER_LINE = 200;

Board* initBoard(char* path){
    if (!path){
        printf("Error transmiting file name\n");
        return NULL;
    }
    
    FILE *fp = fopen(path, "r");
    if(!fp){
        printf("Error opening the file contaning the letters matrix\n");
        return NULL;
    }

    ///begin the count of elements in the grid by counting the spaces
    char* firstLine = calloc(MAX_CHAR_PER_LINE, sizeof(char));
    if(!firstLine){
        printf("error allocating memmory\n");
        return NULL;
    }
    fgets(firstLine,MAX_CHAR_PER_LINE,fp);
    
    size_t boardSize = 1;// 1 as there is no space before the first element;
    for(size_t i = 0; firstLine[i] != '\n' && i < MAX_CHAR_PER_LINE; i++){
        if(firstLine[i] == ' ')
            boardSize++;
    }
    free(firstLine);
    // end of counting number of elements, stored in boardSize

    // allocation of the structure and matrix
    Board* newBoard = malloc(sizeof(Board));
    if(!newBoard){
        printf("Error during memory allocation\n");
        fclose(fp);
        return NULL;
    }

    newBoard->size = boardSize;
    newBoard->found = initTrie();
    newBoard->puzzle = malloc(newBoard->size * sizeof(char**));
    if(!newBoard->puzzle){
        printf("Error during memory allocation\n");
        free(newBoard);
        fclose(fp);
        return NULL;
    }

    for(size_t i = 0; i< newBoard->size; i++){
        newBoard->puzzle[i] = malloc(newBoard->size * sizeof(char*));
        if(!newBoard->puzzle[i]){
            printf("Error during memory allocation\n");
            destroyBoard(newBoard);
            fclose(fp);
            return NULL;
        }
        for(size_t j = 0; j<newBoard->size; j++){
            newBoard->puzzle[i][j] = malloc(MAX_CHAR_PER_SQR * sizeof(char));// array of char in each element of the matrix
            if(!newBoard->puzzle[i][j]){
                printf("Error during memory allocation\n");
                destroyBoard(newBoard);
                fclose(fp);
                return NULL;
            }
        }
    }
    //end memory allocation

    fclose(fp);
    return newBoard;// success
}

void destroyBoard(Board* board){
    if(!board)
        return;

    if(board->puzzle != NULL){
        for(size_t i = 0; i< board->size; i++){
            if(board->puzzle[i] != NULL){
                for(size_t j = 0; j<board->size; j++){
                    if(board->puzzle[i][j] != NULL)
                        free(board->puzzle[i][j]);// the MAX_CHAR_PER_SQR char vectors (char*)
                }
                free(board->puzzle[i]);//the pointer to the arrays of char vector pointers (char**)
            }
        }
        free(board->puzzle);// (char ***)
    }
    free(board);
    return;
}