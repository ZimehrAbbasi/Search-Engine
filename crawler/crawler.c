#include <stdlib.h>
#include <stdio.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../libcs50/mem.h"
#include <string.h>

// STRUCTS
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

// FUNCTION TO PARSE THE ARGUMENTS AND SET UP THE VARIABLES TO BEGIN THE CRAWLING
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth){

    // Checks argument count
    if(argc == 4){
        *seedURL = argv[1];
        *pageDirectory = argv[2];
        *maxDepth = atoi(argv[3]);
        char* url = normalizeURL(*seedURL); // Get the normalized url
        printf("Checking if URL is internal...\n");
        sleep(1);
        if(isInternalURL(url)){ // Check if normalized url is internal
            printf("Creating page directory...\n");
            sleep(1);
            if(pagedir_init(*pageDirectory)){ // Create the page directory
                printf("Checking if depth is in bounds...\n");
                sleep(1);
                if(*maxDepth <= 10 && *maxDepth >= 0){ // Check if the maximum depth is out of range
                    printf("Success!\n");
                    mem_free(url); // free the url
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

// METHOD TO SCAN THE PAGE FOR ADDITIONAL URLS
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen){

    int pos = 0;
    char* url;
    while ((url = webpage_getNextURL(page, &pos)) != NULL) {
        char* normurl = normalizeURL(url); // Get normalized url
        logr("Found", webpage_getDepth(page), normurl);
        if(isInternalURL(normurl)){ // Check if it is internal
            if(hashtable_insert(pagesSeen, normurl, normurl)){
                page_t* p = new_page_t(normurl, webpage_getDepth(page)+1); // Create new page for webpage
                bag_insert(pagesToCrawl, p); // Insert the page into the bag
                logr("Added", webpage_getDepth(page), normurl);
            }else{
                logr("IgnDupl", webpage_getDepth(page), normurl);
            }
        }else{
            logr("IgnExtrn", webpage_getDepth(page), normurl);
        }
        mem_free(url);
        mem_free(normurl);
    }
}

// ITERATING OVER THE URLS, FETHCING THEIR HTML, AND THE SUBSEQUENT URLS FOUNF IN THIR HTML CODE
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth){
    hashtable_t* ht = hashtable_new(200); //  Create the hashtable
    bag_t* bag = bag_new(); //  Create the bag to store the pages
    page_t* page = new_page_t(seedURL, 0); // Seed page
    hashtable_insert(ht, seedURL, seedURL);
    bag_insert(bag, page);
    page_t* temp;
    int DocId = 0;
    while((temp = bag_extract(bag)) != NULL){
        webpage_t* wp;
        wp = webpage_new(temp->url, temp->depth, NULL); // Create a new webpage from the page
        if(webpage_fetch(wp)){ // Fetch the html for the webpage
            pagedir_save(wp, pageDirectory, DocId); // Save the html to files
            logr("Fetched", temp->depth, temp->url);
            if(webpage_getDepth(wp) < maxDepth){ // Check its depth
                logr("Scanning", temp->depth, temp->url);
                pageScan(wp, bag, ht); // Scan the page for additional urls
            }
        }
        page_delete(temp);
        webpage_delete(wp);
        DocId++;
    }
    
    hashtable_delete(ht, NULL);
    bag_delete(bag, NULL);
}

// Method to create new pages
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
    printf("\n####################################################################\n\n");
    printf("Precrawl checks:\n");
    parseArgs(argc, argv, seedURL, pageDirectory, maxDepth);
    printf("\n--------------------------------------------------------------------\n");
    printf("\nSeed URL: %3s\nPageDir: %3s\nMax Depth: %d\n", *seedURL, *pageDirectory, *maxDepth);
    printf("\n####################################################################\n\n");
    crawl(*seedURL, *pageDirectory, *maxDepth);
    printf("\n");
    mem_free(seedURL);
    mem_free(pageDirectory);
    mem_free(maxDepth);
    return 0;

}