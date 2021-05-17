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

typedef struct index{
    hashtable_t* ht;
} index_t;


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
        index_t* index = index_new(300);
        const char* old = argv[1];
        const char* new = argv[2];
        FILE* fp = fopen(old, "r");
        if(fp != NULL){
            index_load(index, fp, parse);
            fclose(fp);
            FILE* fp1 = fopen(new, "w");
            if(fp1 != NULL){
                index_save(index, fp1);
                fclose(fp1);
                index_print(index, stdout);
                index_delete(index);
            }else{
            fprintf(stderr, "Second file invalid...\n");
        }
        }else{
            fprintf(stderr, "First file invalid...\n");
        }
    }else{
        fprintf(stderr, "Invalid arguments...\n");
    }
    
}