//
//  hash.h
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#ifndef hash_h
#define hash_h

#include "util.h"

typedef struct {
    char* key;
    void* value;
}cgs_h_item;


typedef struct cgsHashTable{
    unsigned int size;
    unsigned int _count;
    cgs_h_item** table;

    // Delete table
    void  (*delete_table)(struct cgsHashTable* self);

    // Main functions
    void  (*insert)(struct cgsHashTable* self, const char* k, const void* v);
    void  (*update)(struct cgsHashTable* self, const char* k, const void* v);
    void  (*remove)(struct cgsHashTable* self, const char* k);
    void* (*find)  (struct cgsHashTable* self, const char* k);
    int   (*count) (struct cgsHashTable* self, const char* k);

    // Allow for generic values to be stored
    cgs_allocate_item allocate_item;
    cgs_free_item free_item;
} cgsHashTable;

// For use when Hashtable itself is value
cgs_item_functions HT_STORE;
// Init the hash table
cgsHashTable* ht_create(cgs_item_functions*);

#endif /* hash_h */
