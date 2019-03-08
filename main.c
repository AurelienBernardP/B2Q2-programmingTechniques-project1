#include <stdio.h>
#include <stdlib.h>

#include "tries.h"
#include "dictionary.h"

int main(int argc, char** argv){

    if(argc != 3){
        printf("Usage: %s <dictionary file path> <word puzzle path>", argv[0]);
        return EXIT_FAILURE;
    }
    
    Node* dictionary = initDictionary(argv[1]);
    if(!dictionary) return EXIT_FAILURE;
    char tmpWord[200] = {'\0'};
    FILE* fp = fopen("newDico.txt", "w");
    printTrie(dictionary, tmpWord, 200, 0, fp);
    fclose(fp);
    printf("%d\n",isWordInTrie(dictionary, "IBIS", 0));
    printf("%d\n",isWordInTrie(dictionary, "IBISS", 0));
    printf("%d\n",isWordInTrie(dictionary, "KAKI", 0));

    destroyTrie(dictionary);

    return EXIT_SUCCESS;
}
