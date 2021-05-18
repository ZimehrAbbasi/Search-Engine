/* 
 * hashtable.c - CS50 'hashtable' module
 *
 * see hashtable.h for more information.
 *
 * @Author Zimehr Abbasi
 * Module for creating a hashtable data structure
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "set.h"
#include "hash.h" 
#include "mem.h"

// Struct for hashtable
typedef struct hashtable{
    set_t** set;
    int size;
    int num_items;
} hashtable_t;

// Creating a new hashtable
hashtable_t* hashtable_new(const int num_slots){

    hashtable_t* hashtable = (hashtable_t*)mem_malloc(sizeof(hashtable_t));

    if (hashtable == NULL)
    {
        return NULL; // error allocating counter
    }
    else
    {
        hashtable->set = mem_calloc(num_slots, sizeof(set_t*));
        set_t** sets;
        sets = hashtable->set;

        for(int i = 0; i < num_slots; i++){
            sets[i] = set_new();
        }
        hashtable->size = num_slots;
        hashtable->num_items = 0;

        return hashtable;
    }
}

// Inserting key:item value into the hashtable
bool hashtable_insert(hashtable_t* ht, const char* key, void* item){
    if(ht != NULL && key != NULL && item != NULL){
        set_t** sets;
        sets = ht->set;

        if(hashtable_find(ht, key) != NULL){
            return false;
        }

        unsigned long index = hash_jenkins(key, ht->size);
        bool success = set_insert(sets[index], key, item);
        if(success){
            ht->num_items += 1;
            return true;
        }
    }
    return false;
}

// Finding the item associated with a a particular key
void* hashtable_find(hashtable_t* ht, const char* key){
    if(ht != NULL && key != NULL){
        set_t** sets;
        sets = ht->set;

        for(int i = 0; i < ht->size;i++){
            void *item = set_find(sets[i], key);
            if(item != NULL){
                return item;
            }
        }
    }

    return NULL;
}

// Finding the set associated with a particular key
void* hashtable_insertat(hashtable_t* ht, const char* key, void* item, int num){
    if(ht != NULL){
        set_t** sets;
        sets = ht->set;

        unsigned long index = num;
        bool success = set_insert(sets[index], key, item);
        if(success){
            ht->num_items += 1;
            return true;
        }
    }

    return NULL;
}

// Printing the hashtable
void hashtable_print(hashtable_t* ht, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item)){
    if(fp != NULL){
        if(ht != NULL){
            set_t** sets;
            sets = ht->set;
            for(int i = 0; i < ht->size;i++){
                if(itemprint == NULL){
                    fputs("{}\n", fp);
                    continue;
                }
                set_print(sets[i], fp, itemprint);
            }
        }else{
            fputs("(null)", fp);
        }
    }
}

// Iterating through the hashtable
void hashtable_iterate(hashtable_t* ht, void* arg, void (*itemfunc)(void* arg, const char* key, void* item)){
    if(ht != NULL && itemfunc != NULL){
        set_t** sets;
        sets = ht->set;
        for(int i = 0; i < ht->size;i++){
            set_iterate(sets[i], arg, itemfunc);
        }
    }
}

// Deleting the hashtable
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) ){
    if (ht != NULL)
    {
        set_t** sets;
        sets = ht->set;
        for(int i = 0; i < ht->size;i++){
            set_delete(sets[i], itemdelete);
        }
        free(sets);
        free(ht);
    }
}