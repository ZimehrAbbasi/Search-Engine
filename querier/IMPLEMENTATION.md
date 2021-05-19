# Implementation spec

## Index
* [Data structures](#Data-structures)
* [Control flow](#Control-flow)
* [Function prototypes](#Function-prototypes)
* [Major data structures](#Major-data-structures)
* [High-level pseudo code](#High-level-pseudo-code)
* [Testing plan](##Testing-plan,-including-unit-tests,-integration-tests,-system-tests)


## Data structures

We use 2 primary data structures: the index, which is a wrapper data structure for the hashtable, and the array data structure to hold the scores for each document. 

## Control flow

The querier.c is broken into 6 functions described below:

### main

The main parses the arguments, validates the files and directories, cpounts the number of files in the inputed directory, initializes the index, and ultimately calls `_querier`.

### parse

Parse takes in the `index` and the `line` and inserts the line into the index.

### pageDir_files

Takes in the `pageDir`, counts the number of files in it, and returns the integer.

### tokenize

Tokenize takes in the `token` array and the `query`, and inputs the words from the `query` into `token`.

```text=
for x in query length
    if query[x] == '\n':
        query[x] = '\0'
        break
    if query[x] == ' ':
        if the letter before this not a space
            increment the total number of words by 1 (wordnum++)
            set query[x] = '\0'
        
    else:
        if the letter before is a space:
            tokens[wordnum] = address of query[x]
```

### _querier

_querier takes in the `index`, the `pageDir` and the number of files in `pageDir`.

```text=

read query from stdin
normalize the query
tokenize the query
Print the normalized query
Create the document array[numOfFiles][numOfWordsinQuery] that stores the scores as a 2D array, of [docId][wordCount]
Check if the first or last element of the array are negative, i.e. 'and' 'or'
Check if consecutive words are (and, and), (and, or), (or, or), or (or, and)
Count the number of 'or's in the query
Create max score array for each document
Sort by score (in descending order), sort the index array using the same operations
display results
```

## Function prototypes

### _querier

```c=
void parse(index_t* index, char* line);
int pageDir_files(const char* pageDir);
void load_index(index_t* index, FILE* fp);
int tokenize(char* tokens[] , char* query);
void _querier(index_t* index, const char* pageDir, int i);
```

### index

```c=
index_t* index_new(const int num_slots);
bool index_insert(index_t* index, const char* key, int DocID);
int index_get(index_t* index, const char* key, int DocID);
void index_print(index_t* index, FILE *fp);
void index_delete(index_t* index);
void index_save(index_t* index, FILE* fp);
void index_load(index_t* index, FILE* fp, void (*itemfunc)(index_t* index, char* line));
```

### words

```c=
char* normalize_word(char* s);
```

### pagedir

```c=
bool pagedir_init(const char* pageDirectory);
char* pagedir_add(const char* pageDirectory, int fileNumber);
webpage_t* pagedir_load(FILE* fp);
bool pagedir_val(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
```

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

Most of the units have been tested before and the only one left to test will be the querier. The querier represents the whole system and is covered below.

### Integration Tests

To test the querier program we will test the program against an invalid page directory, invalid file, invalid numebr of arguments, unreadable files, unopenable files. Further, we will also test the program against invalid queries, such as:

```text=
and
or
and earth
or earth
planet earth or
planet earth and
planet earth and or science
planet earth and and science
planet earth or and science
```

### System Tests

To test the system we will run the program with valgrind to check if there are any memory leaks in the program. Further, we will check for any compilation errors from the compiler.