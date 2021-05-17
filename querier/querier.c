#include <stdlib.h>
#include <stdio.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "../common/word.h"
#include <string.h>
#include <unistd.h>
#define BUFFERSIZE 10

typedef struct index{
    hashtable_t* ht;
} index_t;

void parse(index_t* index, char* line);
void load_index(index_t* index, FILE* fp);
void _querier(index_t* index);

void load_index(index_t* index, FILE* fp){
    char* line;
    while((line = file_readLine(fp)) != NULL){
        parse(index, line);
        mem_free(line);
    }
    mem_free(line);
}

char** tokenize(char* query, int *len){
    char **tokens = mem_malloc(strlen(query)+1);
    int i = 0;
    char *word = strtok(query, " ");
    while( word != NULL ) {
        tokens[i] = word;
        word = strtok(NULL, " ");
        i++;
    }
    *len = i;
    return tokens;
}

void _querier(index_t* index){
    
    char *query = calloc(1,1), buffer[BUFFERSIZE];
    while(fgets(buffer, BUFFERSIZE, stdin))
    {
        query = realloc(query, strlen(query)+1+strlen(buffer));
        if( !query ){
            exit(1);
        }
        strcat( query, buffer );
    }
    int* len = mem_malloc(sizeof(int));
    char** tokens = tokenize(query, len);
    int weights[*len];
    for(int j = 0; j < *len; j++){
        weights[j] = 0;
    }
    
    for(int j = 0; j < *len; j++){
        if(strcmp(tokens[j], "and")==0){
            weights[j] = 1;
        }
    }
    for(int j = 0; j < *len; j++){
        printf("%d\n", weights[j]);
    }

    
}

void parse(index_t* index, char* line){

    char delim[] = " ";
    char cpy[strlen(line)];
    strcpy(cpy, line);
    char *ptr = strtok(line, delim);
    char *word = strtok(cpy, delim);
    int docID = -1;
    int count = -1;
    int i = 0;
    while(ptr != NULL)
	{
        if(strcmp(ptr, "0") == 0){
            if(i%2==1){
                docID = atoi(ptr);
            }else{
                count = atoi(ptr);
                for(int j = 0; j < count;j++){
                    index_insert(index, word, docID);
                }
            }
            ptr = strtok(NULL, delim);
            i++;
            continue;
        }
		if(atoi(ptr) != 0){
            if(i%2==1){
                docID = atoi(ptr);
            }else{
                count = atoi(ptr);
                for(int j = 0; j < count;j++){
                    index_insert(index, word, docID);
                }
            }
        }

		ptr = strtok(NULL, delim);
        i++;
	}
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
        if(pagedir_val(pagedir)){
            FILE* fp;
            if((fp = fopen(filepath, "r")) != NULL){
                index_t* index = index_new(300);
                load_index(index, fp);
                _querier(index);
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