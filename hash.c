//
//  hash.c
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "hash.h"

#define HT_PRIME_1 379
#define HT_PRIME_2 443
#define HT_DEFAULT_SIZE 51
#define HT_RESIZE_RATIO 2
#define HT_RESIZE_UP_PERCENT   0.75
#define HT_RESIZE_DOWN_PERCENT 0.10

static h_item HT_DELETED_ITEM = {NULL, NULL};

// helper functions for hash table
static h_item* new_item(const char* k, const void* v, ht_allocate_item);
static void del_item(h_item* item, ht_free_item);
static int ht_hash    (const char* s, const int a, const int m);
static int ht_get_hash(const char* s, const int m, const int t);
static void ht_resize(HashTable* ht, int new_size);

// Main functions
static void   ht_delete_table(HashTable* self);
static void   ht_insert(HashTable* self, const char* k, const void* v);
static void   ht_update(HashTable* self, const char* k, const void* v);
static void   ht_delete(HashTable* self, const char* k);
static int    ht_count(HashTable* self, const char* k);
static void*  ht_find(HashTable* self, const char* k);

// For use when Hashtable itself is value
static void* ht_allocater(const void*);
static void ht_free(void *);
static void ht_print_open(void* self);
ht_item_functions HT_STORE = {ht_allocater, ht_free, ht_print_open};


static h_item* new_item(const char* k, const void* v, ht_allocate_item allocate_item) {
    h_item* n = malloc(sizeof(h_item));
    n->key = strdup(k);
    n->value = allocate_item(v);;
    return n;
}

static void del_item(h_item* item, ht_free_item free_item) {
    free(item->key);
    free_item(item->value);
    free(item);
}

static int ht_hash(const char* s, const int a, const int m) {
    long hash = 0;
    const int len = (int)strlen(s);
    for (int i = 0; i < len; i++) {
        hash += (long)pow(a, len - i+1)*(s[i]);
        hash = hash % m;
    }
    return (int)hash;
}

static int ht_get_hash(const char* s, const int m, const int t) {
    const int hash_a = ht_hash(s, HT_PRIME_1, m);
    const int hash_b = ht_hash(s, HT_PRIME_2, m);
    return (hash_a + (t * (hash_b + 1))) % m;
}

static void ht_delete_table(HashTable* self) {
    for (int i=0; i < self->size; i++) {
        h_item* item = self->table[i];
        if(item != NULL && item != &HT_DELETED_ITEM) del_item(item, self->free_item);
        else if (item != &HT_DELETED_ITEM) free(item);
    }
    free(self->table);
    free(self);
    self = NULL;
}

static int is_prime(const int x) {
    if (x < 2) { return -1; }
    if (x < 4) { return 1; }
    if ((x % 2) == 0) { return 0; }
    for (int i = 3; i <= floor(sqrt((double) x)); i += 2) {
        if ((x % i) == 0) {
            return 0;
        }
    }
    return 1;
}

static int next_prime(int x) {
    while (is_prime(x) != 1) {
        x++;
    }
    return x;
}

static void ht_resize(HashTable* ht, int new_size) {
    if (new_size < HT_DEFAULT_SIZE) {
        return;
    }

    int prime = next_prime(new_size);

    h_item** new_table = calloc((size_t)prime, sizeof(h_item*));
    for (int i = 0; i < ht->size; i++) {
        h_item* old = ht->table[i];

        if (old && old != &HT_DELETED_ITEM){
            int hash = ht_get_hash(old->key, prime, 0);
            h_item* new_curr = new_table[hash];
            int try = 1;
            while (new_curr != NULL) {
                hash = ht_get_hash(old->key, prime, try);
                new_curr = new_table[hash];
                try++;
            }
            new_table[hash] = new_item(old->key, old->value, ht->allocate_item);
            del_item(old, ht->free_item);
        } else if (old == &HT_DELETED_ITEM){
            free(ht->table[i]);
        }
    }

    ht->size = prime;
    free(ht->table);
    ht->table = new_table;
}

