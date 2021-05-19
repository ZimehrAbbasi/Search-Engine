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
int pageDir_files(const char* pageDir);
void load_index(index_t* index, FILE* fp);
int tokenize(char* tokens[] , char* query);
void _querier(index_t* index, const char* pageDir, int i);

int pageDir_files(const char* pageDir){
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
    return i;
}

int tokenize(char* tokens[] , char* query){
    int querylength = strlen(query);
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
    return words;
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

void load_index(index_t* index, FILE* fp){
    char* line;
    while((line = file_readLine(fp)) != NULL){
        parse(index, line);
        mem_free(line);
    }
    mem_free(line);
}

void _querier(index_t* index, const char* pageDir, int i){
    
    char *line = calloc(1,1), buffer[BUFFERSIZE];
    while(fgets(buffer, BUFFERSIZE, stdin))
    {
        line = realloc(line, strlen(line)+1+strlen(buffer));
        if(!line){
            printf("----------------------------------------------------------------------------------");
            exit(1);
        }
        strcat(line, buffer);
    }
    char* query = normalize_word(line);
    mem_free(line);
    char *tokens[50];
    int words = tokenize(tokens, query);
    
    // Print the normalized query
    int weights[words];
    printf("Query: ");
    for(int j = 0; j < words; j++){
        weights[j] = 0;
        printf("%s ", tokens[j]);
    }
    printf("\n");

    // Assign 1 to 'and', 2 to 'or'
    for(int j = 0; j < words; j++){
        if(strcmp(tokens[j], "and")==0){
            weights[j] = 1;
        }
        if(strcmp(tokens[j], "or")==0){
            weights[j] = 2;
        }
    }

    // Create the document array that stores the scores as a 2D array
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

    // Check for invalid entries
    //Check if the first or last element of the array are negative, i.e. 'and' 'or'
    if(docArray[0][0] < 0 || docArray[0][words-1] < 0){
        fprintf(stderr, "Invalid query...\n");
        exit(1);
    }

    // Check if consecutive words are (and, and), (and, or), (or, or), or (or, and)
    for(int x = 0; x < words-1; x++){
        if(docArray[0][x] < 0 && docArray[0][x+1] < 0){
            fprintf(stderr, "Invalid query...\n");
            exit(1);
        }
    }

    // For no precedence
    // for(int x = 0; x < i; x++){
    //     int cur = 0;
    //     for(int y = 0; y< words;y++){
    //         if(docArray[x][y] == 0){
    //             if(cur > docArray[x][y]){
    //                 cur = docArray[x][y]
    //             }
    //         }
    //         if(docArray[x][y] == -1){
    //             if(y != words-1){
    //                 if(docArray[x][y+1] < cur){
    //                     cur = docArray[x][y+1]
    //                 }
    //             }
    //         }
    //         if(docArray[x][y] == -2){
    //             if(y != words-1){
                    
    //                 cur += docArray[x][y+1]
                    
    //             }
    //         }
    //     }
    // }

    // Check the number of 'or's in the query
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

    // Create max score array for each document
    for(int x = 0; x < i; x++){
        int sum = 0;
        for(int y = 0; y< orCount+1;y++){
            sum += orArray[x][y];
        }
        scoreArray[x] = sum;
        sortedIndex[x] = x;
    }

    // Sort by score, sort the index array in the same way
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

    printf("\nSearch Results: \n");
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
        fclose(fp);
    }
    printf("----------------------------------------------------------------------------------\n");
}

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
                printf("----------------------------------------------------------------------------------");
                int num_files = pageDir_files(pagedir);
                while(true){
                    printf("\nQuery? ");
                    break;
                }
                _querier(index, pagedir, num_files);
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
