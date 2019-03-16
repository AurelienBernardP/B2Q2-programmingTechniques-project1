#ifndef __DICTIONARY_HANDLER__
#define __DICTIONARY_HANDLER__
#include <stdlib.h>
#include "tries.h"

/* ------------------------------------------------------------------------- *
 * Loads in memmory as a Ternary Search Trie a dictionary contained in a file.
 * 
 * Important Notes:
 *      The format of that file should be of one word per line ending with exlusively a '\n' charachter.
 *      Words should all be of maximum 199 charachters (this can be changed)
 *      Dictionary should be destroyed at the end of it's use with the destroyTrie(Node* head) function.
 *
 * PARAMETERS
 * path : the path to the location of the dictionary file in the system.
 * 
 * RETURN
 *   On success:
 *      Node pointer to the TST root with all the words of the ditionnary inserted in it.
 *   On failure:
 *      NULL pointer and error message on the sandad output.
 * Use example:
 *       Node* NewDictionaryName = initDico("DictionaryPath");
 * ------------------------------------------------------------------------- */
Root* initDictionary(char* path);

#endif 