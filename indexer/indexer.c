#include <stdlib.h>
#include <stdio.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../libcs50/mem.h"
#include "../common/word.h"
#include <string.h>
#include <unistd.h>

typedef struct webpage {
  char* url;                               // url of the page
  char* html;                              // html code of the page
  size_t html_len;                         // length of html code
  int depth;                               // depth of crawl
} webpage_t;

typedef struct index{
    hashtable_t* ht;
} index_t;

static void indexPage(index_t* index, webpage_t* webpage, int DocId){
    int* pos = mem_malloc(sizeof(int));
    *pos = 0;
    while(true){
        char* word = webpage_getNextWord(webpage, pos);
        if(word == NULL){
            break;
        }
        if(strlen(word) <= 3){
            mem_free(word);
            continue;
        }
        char* norm = normalize_word(word);
        index_insert(index, norm, DocId);
        mem_free(word);
        mem_free(norm);
    }
    mem_free(pos);
}

static void indexBuild(const char* pageDirectory, FILE* fp){

    index_t* index = index_new(300);
    int i = 0;
    while(true){
        char* path = pagedir_add(pageDirectory, i);
        FILE *fp = fopen(path, "r");
        if(fp == NULL){
            mem_free(path);
            break;
        }
        mem_free(path);
        webpage_t* wp = pagedir_load(fp);
        if(wp != NULL){
            indexPage(index, wp, i);
        }
        i += 1;
        webpage_delete(wp);
        fclose(fp);
    }
    index_save(index, fp);
    fclose(fp);
    index_print(index, stdout);
    index_delete(index);
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
            if((fp = fopen(filepath, "w+")) != NULL){
                indexBuild(pagedir, fp);
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