static void ht_insert(HashTable* self, const char* k, const void* v) {
    if (self->count(self, k)) {
        self->update(self, k, v);
        return;
    }

    if (self->_count+1 >= self->size*HT_RESIZE_UP_PERCENT)
        ht_resize(self, self->size*HT_RESIZE_RATIO);

    int hash = ht_get_hash(k, self->size, 0);

    h_item* curr = self->table[hash];
    int try = 1;
    while (curr != NULL && curr != &HT_DELETED_ITEM) {
        hash = ht_get_hash(k, self->size, try);
        curr = self->table[hash];
        try++;
    }

    self->table[hash] = new_item(k, v, self->allocate_item);
    self->_count++;
}

static void ht_delete(HashTable* self, const char* k) {
    int hash = ht_get_hash(k, self->size, 0);

    h_item* curr = self->table[hash];
    int try = 1;
    while (curr != NULL){
        if (curr != &HT_DELETED_ITEM) {
            if(strcmp(curr->key, k) == 0){
                del_item(curr, self->free_item);
                self->table[hash] = &HT_DELETED_ITEM;
                self->_count--;
                break;
            }
        }
        hash = ht_get_hash(k, self->size, try);
        curr = self->table[hash];
        try++;
    }

    if (self->_count <= self->size*HT_RESIZE_DOWN_PERCENT)
         ht_resize(self, (int)self->size/HT_RESIZE_RATIO);
}

static void*  ht_find(HashTable* self, const char* k) {
    int hash = ht_get_hash(k, self->size, 0);

    h_item* curr = self->table[hash];
    int try = 1;
    while (curr != NULL && curr != &HT_DELETED_ITEM && strcmp(curr->key, k) != 0) {

        hash = ht_get_hash(k, self->size, try);
        curr = self->table[hash];
        try++;
    }
    if (curr && curr != &HT_DELETED_ITEM && strcmp(curr->key, k)==0)
        return curr->value;
    else
        return NULL;
}

static int ht_count(HashTable* self, const char* k) {
    int hash = ht_get_hash(k, self->size, 0);

    h_item* curr = self->table[hash];
    int try = 1;
    while (curr != NULL && curr != &HT_DELETED_ITEM && strcmp(curr->key, k) != 0) {
        hash = ht_get_hash(k, self->size, try);
        curr = self->table[hash];
        try++;
    }
    if (curr && strcmp(curr->key, k)==0) return 1;

    return 0;
}

static void ht_update(HashTable* self, const char* k, const void* v) {
    int hash = ht_get_hash(k, self->size, 0);

    if (!self->count(self, k)) return;

    h_item* curr = self->table[hash];
    int try = 1;
    while (curr != NULL && curr != &HT_DELETED_ITEM && strcmp(curr->key, k) != 0) {

        hash = ht_get_hash(k, self->size, try);
        curr = self->table[hash];
        try++;
    }
    if (curr && strcmp(curr->key, k)==0) {
        self->free_item(curr->value);
        curr->value = self->allocate_item(v);
    }

}

static void ht_print(HashTable* self) {
    for (int i = 0; i < self->size; i++) {
        if (!self->table[i]) continue;
        printf("Key: %s\t", self->table[i]->key);
        self->print_item(self->table[i]->value);
    }
}

HashTable* ht_create(ht_item_functions* f) {

    HashTable* ht = malloc(sizeof(HashTable));

    // init memember vars
    ht->_count = 0;
    ht->size = HT_DEFAULT_SIZE;

    // init memember functions
    ht->find   = ht_find;
    ht->count  = ht_count;
    ht->remove = ht_delete;
    ht->insert = ht_insert;
    ht->update = ht_update;
    ht->delete_table = ht_delete_table;
    ht->allocate_item = f->allocater;
    ht->free_item = f->free_item;
    ht->print_item = f->print_item;
    ht->print = ht_print;


    // init the table
    ht->table = calloc((size_t)ht->size, sizeof(h_item*));

    return ht;
}


static void* ht_allocater(const void* ht) {
    const ht_item_functions* f = ht;
    return ht_create(&(ht_item_functions){f->allocater, f->free_item, f->print_item});
}

static void ht_free(void * ht) {
    ht_delete_table(ht);
}

static void ht_print_open(void* t) {
    HashTable* self = t;
    for (int i = 0; i < self->size; i++) {
        if (!self->table[i]) continue;
        printf("Key: %s\t", self->table[i]->key);
        self->print_item(self->table[i]->value);
    }
}
