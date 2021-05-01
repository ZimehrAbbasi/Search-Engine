#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct webpage {
  char* url;
  char* html;
  size_t html_len;
  int depth;
} webpage_t;

bool pagedir_init(const char* pageDirectory){

  char tempPath[256];
  char *name;

  int i = 0;
  while(pageDirectory[i] != "/" || pageDirectory[i] != '\0'){
    
  }



  return !mkdir(pageDirectory, 0700);
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
  fprintf(fp, "%s\n", docid);
  fprintf(fp, "%s\n", page->url);
  fprintf(fp, "%s\n", page->html);
  fclose(fp);

}