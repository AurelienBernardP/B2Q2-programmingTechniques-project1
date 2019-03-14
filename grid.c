#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#include"grid.h"
#include"tries.h"



#define NB_MAX_CHAR 10
#define LONGEST_WORD_DICT 200

const size_t MAX_CHAR_PER_SQR = 10;
const size_t MAX_CHAR_PER_LINE = 200;

static Grid* allocGrid(size_t gridSize);

static void findAllWordsAux(Grid* grid, Node* dict, size_t line, size_t col,
                                             bool* isVisited, char* word);

//Allocation du grid selon la taille de la grille du fichier
static Grid* allocGrid(size_t gridSize){
    // allocation of the structure and matrix
    Grid* newGrid = malloc(sizeof(Grid));
    if(!newGrid){
        printf("Error during memory allocation\n");
        return NULL;
    }

    newGrid->size = gridSize;
    newGrid->found = initTrie();
    newGrid->puzzle = malloc(newGrid->size * sizeof(char**));
    if(!newGrid->puzzle){
        printf("Error during memory allocation\n");
        free(newGrid);
        return NULL;
    }

    for(size_t i = 0; i < newGrid->size; i++){
        newGrid->puzzle[i] = malloc(newGrid->size * sizeof(char*));
        if(!newGrid->puzzle[i]){
            printf("Error during memory allocation\n");
            destroyGrid(newGrid);
            return NULL;
        }
        for(size_t j = 0; j<newGrid->size; j++){
            newGrid->puzzle[i][j] = malloc(MAX_CHAR_PER_SQR * sizeof(char));// array of char in each element of the matrix
            if(!newGrid->puzzle[i][j]){
                printf("Error during memory allocation\n");
                destroyGrid(newGrid);
                return NULL;
            }
        
        }


    }
    //end memory allocation

    return newGrid;

}


//Lis le fichier contenant la grille, alloue la grille et l'initialise
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
    char* fileLine = calloc(MAX_CHAR_PER_LINE, sizeof(char));
    if(!fileLine){
        printf("error allocating memmory\n");
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

    const char delim[2] = " ";

    //Initialisation of the grid based on the file
    for(size_t i = 0; i < gridSize ; i++){
        //To avoid EOL
        fileLine[strlen(fileLine)-1] = 0;

        grid->puzzle[i][0] = strcpy(grid->puzzle[i][0], strtok(fileLine,delim));
        for(size_t j = 1; j < gridSize; j++){
            grid->puzzle[i][j] = strcpy(grid->puzzle[i][j], strtok(NULL,delim));
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

//Supprime le suffixe d'une chaine de charateres (enleve les x derniers charactères du string)
static void deleteSuffixe(char* word, char* suffix){
    if(!word || suffix[0] == '#')
        return;
    
    size_t  n = strlen(suffix);
    for(size_t i = 0; i < n; i++)
        word[strlen(word)-1] = '\0';
    
    return;
}

static void findAllWordsAux(Grid* grid, Node* dict, size_t line, size_t col,
                                                bool* isVisited, char* word){

    if(word[strlen(word)-1] == '#'){
        word[strlen(word)-1] = '\0';
    }

    isVisited[(line * grid->size) + col] = true;
    if(isWordInTrie(dict, word, 0) && !isWordInTrie(grid->found, word, 0)){
        grid->found = insertWord(grid->found, word, 0);
        printf("Insert of %s \n",word);
    }
    
    //Gauche
    if(col > 0 && !isVisited[(line*grid->size)+col-1]){
        findAllWordsAux(grid, dict, line, col-1, isVisited, (strcat(word, grid->puzzle[line][col-1])));
        deleteSuffixe(word, grid->puzzle[line][col-1]);
        isVisited[(line*grid->size)+col-1] = false;
    }


    //Droite
    if(col < grid->size-1 && !isVisited[(line*grid->size)+col+1]){
        findAllWordsAux(grid, dict, line, col+1, isVisited, strcat(word, grid->puzzle[line][col+1]));
        deleteSuffixe(word, grid->puzzle[line][col+1]);
        isVisited[(line*grid->size)+col+1] = false;
    }

    if(line > 0){ 
        //haut
        if(!isVisited[((line-1)*grid->size)+col]){
        findAllWordsAux(grid, dict, line-1, col, isVisited, strcat(word, grid->puzzle[line-1][col]));
        deleteSuffixe(word, grid->puzzle[line-1][col]);
        isVisited[((line-1)*grid->size)+col] = false;
        }
        //Diag haut droit
        if(col < grid->size-1 && !isVisited[((line-1)*grid->size)+col+1]){
            findAllWordsAux(grid, dict, line-1, col+1, isVisited, strcat(word, grid->puzzle[line-1][col+1]));
            deleteSuffixe(word, grid->puzzle[line-1][col+1]);
            isVisited[((line-1)*grid->size)+col+1] = false;
        }
        //Diag haut gauche
        if(col > 0 && !isVisited[((line-1)*grid->size)+col-1]){
            findAllWordsAux(grid, dict, line-1, col-1, isVisited, strcat(word, grid->puzzle[line-1][col-1]));
            deleteSuffixe(word, grid->puzzle[line-1][col-1]);
            isVisited[((line-1)*grid->size)+col-1] = false;
        }
    }

    //bas
    if(line < grid->size-1)
        if(!isVisited[((line+1)*grid->size)+col]){
        findAllWordsAux(grid, dict, line+1, col, isVisited, strcat(word, grid->puzzle[line+1][col]));
        deleteSuffixe(word, grid->puzzle[line+1][col]);
        isVisited[((line+1)*grid->size)+col] = false;
        //Diag bas droit
        if(col < grid->size-1 && !isVisited[((line+1)*grid->size)+col+1]){
            findAllWordsAux(grid, dict, line+1, col+1, isVisited, strcat(word, grid->puzzle[line+1][col+1]));
            deleteSuffixe(word, grid->puzzle[line+1][col+1]);
            isVisited[((line+1)*grid->size)+col+1] = false;
        }
        //Diag bas gauche
        if(col > 0 && !isVisited[((line+1)*grid->size)+col-1]){
            findAllWordsAux(grid, dict, line+1, col-1, isVisited, strcat(word, grid->puzzle[line+1][col-1]));
            deleteSuffixe(word, grid->puzzle[line+1][col-1]);
            isVisited[((line+1)*grid->size)+col-1] = false;
        }
    }

}

//Parcours la grille et le dictionaire pour créer un nouveau Trie contenant
// tous les mots possibles de la grille qui sont dans le dictionnaire
void findAllWords(Grid* grid, Node* dict){
    if(!grid || !dict){
        printf("Error findAllWords: Grid or dictionnary empty!\n");
        return;
    }

    bool* isVisited = calloc(grid->size * grid->size, sizeof(bool));
    if(!isVisited){
        printf("Error: Allocation failed!\n");
        return;
    }

    char* word = malloc(sizeof(char) * LONGEST_WORD_DICT);
    if(!word) return;
    
    for(size_t i=0; i < grid->size; i++){
        for(size_t j=0; j < grid->size; j++){
            if(grid->puzzle[i][j][0] != '#')
                findAllWordsAux(grid, dict, i, j, isVisited, strcat(word,grid->puzzle[i][j]));
        }
    }

    return;
}
