#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/mem.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/hash.h"

typedef struct index{
    hashtable_t* ht;
} index_t;

static void index_print_helper(FILE* fp, const char* key, void* item);
static void index_delete_helper(void* item);
static void index_iterate_helper(void* arg, const char* key, void* item);
static void counters_iterate_helper(void *arg, const int key, const int count);

index_t* index_new(const int num_slots){

    if(num_slots < 1){
        return NULL;
    }
    index_t* index = (index_t*)mem_malloc(sizeof(index_t));
    if(index != NULL){
        index->ht = hashtable_new(num_slots);
    }
    
    return index;
}

bool index_insert(index_t* index, const char* key, int DocID){

    if(index != NULL && key != NULL){

        
        if(hashtable_find(index->ht, key) == NULL){
            counters_t* c = counters_new();
            counters_add(c, DocID);
            return hashtable_insert(index->ht, key, c);
        }
        
        counters_t* c = hashtable_find(index->ht, key);
        counters_add(c, DocID);
        return true;
        
    }
    return false;
}

int index_get(index_t* index, const char* key, int DocID){

    if(index != NULL && key != NULL){

        if(hashtable_find(index->ht, key) != NULL){
            counters_t* c = hashtable_find(index->ht, key);
            return counters_get(c, DocID);
        }
    }
    return 0;
}

void index_print(index_t* index, FILE *fp){
    if(index != NULL && fp != NULL){
        hashtable_print(index->ht, fp, index_print_helper);
    }
}

void index_delete(index_t* index){
    if(index != NULL){
        hashtable_delete(index->ht, index_delete_helper);
        mem_free(index);
    }
}

void index_save(index_t* index, FILE* fp){
    if(index != NULL && fp != NULL){
        hashtable_iterate(index->ht, fp, index_iterate_helper);
    }
}

void index_load(index_t* index, FILE* fp, void (*itemfunc)(index_t* index, char* line)){
    char* line;
    while((line = file_readLine(fp)) != NULL){
        if(itemfunc != NULL){
            itemfunc(index, line);
        }
        mem_free(line);
    }
    mem_free(line);
}

static void index_iterate_helper(void* arg, const char* key, void* item){
    fputs(key, arg);
    fputs(" ", arg);
    counters_iterate(item, arg, counters_iterate_helper);
    fputs("\n", arg);
}

static void counters_iterate_helper(void *arg, const int key, const int count){
    char keyc[10];
    char countc[10];
    sprintf(keyc, "%d", key);
    sprintf(countc, "%d", count);
    fputs(keyc, arg);
    fputs(" ", arg);
    fputs(countc, arg);
    fputs(" ", arg);
}

static void index_print_helper(FILE* fp, const char* key, void* item){
    const char *keyname = key;
    counters_t* ct = item;
    fprintf(fp, "%s", keyname);
    counters_print(ct, fp);
}

static void index_delete_helper(void* item){
    counters_t* ct = item;
    counters_delete(ct);
}

// int main(const int argc, const char* argv[]){
//     index_t* i = index_new(200);
//     if(i == NULL){
//         printf("Failed\n");
//     }
//     index_insert(i, "word", 1);
//     index_insert(i, "word", 2);
//     bool x = index_insert(i, "word", 1);
//     if(x){
//         printf("Success\n");
//     }
//     index_print(i, stdout);
//     return 0;
// }