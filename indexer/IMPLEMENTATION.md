# CS50 Lab 5
## CS50 Spring 2021

### indexer

```c
index_t* index_new(const int num_slots);
bool index_insert(index_t* index, const char* key, int DocID);
void index_print(index_t* index, FILE *fp);
void index_delete(index_t* index);
void index_save(index_t* index, FILE* fp);
```

We implement the index as a wrapper for the hashtable.
The *index* itself is represented as a `struct index` containing a pointer to the hashtable stored in memory.

To insert a new item in the index we create a new counter to hold the `docID` and the `count`, and insert into an available slot.

The `index_print` method prints a little syntax around the index, and between items, but mostly calls the `itemprint` function on each item by scanning the hashtable and counters.

The `index_save` method calls the `itemfunc` function on each item by scanning the hashtable and counters for each key, and print them out to a list.

The `index_delete` method deletes the hashtable and the counters