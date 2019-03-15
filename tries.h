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

/* ------------------------------------------------------------------------- *
 * Creates a new empty Ternary Search Trie (TST)object.
 *
 * PARAMETERS
 * Void, no parameters
 *
 * RETURN
 * Node pointer to a new TSt
 *
 * USE:
 * 
 *      Node* TSTNAME = initTrie();
 * ------------------------------------------------------------------------- */
Node* initTrie(void);

/* ------------------------------------------------------------------------- *
 * Inserts a word in an previously created TST.
 *
 * PARAMETERS
 * head : Node pointer to the root of the peviously created TST
 * word : The word to be inserted.
 *         !! The format of the word must be a character array with '\n' or '\0'
 *            as a end of word charachter !!
 * 
 * RETURN
 * Node pointer to the TST with the new word inserted.
 *
 * USE:
 *       PreviouslyCreatedTSTPointer = insertWord(PreviouslyCreatedTSTPointer, word);
 * ------------------------------------------------------------------------- */
Node* insertWord(Node* head, char* word);

/* ------------------------------------------------------------------------- *
 * Prints all the words in lower case contained in a TST, into the wanted file stream.
 *
 * PARAMETERS
 * head : Node pointer to the root of the peviously created TST
 * stream : Pointer to a valid file stream, standard or previously opened in write("w") mode
 * 
 * RETURN
 * void, the word will be printed in the file stream.
 *
 * USE: with for example the standard outpout file stream
 *       printTrie(PreviouslyCreatedTSTPointer, stdout);
 * ------------------------------------------------------------------------- */
void printTrie(Node* head, FILE* stream);

/* ------------------------------------------------------------------------- *
 * Lookup for a word in a previously created TST. 
 *
 * PARAMETERS
 * head : Node pointer to the root of the peviously created TST
 * word : The word to be looked up.
 *         !! The format of the word must be a character array with '\n' or '\0'
 *            as a end of word charachter !!
 * RETURN
 * 0 if the word is NOT in the TST
 * 1 if the word IS in the TST
 *
 * ------------------------------------------------------------------------- */
bool isWordInTrie(Node* head, char* word);

/* ------------------------------------------------------------------------- *
 * Frees all the cells in the previously created TST.
 *
 * PARAMETERS
 * head : Node pointer to the root of the peviously created TST
 * 
 * RETURN
 * void, the TST is destroyed
 *
 * USE: 
 *       destroyTrie(PreviouslyCreatedTSTPointer);
 * ------------------------------------------------------------------------- */
void destroyTrie(Node* head);

#endif