#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#include "tries.h"

const size_t MAX_WORD = 200;
const size_t LETTERS_IN_ALPHABET = 26;

/*-------------------------------------------------------------------
*  STRUCTURE node_t:
*  This structure represents a node of the Ternary Search Trie.
*
*  FIELDS :
*     - letter: the information contained in the cell
*     - isEndOfWord : boolean value to symbolize the end of a word in the TST
*     - left, right, middle : pointers to the child of the current node.
--------------------------------------------------------------------*/
struct node_t{
    char letter;
    bool isEndOfWord;
    Node* left;
    Node* right;
    Node* middle; 
};

struct root_t{
    Node** roots;
};

/* ------------------------------------------------------------------------- *
 * Creates a new empty Ternary Search Trie (TST)object.
 *
 * PARAMETERS
 *  Void, no parameters
 *
 * RETURN
 *  Node pointer to a new TST on success
 *
 * USE:
 *      Node* TSTNAME = initTrie();
 * ------------------------------------------------------------------------- */
static Node* initTrie(void){
    return NULL;
}

Root* initRoot(void){
    
    Root* newRoot= malloc(sizeof(Root));
    if(!newRoot)
        return NULL;

    newRoot->roots = malloc(LETTERS_IN_ALPHABET * sizeof(Node*));
    if(!newRoot->roots){
        free(newRoot);
        fprintf(stderr,"Error in memory allocation\n");
        return NULL;
    }
    for(size_t i = 0; i<LETTERS_IN_ALPHABET; i++)
        newRoot->roots[i] = initTrie();
    
    return newRoot;
}



/*********************
 * printTrieHelper
 * Helper function to the printTrie function defined in this module;
 * 
 * *******************/
static void printTrieHelper(Node* head, char* word, size_t wordSize, size_t index, FILE* stream){
    if(!stream || !word || wordSize < index){
        fprintf(stderr, "Error uninitialized arguments or word too long");
        return;
    }

    word[index] = head->letter;
    if(head->isEndOfWord){
        word[index+1] = '\0';
        fprintf(stream, "%s\n",word);
    }

    if(head->left != NULL){
        printTrieHelper(head->left, word, wordSize, index, stream);// we DO NOT put the letter in the word and get to the left;
    }
    if(head->middle != NULL){
        word[index] = head->letter;
        ++index;
        printTrieHelper(head->middle, word, wordSize, index, stream);// we DO put the letter in the word and get to the middle;
        --index;
    }
    if(head->right != NULL){
        printTrieHelper(head->right, word, wordSize, index, stream);// we DO NOT put the letter in the word and get to the left;
    }
    return;
}
void printTrie(Root* root, FILE* stream){
    if(!root || !stream) return;// unapropriate arguments

    char tmpWord[MAX_WORD];
    for(size_t i = 0; i < LETTERS_IN_ALPHABET; i++)
        printTrieHelper(root->roots[i], tmpWord, MAX_WORD, 0, stream);
    return;
}


static Node* newNode(char newLetter, bool isEnd){
    Node* newNode = malloc(sizeof(Node));
    if(!newNode) return NULL;

    newNode->letter = newLetter;
    newNode->isEndOfWord = isEnd;
    newNode->left = newNode->right = newNode->middle = NULL;
    return newNode;
}

/*********************
 * insertWordHelper
 * Helper function to the insertWord function defined in this module;
 * 
 * *******************/
static Node* insertWordHelper(Node* head, char* word, size_t index){
    if(!word) return head;

    char newLetter = tolower(word[index]);
    if(newLetter == '\n')
        newLetter = '\0';

    if(!head){
            head = newNode(newLetter, false);
    }
    if(!head){
        return head;
    }
    
    if(newLetter < head->letter)
        head->left = insertWordHelper(head->left, word, index);
    if(newLetter > head->letter)
        head->right = insertWordHelper(head->right, word, index);
    if(newLetter == head->letter){    
        if(word[index+1] == '\n' || word[index+1] == '\0'){
            head->isEndOfWord = true;
            return head;
        }
        head->middle = insertWordHelper(head->middle, word, ++index);
    }
    return head;
}
void insertWord(Root* root, char* word){
    if(!root || !word || word[0] < 'A') // arguments are not formated correctly 
        return;

    size_t indexOfRoot = tolower(word[0]) - 'a';//compute the branch of the root in which the word could be
    root->roots[indexOfRoot] = insertWordHelper(root->roots[indexOfRoot], word, 0);

    return; 
}

/*********************
 * isWordInTrieHelper
 * Helper function to the isWordInTrie function defined in this module;
 * 
 * *******************/
static bool isWordInTrieHelper(Node* head, char* word, size_t index){
    if(!head || !word)
        return false;
    
    char letter = tolower(word[index]);
    if((('\0' == letter) && ('\0'== head->letter)) || (word[index+1] == '\0' && head->isEndOfWord && letter == head->letter))
        return true;

    if(letter < head->letter){
        return isWordInTrieHelper(head->left, word, index);
    }
    if(letter > head->letter){
        return isWordInTrieHelper(head->right, word, index);
    }
    if(letter == head->letter){
        return isWordInTrieHelper(head->middle, word, ++index);
    }
return true;///will never reach this condition
}

bool isWordInTrie(Root* root, char* word){
    if(!root || !word || word[0] < 'A'){ // arguments are not formated correctly 
        return false;
    }
    size_t indexOfRoot = tolower(word[0]) - 'a';//compute the branch of the root in which the word could be
    return isWordInTrieHelper(root->roots[indexOfRoot], word, 0);
}

/* ------------------------------------------------------------------------- *
 * Frees all the cells in the previously created TST.
 *
 * PARAMETERS
 * head : Node pointer to the root of the previously created TST
 * 
 * RETURN
 * void, the TST is destroyed
 *
 * USE: 
 *       destroyTrie(PreviouslyCreatedTSTPointer);
 * ------------------------------------------------------------------------- */
static void destroyTrie(Node* head){
    if(!head) return;

    destroyTrie(head->left);
    destroyTrie(head->middle);
    destroyTrie(head->right);
    free(head);
}

void destroyFromRoot(Root* root){
    if(!root)
        return;
    
    for(size_t i = 0; i < LETTERS_IN_ALPHABET; i++)
        destroyTrie(root->roots[i]);
    
    free(root->roots);
    free(root);
}