//
//  bst.h
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#ifndef bst_h
#define bst_h

typedef void  (*bst_free_item)(void*);
typedef void  (*bst_print_item)(void*);
typedef void* (*bst_allocate_item)(const void*);
typedef int   (*bst_compare_key)(const void*, const void*);

typedef struct {
    bst_allocate_item item_allocater;
    bst_allocate_item key_allocater;
    bst_free_item free_item;
    bst_free_item free_key;
    bst_print_item print_item;
    bst_compare_key compare;
} bst_item_functions;

typedef struct tree_node {
    struct tree_node* left;
    struct tree_node* right;
    
    unsigned int size;
    
    void* key;
    void* data;
}tree_node;


typedef struct BST{
    tree_node* root;
    unsigned int size;
    
    void (*delete_bst)(struct BST* self);
    
    void (*insert)(struct BST* self, const void* k, const void* d);
    void (*remove)(struct BST* self, const void* k);
    void*  (*find)(struct BST* self, const void* k);
    int  (*empty)(struct BST* self);
    
    
    bst_allocate_item allocater;
    bst_allocate_item key_allocater;
    bst_free_item     free_item;
    bst_free_item     free_key;
    bst_print_item    print_item;
    bst_compare_key   compare;
    
}BST;

bst_item_functions BST_STORE;
BST* bst_create(bst_item_functions*);


#endif /* bst_h */
