#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#include"grid.h"
#include"tries.h"



#define NB_MAX_CHAR 10
#define LONGEST_WORD 200

const size_t MAX_CHAR_PER_SQR = 10;
const size_t MAX_CHAR_PER_LINE = 200;


static void findAllWordsAux(Grid* grid, Node* dict, size_t line, size_t col,
                                                bool* isVisited, char* word);

Grid* initGrid(char* path){
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
    
    size_t gridSize = 1;// 1 as there is no space before the first element;
    for(size_t i = 0; firstLine[i] != '\n' && i < MAX_CHAR_PER_LINE; i++){
        if(firstLine[i] == ' ')
            gridSize++;
    }
    free(firstLine);
    // end of counting number of elements, stored in GridSize

    // allocation of the structure and matrix
    Grid* newGrid = malloc(sizeof(Grid));
    if(!newGrid){
        printf("Error during memory allocation\n");
        fclose(fp);
        return NULL;
    }

    newGrid->size = gridSize;
    newGrid->found = initTrie();
    newGrid->puzzle = malloc(newGrid->size * sizeof(char**));
    if(!newGrid->puzzle){
        printf("Error during memory allocation\n");
        free(newGrid);
        fclose(fp);
        return NULL;
    }

    for(size_t i = 0; i< newGrid->size; i++){
        newGrid->puzzle[i] = malloc(newGrid->size * sizeof(char*));
        if(!newGrid->puzzle[i]){
            printf("Error during memory allocation\n");
            destroyGrid(newGrid);
            fclose(fp);
            return NULL;
        }
        for(size_t j = 0; j<newGrid->size; j++){
            newGrid->puzzle[i][j] = malloc(MAX_CHAR_PER_SQR * sizeof(char));// array of char in each element of the matrix
            if(!newGrid->puzzle[i][j]){
                printf("Error during memory allocation\n");
                destroyGrid(newGrid);
                fclose(fp);
                return NULL;
            }
        }
    }
    //end memory allocation

    fclose(fp);
    return newGrid;// success
}

void destroyGrid(Grid* grid){
    if(!grid)
        return;

    if(grid->puzzle != NULL){
        for(size_t i = 0; i< grid->size; i++){
            if(grid->puzzle[i] != NULL){
                for(size_t j = 0; j<grid->size; j++){
                    if(grid->puzzle[i][j] != NULL)
                        free(grid->puzzle[i][j]);// the MAX_CHAR_PER_SQR char vectors (char*)
                }
                free(grid->puzzle[i]);//the pointer to the arrays of char vector pointers (char**)
            }
        }
        free(grid->puzzle);// (char ***)
    }
    free(grid);
    return;
}

static void findAllWordsAux(Grid* grid, Node* dict, size_t line, size_t col,
                                                bool* isVisited, char* word){

    isVisited[line + col] = true;
    if(isWordInTrie(dict, word, 0))
        grid->found = insertWord(grid->found, word, 0);//insert en void ?
    
    //Gauche
    if(col > 0 && !isVisited[(line*grid->size)+col-1]){
        //Need biggest length of aword
        findAllWords(grid, dict, line, col-1, isVisited, (strcat(word, grid->puzzle[line][col-1])));
        char* tmp = word[strlen(word)-strlen(grid->puzzle[line][col-1])];//Does that work ?
        tmp = 0;
        isVisited[(line*grid->size)+col-1] = false;
    }


    //Droite
    if(col < grid->size-1 && !isVisited[(line*grid->size)+col+1]){
        findAllWords(grid, dict, line, col+1, isVisited, strcat(word, grid->puzzle[line][col+1]));
        char* tmp = word[strlen(word)-strlen(grid->puzzle[line][col+1])];//Does that work ?
        tmp = 0;
        isVisited[(line*grid->size)+col+1] = false;
    }

    //haut
    if(line > 0 && !isVisited[((line-1)*grid->size)+col]){
        findAllWords(grid, dict, line-1, col, isVisited, strcat(word, grid->puzzle[line-1][col]));
        char* tmp = word[strlen(word)-strlen(grid->puzzle[line-1][col])];//Does that work ?
        tmp = 0;
        isVisited[((line-1)*grid->size)+col] = false;
        //Diag haut droit
        if(col < grid->size-1 && !isVisited[((line-1)*grid->size)+col+1]){
            findAllWords(grid, dict, line-1, col+1, isVisited, strcat(word, grid->puzzle[line-1][col+1]));
            char* tmp = word[strlen(word)-strlen(grid->puzzle[line-1][col+1])];//Does that work ?
            tmp = 0;
            isVisited[((line-1)*grid->size)+col+1] = false;
        }
        //Diag haut gauche
        if(col > 0 && !isVisited[((line-1)*grid->size)+col-1]){
            findAllWords(grid, dict, line-1, col-1, isVisited, strcat(word, grid->puzzle[line-1][col-1]));
            char* tmp = word[strlen(word)-strlen(grid->puzzle[line-1][col-1])];//Does that work ?
            tmp = 0;
            isVisited[((line-1)*grid->size)+col-1] = false;
        }
    }

    //bas
    if(line < grid->size-1 && !isVisited[((line+1)*grid->size)+col]){
        findAllWords(grid, dict, line+1, col, isVisited, strcat(word, grid->puzzle[line+1][col]));
        char* tmp = word[strlen(word)-strlen(grid->puzzle[line+1][col])];//Does that work ?
        tmp = 0;
        isVisited[((line+1)*grid->size)+col] = false;
        //Diag bas droit
        if(col < grid->size-1 && !isVisited[((line+1)*grid->size)+col+1]){
            findAllWords(grid, dict, line+1, col+1, isVisited, strcat(word, grid->puzzle[line+1][col+1]));
            char* tmp = word[strlen(word)-strlen(grid->puzzle[line+1][col+1])];//Does that work ?
            tmp = 0;
            isVisited[((line+1)*grid->size)+col+1] = false;
        }
        //Diag haut gauche
        if(col > 0 && !isVisited[((line+1)*grid->size)+col-1]){
            findAllWords(grid, dict, line+1, col-1, isVisited, strcat(word, grid->puzzle[line-1][col-1]));
            char* tmp = word[strlen(word)-strlen(grid->puzzle[line+1][col-1])];//Does that work ?
            tmp = 0;
            isVisited[((line+1)*grid->size)+col-1] = false;
        }
    }

}

void findAllWords(Grid* grid, Node* dict){
    if(!grid || !dict){
        printf("Error: Grid or dictionnary empty!\n");
        return NULL;
    }

    bool* isVisited = calloc(grid->size, bool);
    if(!isVisited){
        printf("Error: Allocation failed!\n");
        return NULL;
    }

    char word[LONGEST_WORD]; 

    for(size_t i=0; i < grid->size; i++)
        for(size_t j=0; j < grid->size; j++){
            findAllWordsAux(grid, dict, i, j, isVisited, strcat(word,grid->puzzle[i][j]));
    }
    
}
