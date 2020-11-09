//
//  hash.h
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#ifndef hash_h
#define hash_h

typedef void* (*ht_allocate_item)(const void*);
typedef void  (*ht_free_item)(void*);
typedef void  (*ht_print_item)(void*);

typedef struct {
    ht_allocate_item allocater;
    ht_free_item free_item;
    ht_print_item print_item;
} ht_item_functions;

typedef struct {
    char* key;
    void* value;
}h_item;


typedef struct HashTable{
    unsigned int size;
    unsigned int _count;
    h_item** table;

    // Delete table
    void  (*delete_table)(struct HashTable* self);

    // Main functions
    void  (*insert)(struct HashTable* self, const char* k, const void* v);
    void  (*update)(struct HashTable* self, const char* k, const void* v);
    void  (*remove)(struct HashTable* self, const char* k);
    void* (*find)  (struct HashTable* self, const char* k);
    int   (*count) (struct HashTable* self, const char* k);
    void  (*print) (struct HashTable* self);

    // Allow for generic values to be stored
    ht_allocate_item allocate_item;
    ht_free_item free_item;
    ht_print_item print_item;

} HashTable;

// For use when Hashtable itself is value
ht_item_functions HT_STORE;
// Init the hash table
HashTable* ht_create(ht_item_functions*);

#endif /* hash_h */
