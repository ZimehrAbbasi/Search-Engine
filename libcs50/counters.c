/* 
 * counters.c - CS50 'counters' module
 *
 * see counters.h for more information.
 *
 * @Author Zimehr Abbasi
 * Module for creating a counter data structure
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "mem.h"

// Counters node struct
typedef struct counternode
{
    int key;
    int count;
    struct counternode *next;
} counternode_t;

// Counters struct
typedef struct counters
{
    struct counternode *head;
    int size;
} counters_t;

// Function prototype to create new counternode
static counternode_t *counternode_new(const int key);

// Creating a new counter
counters_t *
counters_new(void)
{
    counters_t *counter = (counters_t *)mem_malloc(sizeof(counters_t));

    if (counter == NULL)
    {
        return NULL; // error allocating counter
    }
    else
    {
        // initialize contents of counter structure
        counter->head = NULL;
        counter->size = 0;
        return counter;
    }
}

// Adding a key to the counter or incrementing predefinned key
int counters_add(counters_t *ctrs, const int key)
{
    if (ctrs != NULL && key >= 0)
    {
        int val = counters_get(ctrs, key);
        if (val != 0)
        {
            return val;
        }

        counternode_t *new = counternode_new(key);

        if (new != NULL)
        {
            // add it to the head of the list
            new->next = ctrs->head;
            ctrs->head = new;
            ctrs->size += 1;
            return new->count;
        }
    }
    return 0;
}

// Getting count for specific key
int counters_get(counters_t *ctrs, const int key)
{
    if (ctrs != NULL && key >= 0)
    {
        counternode_t *temp;
        temp = ctrs->head;
        while (temp != NULL)
        {
            if (temp->key == key)
            {
                temp->count += 1;
                return temp->count;
            }
            temp = temp->next;
        }
    }

    return 0;
}

// Set count for specific counter
bool counters_set(counters_t *ctrs, const int key, const int count)
{
    if (ctrs != NULL && key >= 0 && count >= 0)
    {
        counternode_t *temp;
        temp = ctrs->head;
        while (temp != NULL)
        {
            if (temp->key == key)
            {
                temp->count = count;
                return temp->count;
            }
            temp = temp->next;
        }

        counters_add(ctrs, key);
        return counters_set(ctrs, key, count);
    }

    return 0;
}

// Print value of counter
void counters_print(counters_t *ctrs, FILE *fp)
{
    if (fp != NULL)
    {
        if (ctrs != NULL)
        {
            for (counternode_t *node = ctrs->head; node != NULL; node = node->next)
            {
                fputc('{', fp);
                fprintf(fp, "%d=%d", node->key, node->count);
                fputc('}', fp);
                if (node->next != NULL)
                {
                    fputc(',', fp);
                }
            }
        }
        else
        {
            fputs("(null)\n", fp);
        }
    }
}

// Iterating through the counter
void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, const int count))
{
    if (ctrs != NULL && itemfunc != NULL)
    {
        // call itemfunc with arg, on each item
        for (counternode_t *node = ctrs->head; node != NULL; node = node->next)
        {
            (*itemfunc)(arg, node->key, node->count);
        }
    }
}

// Deleting counter
void counters_delete(counters_t *ctrs)
{
    if (ctrs != NULL)
    {
        for (counternode_t *node = ctrs->head; node != NULL;)
        {
            counternode_t *next = node->next; // remember what comes next
            mem_free(node);                   // free the node
            node = next;                      // and move on to next
        }
        mem_free(ctrs);
    }
}

static counternode_t * // not visible outside this file
counternode_new(const int key)
{
    counternode_t *node = mem_malloc(sizeof(counternode_t));

    if (node == NULL)
    {
        // error allocating memory for node; return error
        return NULL;
    }
    else
    {
        node->key = key;
        node->count = 1;
        node->next = NULL;
        return node;
    }
}