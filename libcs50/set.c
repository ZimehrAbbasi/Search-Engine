/* 
 * set.c - CS50 'set' module
 *
 * see set.h for more information.
 *
 * @Author Zimehr Abbasi
 * Module for creating a set data structure
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "mem.h"

// Set node struct
typedef struct setnode
{
    char *key;
    void *item;
    struct setnode *next;
} setnode_t;

// Set struct
typedef struct set
{
    struct setnode *head;
    int size;
} set_t;

// Function prototype for creating new setnode
static setnode_t *setnode_new(char *key, void *item);

// Creating a new set
set_t *
set_new(void)
{
    set_t *set = (set_t *)mem_malloc(sizeof(set_t));

    if (set == NULL)
    {
        return NULL; // error allocating set
    }
    else
    {
        // initialize contents of set structure
        set->head = NULL;
        set->size = 0;
        return set;
    }
}

// Inserting key:item pair into set
bool set_insert(set_t *set, const char *key, void *item)
{
    if (set != NULL && key != NULL && item != NULL)
    {

        char *keycpy = (char *)mem_malloc(strlen(key) * sizeof(char));
        strcpy(keycpy, key);

        setnode_t *temp;
        temp = set->head;
        while (temp != NULL)
        {
            if (strcmp(temp->key, keycpy) == 0)
            {
                free(keycpy);
                return false;
            }
            temp = temp->next;
        }
        // allocate a new node to be added to the list
        setnode_t *new = setnode_new(keycpy, item);

        if (new != NULL)
        {
            // add it to the head of the list
            new->next = set->head;
            set->head = new;
            set->size += 1;
            return true;
        }
    }

    return false;

#ifdef MEMTEST
    mem_report(stdout, "After set_insert");
#endif
}

// Finding item with corresponding key
void *set_find(set_t *set, const char *key)
{
    if (set != NULL && key != NULL)
    {
        setnode_t *temp;
        temp = set->head;
        while (temp != NULL)
        {
            if (strcmp(temp->key, key) == 0)
            {
                return (temp->item);
            }
            temp = temp->next;
        }
    }
    return NULL;
}

// Printing the set
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item))
{

    if (fp != NULL)
    {
        if (set != NULL)
        {
            fputc('{', fp);
            for (setnode_t *node = set->head; node != NULL; node = node->next)
            {
                // print this node
                if (itemprint != NULL)
                { // print the node's item
                    (*itemprint)(fp, node->key, node->item);
                    if (node->next != NULL)
                    {
                        fputc(',', fp);
                    }
                }
            }
            fputc('}', fp);
        }
        else
        {
            fputs("(null)", fp);
        }
        printf("\n");
    }
}

// Iterating through the set
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item))
{
    if (set != NULL && itemfunc != NULL)
    {
        // call itemfunc with arg, on each item
        for (setnode_t *node = set->head; node != NULL; node = node->next)
        {
            (*itemfunc)(arg, node->key, node->item);
        }
    }
}

// Deleting the set
void set_delete(set_t *set, void (*itemdelete)(void *item))
{
    if (set != NULL)
    {
        for (setnode_t *node = set->head; node != NULL;)
        {
            if (itemdelete != NULL)
            {                              // if possible...
                (*itemdelete)(node->item); // delete node's item
            }
            setnode_t *next = node->next; // remember what comes next
            char* key = node->key;
            mem_free(key);
            mem_free(node); // free the node
            node = next;    // and move on to next
        }

        mem_free(set);
        set = NULL;
    }
#ifdef MEMTEST
    mem_report(stdout, "End of set_delete");
#endif
}

static setnode_t * // not visible outside this file
setnode_new(char *key, void *item)
{
    setnode_t *node = mem_malloc(sizeof(setnode_t));

    if (node == NULL)
    {
        // error allocating memory for node; return error
        return NULL;
    }
    else
    {
        node->key = key;
        node->item = item;
        node->next = NULL;
        return node;
    }
}