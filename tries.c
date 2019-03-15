#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#include "tries.h"

const size_t MAX_WORD = 200;

/*-------------------------------------------------------------------
*  STRUCTURE node_t:
*  This structure represents a node of the Ternary Search Trie.
*
*  FIELDS :
*     - letter: the information contained in the cell
*     - isEndOfWord : bolean value to symbolize the end of a word in the TST
*     - left, right, middle : pointers to the child of the current node.
--------------------------------------------------------------------*/
struct node_t{
    char letter;
    bool isEndOfWord;
    Node* left;
    Node* right;
    Node* middle; 
};

Node* initTrie(void){
    return NULL;
}

/*********************
 * printTrieHelper
 * Helper function to the printTrie funtion defined in this module;
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
void printTrie(Node* head, FILE* stream){
    char tmpWord[MAX_WORD];
    printTrieHelper(head, tmpWord, MAX_WORD, 0, stream);
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
 * Helper function to the insertWord funtion defined in this module;
 * 
 * *******************/
static Node* insertWordHelper(Node* head, char* word, size_t index){
    if(!word) return head;

    char newLetter = tolower(word[index]);
    if(newLetter == '\n')
        newLetter == '\0';

    if(!head){
            head = newNode(newLetter, false);
    }
    if(!head){
        return head;
    }
    
    if(newLetter < head->letter) // on va a chaque fois verifier les 3 conditions -> à changer
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
Node* insertWord(Node* head, char* word){
    return insertWordHelper(head, word, 0);
}


/*********************
 * isWordInTrieHelper
 * Helper function to the isWordInTrie funtion defined in this module;
 * 
 * *******************/
static bool isWordInTrieHelper(Node* head, char* word, size_t index){
    if(!head || !word)
        return false;
    
    char letter = tolower(word[index]);
    if((('\0' == letter) && ('\0'== head->letter)) || (word[index+1] == '\0' && head->isEndOfWord && word[index] == head->letter))
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
bool isWordInTrie(Node* head, char* word){
    return isWordInTrieHelper(head, word, 0);
}

void destroyTrie(Node* head){
    if(!head) return;

    destroyTrie(head->left);
    destroyTrie(head->middle);
    destroyTrie(head->right);
    free(head);
}
