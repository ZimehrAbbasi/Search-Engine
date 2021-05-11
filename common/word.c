#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

char* normalize_word(char* s){
    
    char *lower = calloc(strlen(s)+1, sizeof(char));
    for (int i = 0; i < strlen(s); ++i) {
        lower[i] = tolower((unsigned char) s[i]);
    }
    return lower;

}

// int main(const int argc, const char* argv[]){
//     char* word = "wAR";
//     char* temp =  normalize_word(word);
//     printf("%s\n", temp);
//     free(temp);
// }