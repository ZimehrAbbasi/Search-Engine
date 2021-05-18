#include <stdlib.h>
#include <stdio.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "../libcs50/set.h"
#include "../common/word.h"
#include <string.h>
#include <unistd.h>
#define BUFFERSIZE 200
#define INT_MAX 2147483647

typedef struct index{
    hashtable_t* ht;
} index_t;

void parse(index_t* index, char* line);
void load_index(index_t* index, FILE* fp);
void _querier(index_t* index, const char* pageDir);
// static void item_print(FILE* fp, const char* key, void* item);

void load_index(index_t* index, FILE* fp){
    char* line;
    while((line = file_readLine(fp)) != NULL){
        parse(index, line);
        mem_free(line);
    }
    mem_free(line);
}

// char** ccc(char* query, int *len){
//     char *tokens[] = mem_malloc(strlen(query));
//     int i = 0;
//     char *word = strtok(query, " ");
//     while( word != NULL && strcmp(word, "(null)") != 0) {
//         tokens[i] = mem_malloc(strlen(word));
//         strcpy(tokens[i], word);
//         word = strtok(NULL, " ");
//         i++;
//     }
//     *len = i;
//     return tokens;
// }

void _querier(index_t* index, const char* pageDir){
    
    char *query = calloc(1,1), buffer[BUFFERSIZE];
    while(fgets(buffer, BUFFERSIZE, stdin))
    {
        query = realloc(query, strlen(query)+1+strlen(buffer));
        if( !query ){
            exit(1);
        }
        strcat( query, buffer );
    }
    int querylength = strlen(query);
    char *tokens[15];
    bool space = true;
    int words = 0;
    for(int i = 0; i< querylength;i++){
        if(query[i] == '\n'){
            query[i] = '\0';
            break;
        }
        if(query[i] == ' '){
            if(space == false){
                query[i] = '\0';
                words += 1;
            }
            space = true;
        }else{
            if(space == true){
                tokens[words] = &query[i];
            }
            space = false;
        }
    }
    words += 1;
    
    int weights[words];
    printf("Query: ");
    for(int j = 0; j < words; j++){
        weights[j] = 0;
        printf("%s ", tokens[j]);
    }
    printf("\n");
    for(int j = 0; j < words; j++){
        if(strcmp(tokens[j], "and")==0){
            weights[j] = 1;
        }
        if(strcmp(tokens[j], "or")==0){
            weights[j] = 2;
        }
    }
    int zeros = 0;
    for(int j = 0; j < words; j++){
        if(weights[j] == 0){
            zeros += 1;
        }
    }

    int i = 0;
    while(true){
        char* path = pagedir_add(pageDir, i);
        FILE *fp = fopen(path, "r");
        if(fp == NULL){
            mem_free(path);
            break;
        }
        mem_free(path);
        i += 1;
        fclose(fp);
    }

    int docArray[i][words];

    for(int j = 0; j < i; j++){
        char in[10];
        sprintf(in, "%d", i);
        for(int k = 0; k < words; k++){
            if(weights[k] == 0){
                int num = index_get(index, tokens[k], j);
                docArray[j][k] = num;
            }else{
                if(weights[k] == 1){
                    docArray[j][k] = -1;
                }
                if(weights[k] == 2){
                    docArray[j][k] = -2;
                }
            }
        }
    }
    mem_free(query);

    // TO ADD THE EDGE CASES

    if(docArray[0][0] < 0 || docArray[0][words-1] < 0){
        fprintf(stderr, "Invalid query...\n");
        exit(1);
    }

    for(int x = 0; x < words-1; x++){
        if(docArray[0][x] < 0 && docArray[0][x+1] < 0){
            fprintf(stderr, "Invalid query...\n");
            exit(1);
        }
    }

    int orCount = 0;
    for(int y = 0; y< words;y++){
        if(docArray[0][y] == -2){
            orCount += 1;
        }
    }
    
    int orArray[i][orCount+1];

    for(int x = 0; x < i; x++){
        int min = INT_MAX;
        int ticker = 0;
        for(int y = 0; y< words;y++){
            if(docArray[x][y] == -2){
                orArray[x][ticker] = min;
                ticker += 1;
                min = INT_MAX;
                continue;
            }
            if(docArray[x][y] == -1){
                continue;
            }
            if(docArray[x][y] < min){
                min = docArray[x][y];
                orArray[x][ticker] = min;
            }
        }
        orArray[x][ticker] = min;
    }
    
    int scoreArray[i];
    int sortedIndex[i];

    for(int x = 0; x < i; x++){
        int sum = 0;
        for(int y = 0; y< orCount+1;y++){
            sum += orArray[x][y];
        }
        scoreArray[x] = sum;
        sortedIndex[x] = x;
    }
    for(int x = 0; x < i; x++){
        for(int y = 0; y< i;y++){
            if(scoreArray[y] < scoreArray[x]){
                int temp1 = scoreArray[x];
                scoreArray[x] = scoreArray[y];
                scoreArray[y] = temp1;

                int temp2 = sortedIndex[x];
                sortedIndex[x] = sortedIndex[y];
                sortedIndex[y] = temp2;
            }
        }
    }

    printf("Search Results: \n\n");

    for(int x = 0; x < i; x++){
        if(scoreArray[x] == 0){
            continue;
        }
        char *path = pagedir_add(pageDir, x);
        FILE *fp = fopen(path, "r");
        if(fp == NULL){
            mem_free(path);
            break;
        }
        char *line = file_readLine(fp);
        printf("Score: %4d Document: %4d URL: %4s\n", scoreArray[x], sortedIndex[x], line);
        mem_free(line);
        mem_free(path);
    }
    printf("\n");
}

