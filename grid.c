#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#include"grid.h"
#include"tries.h"

/*-------------------------------------------------------------------
*  STRUCTURE grid_t:
*  This structure contains a grid of characters, its size, and a Ternary
*  Search Trie.
*
*  FIELDS :
*     - grid: Grid of characters
*     - size: The size of the grid
*     - wordsFound:  A Ternary Search Trie containing all the words possible from
                the grid that are in a dictionary.
--------------------------------------------------------------------*/
struct grid_t{
    char* **grid;
    size_t size;
    Root* wordsFound;
};


const size_t MAX_CHAR_PER_SQR = 10; //Number max. of characters in a case of the grid
const size_t MAX_CHAR_PER_LINE = 200; //The size of the longest word possible in the dictionnary

/* ------------------------------------------------------------------------- *
 * Allocates a structure of type Grid based on the size given
 *
 * PARAMETERS
 *  gridSize, the size of a grid
 *
 * RETURN
 *  newGrid: a pointer to an allocated structure of type Grid
 *  NULL: error at allocating a structure of type Grid
 *
 * ------------------------------------------------------------------------- */
static Grid* allocGrid(size_t gridSize);

/* ------------------------------------------------------------------------- *
 * Add the suffix given by "string" to the string "word"
 *
 * PARAMETERS
 *  word, the string which will be modified
 *  suffix, a string
 *
 * RETURN
 *  void, the suffix has been added
 * ------------------------------------------------------------------------- */
static char* addSuffix(char* word, char* suffix);

/* ------------------------------------------------------------------------- *
 * Delete the suffix given by "string" from the string "word"
 *
 * PARAMETERS
 *  word, the string which will be modified
 *  suffix, a string
 *
 * RETURN
 *  void, the suffix has been deleted
 * ------------------------------------------------------------------------- */
static void deleteSuffix(char* word, char* suffix);

/*********************
 * printTrieHelper
 * Helper function to the findAllWords function defined in this module
 * (more information in the report)
 * 
 * *******************/
static void findAllWordsHelper(Grid* grid, Root* dict, size_t line, size_t col,
                                                    bool* isVisited, char* word);

static Grid* allocGrid(size_t gridSize){
    //Allocation of a pointer to the structure of type Grid
    Grid* newGrid = malloc(sizeof(Grid));
    if(!newGrid){
        printf("Error during memory allocation\n");
        return NULL;
    }

    newGrid->size = gridSize;

    //Allocation of the TST which contains all the words found
    newGrid->wordsFound = initRoot();
    if(!newGrid->wordsFound){
        free(newGrid);
        return NULL;
    }

    //Allocating the matrix of string which represents the grid
    newGrid->grid = malloc(newGrid->size * sizeof(char**));
    if(!newGrid->grid){
        printf("Error during memory allocation\n");
        free(newGrid);
        return NULL;
    }
    for(size_t i = 0; i < newGrid->size; i++){
        newGrid->grid[i] = malloc(newGrid->size * sizeof(char*));
        if(!newGrid->grid[i]){
            printf("Error during memory allocation\n");
            destroyGrid(newGrid);
            return NULL;
        }
        for(size_t j = 0; j<newGrid->size; j++){
            newGrid->grid[i][j] = malloc(MAX_CHAR_PER_SQR * sizeof(char));// array of char in each element of the matrix
            if(!newGrid->grid[i][j]){
                printf("Error during memory allocation\n");
                destroyGrid(newGrid);
                return NULL;
            }
        
        }
    }//end memory allocation

    return newGrid;
}

Root* getWordsFound(Grid* grid){
    if(!grid) return NULL;

    return grid->wordsFound;
}

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

    ///Calculate the number of elements in the grid by counting the spaces
    char* fileLine = calloc(MAX_CHAR_PER_LINE, sizeof(char));
    if(!fileLine){
        printf("error allocating memory\n");
        return NULL;
    }
    fgets(fileLine,MAX_CHAR_PER_LINE,fp);

    size_t gridSize = 1;// 1 as there is no space before the first element;
    for(size_t i = 0; fileLine[i] != '\n' && i < MAX_CHAR_PER_LINE; i++){
        if(fileLine[i] == ' ')
            gridSize++;
    }
    // end of counting number of elements, stored in GridSize
    
    //Allocation of memory for the grid
    Grid* grid = allocGrid(gridSize);
    if(!grid){
        fclose(fp);
        free(fileLine);
        return NULL;
    }

    const char delim[2] = " ";//Delimitation between each cell in the grid

    //Initialisation of the grid based on the file
    for(size_t i = 0; i < gridSize ; i++){
        grid->grid[i][0] = strcpy(grid->grid[i][0], strtok(fileLine,delim));
        for(size_t j = 1; j < gridSize; j++)
            grid->grid[i][j] = strcpy(grid->grid[i][j], strtok(NULL,delim));
        fgets(fileLine,MAX_CHAR_PER_LINE,fp);
    }

    fclose(fp);
    free(fileLine);

    return grid;// success

}

void destroyGrid(Grid* grid){
    if(!grid)
        return;

    if(grid->grid != NULL){
        for(size_t i = 0; i< grid->size; i++){
            if(grid->grid[i] != NULL){
                for(size_t j = 0; j<grid->size; j++){
                    if(grid->grid[i][j] != NULL)
                        free(grid->grid[i][j]);// the MAX_CHAR_PER_SQR char vectors
                }
                free(grid->grid[i]);//the pointer to the arrays of char vector pointers
            }
        }
        free(grid->grid);
    }
    //destroyTrie(grid->wordsFound);
    free(grid);
    return;
}

