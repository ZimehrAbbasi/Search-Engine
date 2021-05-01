#include <stdlib.h>
#include <stdio.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../libcs50/mem.h"
#include <string.h>

typedef struct page {
    char* url;
    int depth;
} page_t;

// FUNCTION PROTOTYPES
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static page_t* new_page_t(char* url, int depth);
static void page_delete(page_t* page);
static void logr(const char *word, const int depth, const char *url);

static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth){

    if(argc == 4){
        *seedURL = argv[1];
        *pageDirectory = argv[2];
        *maxDepth = atoi(argv[3]);
        char* url = normalizeURL(*seedURL);
        printf("Checking if URL is internal...\n");
        sleep(1);
        if(isInternalURL(url)){
            printf("Creating page directory...\n");
            sleep(1);
            if(pagedir_init(*pageDirectory)){
                printf("Checking if depth is in bounds...\n");
                sleep(1);
                if(*maxDepth <= 10 && *maxDepth >= 0){
                    printf("Success!\n");
                    mem_free(url);
                    return;
                }else{
                    fprintf(stderr, "Max Depth out of range.\n");
                }
                
            }else{
                fprintf(stderr, "Directory creation failed.\n");
            }
        }else{
            fprintf(stderr, "Url not internal.\n");
        }
        mem_free(url);
    }else{
        fprintf(stderr, "Argument count error.\n");
    }
    exit(1);

}

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen){

    int pos = 0;
    char* url;
    while ((url = webpage_getNextURL(page, &pos)) != NULL) {
        logr("Found", webpage_getDepth(page), url);
        if(isInternalURL(url)){
            if(hashtable_insert(pagesSeen, url, url)){
                page_t* p = new_page_t(url, webpage_getDepth(page)+1);
                bag_insert(pagesToCrawl, p);
                logr("Added", webpage_getDepth(page), url);
            }else{
                logr("IgnDupl", webpage_getDepth(page), url);
            }
        }else{
            logr("IgnExtrn", webpage_getDepth(page), url);
        }
        mem_free(url);
    }
}

static void crawl(char* seedURL, char* pageDirectory, const int maxDepth){
    hashtable_t* ht = hashtable_new(200);
    bag_t* bag = bag_new();
    page_t* page = new_page_t(seedURL, 0);
    hashtable_insert(ht, seedURL, seedURL);
    bag_insert(bag, page);
    page_t* temp;
    int DocId = 0;
    while((temp = bag_extract(bag)) != NULL){
        webpage_t* wp;
        wp = webpage_new(temp->url, temp->depth, NULL);
        if(webpage_fetch(wp)){
            pagedir_save(wp, pageDirectory, DocId);
            logr("Fetched", temp->depth, temp->url);
            if(webpage_getDepth(wp) < maxDepth){
                logr("Scanning", temp->depth, temp->url);
                pageScan(wp, bag, ht);
            }
        }
        page_delete(temp);
        webpage_delete(wp);
        DocId++;
    }
    
    hashtable_delete(ht, NULL);
    bag_delete(bag, NULL);
}

static page_t* new_page_t(char* url, int depth){

    if(url != NULL){
        char* urlcpy = (char*)mem_malloc(sizeof(char)*(strlen(url)+1));
        strcpy(urlcpy, url);
        page_t* page = (page_t*)mem_malloc(sizeof(page_t));
        page->url = urlcpy;
        page->depth = depth;
        return page;
    }
    return NULL;
}

static void page_delete(page_t* page){
    if(page != NULL){
        mem_free(page);
    }
}

static void logr(const char *word, const int depth, const char *url)
{
    printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}

int main(const int argc, char* argv[]){

    char** seedURL = (char **)malloc(sizeof(char*));
    char** pageDirectory = (char **)malloc(sizeof(char*));
    int* maxDepth = (int *)malloc(sizeof(int));
    parseArgs(argc, argv, seedURL, pageDirectory, maxDepth);
    crawl(*seedURL, *pageDirectory, *maxDepth);
    printf("\n");
    mem_free(seedURL);
    mem_free(pageDirectory);
    mem_free(maxDepth);
    return 0;

}