void parse(index_t* index, char* query){

    int querylength = strlen(query);
    bool space = true;
    int words = 0;
    char* firstword;
    char* count;
    char* Id;
    for(int i = 0; i< querylength;i++){
        if(query[i] == ' '){
            if(space == false){
                query[i] = '\0';
                if(words % 2 == 0 && words != 0){
                    for(int j = 0; j<atoi(count);j++){
                        index_insert(index, firstword, atoi(Id));
                    }
                }
                words += 1;
            }
            space = true;
        }else{
            if(space == true){
                if(i == 0){
                    firstword = &query[i];
                    space = false;
                    continue;
                }
                if(words%2 == 1){
                    Id = &query[i];
                }else{
                    count = &query[i];
                }
            }
            space = false;
        }
    }
    words += 1;
}

// static void item_print(FILE* fp, const char* key, void* item){
//     int i = *(int *)item;
//     char str[10];
//     sprintf(str, "%d", i);
//     fputs(key, fp);
//     fputs(" ", fp);
//     fputs(str, fp);
// }

int main(const int argc, const char* argv[]){
    
    if(argc == 3){
        const char* pagedir = argv[1];
        const char* filepath = argv[2];
        int r;
        int e;
        r = access(filepath, R_OK);
        e = access(filepath, F_OK);
        if( e == -1 ){
            fprintf(stderr,"File does not exist...\n");
            exit(1);
        }
        if( r == -1 ){
            fprintf(stderr,"File not readable...\n");
            exit(1);
        }
        printf("Staring processes...\n");
        if(pagedir_val(pagedir)){
            FILE* fp;
            if((fp = fopen(filepath, "r")) != NULL){
                printf("Creating index object...\n");
                index_t* index = index_new(300);
                printf("Loading in index...\n");
                load_index(index, fp);
                while(true){
                    printf("Query? ");
                    break;
                }
                _querier(index, pagedir);
                index_delete(index);
                fclose(fp);
            }else{
                fprintf(stderr, "Could not open file...\n");
            }
        }else{
            fprintf(stderr, "Page Directory Invalid...\n");
        }

    }else{
        fprintf(stderr, "Invalid arguments...\n");
    }
}
