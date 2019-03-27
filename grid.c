#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

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
 * Add the string given by "suffix" to the string "word"
 *
 * PARAMETERS
 *  word, the string to modify
 *  suffix, a string
 *
 * RETURN
 *  word, the suffix has been added
 * ------------------------------------------------------------------------- */
static char* addSuffix(char* word, char* suffix);

/* ------------------------------------------------------------------------- *
 * Based on the string "content", the rows of the line $line$ of the grid
 * are completed.
 *
 * PARAMETERS
 *  grid, a pointer to a structure of type Grid
 *  line, a natural representing the line of grid to which the rows must be completed
 *  content, a string containing words which are added to the grid
 *
 * RETURN
 *  void, the rows has been completed properly
 *  If an error occurs, prints a message
 * ------------------------------------------------------------------------- */
static void copyContentInGrid(Grid* grid, size_t line, char* content);


/* ------------------------------------------------------------------------- *
 * Delete the string given by "suffix" from the string "word"
 *
 * PARAMETERS
 *  word, the string to modify
 *  suffix, a string
 *
 * RETURN
 *  void, the suffix has been deleted
 * ------------------------------------------------------------------------- */
static void deleteSuffix(char* word, char* suffix);

/* ------------------------------------------------------------------------- *
 * Gives the length of a string
 *
 * PARAMETERS
 *  word, the string
 *
 * RETURN
 * length, a natural representing the length of the given string
 * 0, the string is empty
 * ------------------------------------------------------------------------- */
static size_t stringLength(char* word);

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
        printf("Error allocGrid: Allocation failed\n");
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
    newGrid->grid = calloc(newGrid->size, sizeof(char**));
    if(!newGrid->grid){
        printf("Error allocGrid: Allocation failed\n");
        destroyFromRoot(newGrid->wordsFound);
        free(newGrid);
        return NULL;
    }
    for(size_t i = 0; i < newGrid->size; i++){
        newGrid->grid[i] = calloc(newGrid->size, sizeof(char*));
        if(!newGrid->grid[i]){
            printf("Error allocGrid: Allocation failed\n");
            destroyFromRoot(newGrid->wordsFound);
            destroyGrid(newGrid);
            return NULL;
        }
        for(size_t j = 0; j<newGrid->size; j++){
            newGrid->grid[i][j] = calloc(MAX_CHAR_PER_SQR, sizeof(char));// array of char in each element of the matrix
            if(!newGrid->grid[i][j]){
                printf("Error allocGrid: Allocation failed\n");
                destroyFromRoot(newGrid->wordsFound);
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

static void copyContentInGrid(Grid* grid, size_t line, char* content){
    if(!grid || !content){
        printf("Error copyContentInGrid: grid or content empty!\n");
        return;
    }

    //Check if the length of the content is correct
    size_t contentLength = stringLength(content);
    if(contentLength > (grid->size * (MAX_CHAR_PER_SQR + 1)) && contentLength > (2 * grid->size)){
        printf("Error copyContentInGrid: The length of the content is not correct!\n");
        return;
    }

    //Copying each word of the content to a row of a line of the grid
    size_t i, j, row;
    row = 0;
    i = 0;
    while(row < grid->size){
        //For each word of the content, a row is associated to it
        j = 0;
        while(i < contentLength && content[i] != ' '){
            grid->grid[line][row][j] = content[i];
            i++;
            j++;
        }
        i++;
        row++;
    }

    return;
}

Grid* initGrid(char* path){
    if (!path){
        printf("Error initGrid: path to the file is empty\n");
        return NULL;
    }
    
    FILE *fp = fopen(path, "r");
    if(!fp){
        printf("Error initGrid: file does not exist\n");
        return NULL;
    }

    ///Calculate the number of elements in the grid by counting the spaces
    char* fileLine = calloc(MAX_CHAR_PER_LINE, sizeof(char));
    if(!fileLine){
        printf("Error initGrid: Allocation failed!\n");
        fclose(fp);
        return NULL;
    }

    fgets(fileLine,MAX_CHAR_PER_LINE,fp);
    size_t gridSize = 1;// 1 as there is no space before the first element
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

    //Initialisation of the grid based on the file
    for(size_t i = 0; i < gridSize ; i++){
        //Removing the EOL character
        if(fileLine[stringLength(fileLine) - 1] == '\n')
            fileLine[stringLength(fileLine) - 1] = '\0';
        
        //Completing the line of the grid based on the file
        copyContentInGrid(grid, i, fileLine);
        if(!grid->grid[i]){
            destroyGrid(grid);
            fclose(fp);
            free(fileLine);
            return NULL;
        }
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
    destroyFromRoot(getWordsFound(grid));
    free(grid);
    return;
}

static size_t stringLength(char* word){
    if(!word)
        return 0;

    //Counting the length of the word
    size_t length = 0;
    while(word[length] != '\0')
        length++;
    
    return length;
}

static char* addSuffix(char* word, char* suffix){
    if(!suffix && suffix[0] != '#')
        return word;

    //Check if the suffix can be added to the word
    size_t suffixLength = stringLength(suffix);
    size_t wordLength = stringLength(word);
    if((MAX_CHAR_PER_LINE < (wordLength + suffixLength))){
        printf("Error addSuffix: the max. size has been reached!");
        word = realloc(word, wordLength+suffixLength);
    }

    //Contenating the word and the suffix
    for(size_t i = 0; i < suffixLength; i++)
        word[wordLength + i] = suffix[i];

    return word;
}

static void deleteSuffix(char* word, char* suffix){
    if(!word || !suffix)
        return;
    
    //Deleting the suffix from the word
    size_t suffixLength = stringLength(suffix);
    size_t wordLength = stringLength(word);
    for(size_t i = 0; i < suffixLength; i++){
        int j = wordLength - suffixLength + i;
        if(j >= 0)
            word[j] = '\0';
    }
    return;
}

static void findAllWordsHelper(Grid* grid, Root* dict, size_t line, size_t col,
                                                bool* isVisited, char* word){

    //Keeping a trace of our path in the grid
    isVisited[(line * grid->size) + col] = true;

    //If it is an obstacle, the function stops
    if(grid->grid[line][col][0] == '#')
        return;

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

int findAllWords(Grid* grid, Root* dict){
    if(!grid || !dict){
        printf("Error findAllWords: Grid or dictionnary empty!\n");
        return -1;
    }

    bool* isVisited = calloc(grid->size * grid->size, sizeof(bool));
    if(!isVisited){
        printf("Error findAllWords: Allocation failed!\n");
        return -1;
    }

    char* word = calloc(MAX_CHAR_PER_LINE , sizeof(char));
    if(!word){
        printf("Error findAllWorlds: Allocation failed!\n");
        free(isVisited);
        return -1;
    }

    //Each cell of the grid is given as the origin of the word
    for(size_t i=0; i < grid->size; i++){
        for(size_t j=0; j < grid->size; j++){
            //If the cell is an obstacle, it can't be an origin
            if(grid->grid[i][j][0] != '#'){
                findAllWordsHelper(grid, dict, i, j, isVisited, addSuffix(word,grid->grid[i][j]));
                isVisited[(i * grid->size) + j] = false;
            }
            deleteSuffix(word, grid->grid[i][j]);
        }
    }

    free(word);
    free(isVisited);

    return 0;
}
