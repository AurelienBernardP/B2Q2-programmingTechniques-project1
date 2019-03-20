/********************************
 * Module Handling the creation, modification
 * and deletion of Ternary Search Trie structure.
 * ******************************/

#ifndef __TRIES_HANDLER__
#define __TRIES_HANDLER__

#include <stdbool.h>
#include <stdlib.h>

/* Opaque Structure */
typedef struct node_t Node;
/* Opaque Structure */
typedef struct root_t Root;

/* ------------------------------------------------------------------------- *
 * Creates a new empty Ternary Search Trie (TST)object.
 *
 * PARAMETERS
 *  Void, no parameters
 *
 * RETURN
 *  Root pointer to a new TST on success
 *  NULL pointer on failure
 *
 * USE:
 *      Root* TSTNAME = initRoot();
 * ------------------------------------------------------------------------- */
Root* initRoot(void);

/* ------------------------------------------------------------------------- *
 * Inserts a word in an previously created TST.
 *
 * PARAMETERS
 * Root : Root pointer to the root of the previously created TST
 * word : The word to be inserted.
 *         !! The format of the word must be a character array with '\n' or '\0'
 *            as a end of word character !!
 * 
 * RETURN
 * void, the word will be inserted on success,
 *
 * USE:
 *       i
 * ------------------------------------------------------------------------- */
void insertWord(Root* root, char* word);

/* ------------------------------------------------------------------------- *
 * Prints all the words in lower case contained in a TST, into the wanted file stream.
 *
 * PARAMETERS
 * head : Node pointer to the root of the previously created TST
 * stream : Pointer to a valid file stream, standard or previously opened in write("w") mode
 * 
 * RETURN
 * void, the word will be printed in the file stream.
 * Prints ERROR in stderr file stream if the trie is empty
 *
 * USE: with for example the standard outpout file stream
 *       printTrie(PreviouslyCreatedTSTPointer, stdout);
 * ------------------------------------------------------------------------- */
void printTrie(Root* root, FILE* stream);

/* ------------------------------------------------------------------------- *
 * Lookup for a word in a previously created TST. 
 *
 * PARAMETERS
 * root : Root pointer to the root of the previously created TST
 * word : The word to be looked up.
 *         !! The format of the word must be a character array with '\n' or '\0'
 *            as a end of word character !!
 * RETURN
 * 0 if the word is NOT in the TST
 * 1 if the word IS in the TST
 * 2 if the there are no words with a given prefix
 *
 * ------------------------------------------------------------------------- */
unsigned int isWordInTrie(Root* root, char* word);

/* ------------------------------------------------------------------------- *
 * Frees all the cells in the previously created TST.
 *
 * PARAMETERS
 * root : Root pointer to the root of the previously created TST
 * 
 * RETURN
 * void, the TST is destroyed
 *
 * USE: 
 *       destroyFromRoot(PreviouslyCreatedTSTPointer);
 * ------------------------------------------------------------------------- */
void destroyFromRoot(Root* root);

#endif