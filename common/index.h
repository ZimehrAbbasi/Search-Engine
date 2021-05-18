#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/mem.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/hash.h"

#ifndef __INDEX_H
#define __INDEX_H

typedef struct index index_t;

index_t* index_new(const int num_slots);

bool index_insert(index_t* index, const char* key, int DocID);

int index_get(index_t* index, const char* key, int DocID);

void index_print(index_t* index, FILE *fp);

void index_delete(index_t* index);

void index_save(index_t* index, FILE* fp);

void index_load(index_t* index, FILE* fp, void (*itemfunc)(index_t* index, char* line));

#endif