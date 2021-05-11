#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// INITIATING A PAGE DIRECTORY TO SAVE THE HTML CODE FOR THE FOUND URLS
bool pagedir_init(const char* pageDirectory);

char* pagedir_add(const char* pageDirectory, int fileNumber);

webpage_t* pagedir_load(FILE* fp);

bool pagedir_val(const char* pageDirectory);

// SAVIING THE HTML TO THE INITIATED PAGE DIRECTORY
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

#endif // __PAGEDIR_H