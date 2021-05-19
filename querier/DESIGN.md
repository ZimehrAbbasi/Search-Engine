# Design spec

## Index
* [User Interface](#User-interface)
* [Inputs and outputs](#Inputs-an-outputs)
* [Functional decomposition into functions/modules](#Functional-decomposition-int-functions/modules)
* [High-level pseudo code](#High-level-pseudo-code)
* [Testing plan](##Testing-plan,-including-unit-tests,-integration-tests,-system-tests)

## User interface

As described in the Requirements Spec, the querier's only interface with the user is on the command-line; it must always have two arguments:

* **pageDir:** A crawler created directory in which all the documents are stored.
* **indexFile:** A file containing the text version of the index
```bash=
$ ./querier [pageDir] [indexFile]
```
#### Example Run
```bash=
./querier ../data/letters ../data/letters.index
```

## Inputs and outputs

_Input:_ Command line parameters are described above. Other inputs include the query to be processed which will be read from stdin.

_Output:_ As per the Requirements spec, the program will output the score, document Id, and the URL of the document if the query matches any permutations of words found in the files.
```bash=
Score: 3 Document: 7 URL: https://example.com/
```

## Functional decomposition into functions/modules
* main: Parses arguments and begins the querier with given parameters
* parse: Reads a line in the index file and inserts it into the index
* load index: converts the index file into an index object
* tokenize: create an array of words from the query
* _querier: Main function of the querier program which takes in a request and outputs the search results

## High-level pseudo code

```text
parse the command line, validate parameters, initialize other modules
read index file into index
check number of files in pageDirectory
read query from stdin
normalize query
tokanize query (split into words)
assign weights to query (0 for normal words, 1 for 'and', 2 for 'or')
create document array
check for invalid inputs
squish scores of 'and' and the rest of the words into 1 number and create an orArray
create score array from the orArray
sort index
outout the top n search results
```

## Testing plan, including unit tests, integration tests, system tests.

### Unit Tests 
* Testing the parse function
    * Test writes file's line into the parse function and reads the resulting index
* Testing the load index function
    * Test inputs an entire idnex file and prints resulting index

### Integration Tests
* Testing the querier
    * Test the querier function with the parse function as well as the load index function

### System Tests
* Testing if there are any memory leaks using valgrind.
    * Test modules separately. Make sure each module's data constructors and processors do not leak memory.
    * Test modules together. Make sure collective functionality does not leak memory.
* Testing for compiler warnings.
    * Run make clean and make all to test for compilation warnings / errors