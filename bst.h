//
//  bst.h
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#ifndef bst_h
#define bst_h

#include "util.h"

typedef struct cgs_tree_node {
    struct cgs_tree_node* left;
    struct cgs_tree_node* right;
    
    unsigned int size;
    
    void* key;
    void* data;
}cgs_tree_node;


typedef struct cgsBST{
    cgs_tree_node* root;
    unsigned int size;
    
    void (*delete_bst)(struct cgsBST* self);
    
    void (*insert)(struct cgsBST* self, const void* k, const void* d);
    void (*remove)(struct cgsBST* self, const void* k);
    void*  (*find)(struct cgsBST* self, const void* k);
    int   (*empty)(struct cgsBST* self);
    
    
    cgs_allocate_item allocater;
    cgs_allocate_item key_allocater;
    cgs_free_item     free_item;
    cgs_free_item     free_key;
    cgs_print_item    print_item;
    cgs_compare_key   compare;
    
}cgsBST;

cgs_item_functions CGS_BST_STORE;
cgsBST* bst_create(cgs_item_functions*);


#endif /* bst_h */
