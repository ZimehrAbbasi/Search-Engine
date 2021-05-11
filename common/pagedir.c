#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include <dirent.h>
#include <errno.h>

typedef struct webpage {
  char* url;
  char* html;
  size_t html_len;
  int depth;
} webpage_t;

bool pagedir_init(const char* pageDirectory){

  char path[260];
  sprintf(path, "%s/.crawler", pageDirectory);
  FILE* fp = fopen(path, "w+");
  if(fp == NULL){
    return false;
  }
  fclose(fp);
  return true;
}

char* pagedir_add(const char* pageDirectory, int fileNumber){

  char docid[10]; // Since the limit for the integer is 2147483647
  sprintf(docid, "%d", fileNumber);
  char* name = malloc(strlen(pageDirectory)+strlen(docid)+2);
  strcpy(name, pageDirectory);
  strcat(name, "/");
  strcat(name, docid);

  return name;

}

bool pagedir_val(const char* pageDirectory){
  DIR* dir = opendir(pageDirectory);
  if (dir) {
    closedir(dir);
    char name[strlen(pageDirectory)+strlen(".crawler")+1];
    strcpy(name, pageDirectory);
    strcat(name, "/");
    strcat(name, ".crawler");
    FILE* fp;

    if((fp = fopen(name, "r")) != NULL){
      fclose(fp);
      return true;
    }
  } 
  return false;
}

void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID){

  char docid[10]; // Since the limit for the integer is 2147483647
  sprintf(docid, "%d", docID);

  char name[strlen(pageDirectory)+strlen(docid)+1];
  strcpy(name, pageDirectory);
  strcat(name, "/");
  strcat(name, docid);

  FILE* fp;
  fp = fopen(name, "w");
  fprintf(fp, "%s\n", page->url);
  fprintf(fp, "%d\n", page->depth);
  fprintf(fp, "%s\n", page->html);
  fclose(fp);

}

webpage_t* pagedir_load(FILE* fp){
  char* url = file_readLine(fp);
  char* depths = file_readLine(fp);
  int depth = atoi(depths);
  mem_free(depths);
  char* html = file_readFile(fp);
  webpage_t* wp = webpage_new(url, depth, html);
  return wp;
}