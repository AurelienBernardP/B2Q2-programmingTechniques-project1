#ifndef __GRID_HANDLER__
#define __GRID_HANDLER__

#include "tries.h"

/* Opaque Structure */
typedef struct grid_t Grid;

/* ------------------------------------------------------------------------- *
 * Allocates and initialises a structure of type Grid based on the content
 * of a file.
 *
 * PARAMETERS
 *  path, a string which represents the path to the file containing the grid
 *
 * RETURN
 *  grid: a pointer to an allocated and initialised structure of type Grid
 *  NULL: error at allocating or initialising a structure of type Grid
 *
 * ------------------------------------------------------------------------- */
Grid* initGrid(char* path);

/* ------------------------------------------------------------------------- *
 * Get the field "wordsFound" of a pointer to the structure of type Grid
 *
 * PARAMETERS
 *  grid, a pointer to the structure of type Grid
 *
 * RETURN
 *  grid->field: a pointer to the field wordsFound of the parameter
 *  NULL: the paramater grid given is empty
 *
 * ------------------------------------------------------------------------- */
Root* getWordsFound(Grid* grid);

/* ------------------------------------------------------------------------- *
 * Frees all the memory allocated for the structure of type Grid.
 *
 * PARAMETERS
 * grid : a pointer to a structure of type Grid
 * 
 * RETURN
 * void, the structure of type Grid is destroyed
*/
void destroyGrid(Grid* grid);

/* ------------------------------------------------------------------------- *
 * Check all the words possible from the grid that are also in the given
 * dictionary and save them in the structure of type Grid.
 * (more information in the report)
 *
 * PARAMETERS
 *  grid, a pointer to the structure of type Grid
 *  dict, a pointer to the structure of type Root
 *
 * RETURN
 * 0, success, all the words from the grid and the dictionary are saved.
 * -1; Prints a message if an error occurs
 * ------------------------------------------------------------------------- */
int findAllWords(Grid* grid, Root* dict);

#endif