static char* addSuffix(char* word, char* suffix){
    if(!suffix || suffix[0] == '#')
        return word;

    //Check if the suffixe can be added in the word
    if(((int)MAX_CHAR_PER_LINE - (int)(strlen(word) + strlen(suffix)) < 0)){
        printf("Error addSuffix: the word has reached its size limit!\n");
        return word;
    }

    //Adding the suffix
    size_t n = strlen(suffix);
    for(size_t i = 0; i <= n; i++)
        word[strlen(word)] = suffix[i];

    return word;
}

static void deleteSuffix(char* word, char* suffix){
    if(!word)
        return;
    
    //Deleting the suffix
    size_t n = strlen(suffix);
    for(size_t i = 0; i < n; i++)
        word[strlen(word)-1] = '\0';
    
    return;
}

static void findAllWordsHelper(Grid* grid, Root* dict, size_t line, size_t col,
                                                bool* isVisited, char* word){

    //Keeping a trace of our path in the grid
    isVisited[(line * grid->size) + col] = true;

    switch(isWordInTrie(dict, word)){
        case 1:
            //Adding the word to the words found
            insertWord(grid->wordsFound, word);
            break;
        case 2:
            //The word is not the prefix of any words of the dictionary
            return;
    }

    //Check if the left cell exists and if it has already been visited
    if(col > 0 && !isVisited[(line*grid->size)+col-1]){
        findAllWordsHelper(grid, dict, line, col-1, isVisited, addSuffix(word, grid->grid[line][col-1]));
        deleteSuffix(word, grid->grid[line][col-1]);
        isVisited[(line*grid->size)+col-1] = false;
    }

    //Check if the right cell exists and if it has already been visited
    if(col < grid->size-1 && !isVisited[(line*grid->size)+col+1]){
        findAllWordsHelper(grid, dict, line, col+1, isVisited, addSuffix(word, grid->grid[line][col+1]));
        deleteSuffix(word, grid->grid[line][col+1]);
        isVisited[(line*grid->size)+col+1] = false;
    }

    //Check if the upper cell exists
    if(line > 0){ 
        //Check if the upper cell has already been visited
        if(!isVisited[((line-1)*grid->size)+col]){
            findAllWordsHelper(grid, dict, line-1, col, isVisited, addSuffix(word, grid->grid[line-1][col]));
            deleteSuffix(word, grid->grid[line-1][col]);
            isVisited[((line-1)*grid->size)+col] = false;
        }
        //Check if the upper right cell exists and if it has already been visited
        if(col < grid->size-1 && !isVisited[((line-1)*grid->size)+col+1]){
            findAllWordsHelper(grid, dict, line-1, col+1, isVisited, addSuffix(word, grid->grid[line-1][col+1]));
            deleteSuffix(word, grid->grid[line-1][col+1]);
            isVisited[((line-1)*grid->size)+col+1] = false;
        }
        //Check if the upper left cell exists and if it has already been visited
        if(col > 0 && !isVisited[((line-1)*grid->size)+col-1]){
            findAllWordsHelper(grid, dict, line-1, col-1, isVisited, addSuffix(word, grid->grid[line-1][col-1]));
            deleteSuffix(word, grid->grid[line-1][col-1]);
            isVisited[((line-1)*grid->size)+col-1] = false;
        }
    }

    //Check if the lower cell exists
    if(line < grid->size-1){
        //Check if the lower cell has already been visited
        if(!isVisited[((line+1)*grid->size)+col]){
            findAllWordsHelper(grid, dict, line+1, col, isVisited, addSuffix(word, grid->grid[line+1][col]));
            deleteSuffix(word, grid->grid[line+1][col]);
            isVisited[((line+1)*grid->size)+col] = false;
        }
        //Check if the lower right cell exists and if it has already been visited
        if(col < grid->size-1 && !isVisited[((line+1)*grid->size)+col+1]){
            findAllWordsHelper(grid, dict, line+1, col+1, isVisited, addSuffix(word, grid->grid[line+1][col+1]));
            deleteSuffix(word, grid->grid[line+1][col+1]);
            isVisited[((line+1)*grid->size)+col+1] = false;
        }
        //Check if the lower left cell exists and if it has already been visited
        if(col > 0 && !isVisited[((line+1)*grid->size)+col-1]){
            findAllWordsHelper(grid, dict, line+1, col-1, isVisited, addSuffix(word, grid->grid[line+1][col-1]));
            deleteSuffix(word, grid->grid[line+1][col-1]);
            isVisited[((line+1)*grid->size)+col-1] = false;
        }
    }
}

void findAllWords(Grid* grid, Root* dict){
    if(!grid || !dict){
        printf("Error findAllWords: Grid or dictionnary empty!\n");
        return;
    }

    bool* isVisited = calloc(grid->size * grid->size, sizeof(bool));
    if(!isVisited){
        printf("Error findAllWords: Allocation failed!\n");
        return;
    }

    char* word = malloc(sizeof(char) * MAX_CHAR_PER_LINE);
    if(!word){
        printf("Error findAllWorlds: Allocation failed!\n");
        return;
    }

    //Going through the whole grid to found all the possible words from the gird
    for(size_t i=0; i < grid->size; i++){
        for(size_t j=0; j < grid->size; j++){
            if(grid->grid[i][j][0] != '#'){
                findAllWordsHelper(grid, dict, i, j, isVisited, addSuffix(word,grid->grid[i][j]));
                isVisited[(i * grid->size) + j] = false;
            }
            deleteSuffix(word, grid->grid[i][j]);
        }
    }
    
    return;
}
