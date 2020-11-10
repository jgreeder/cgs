//
//  bst.c
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#include <stdlib.h>

#include "bst.h"

void fix_size(cgs_tree_node *T)
{
  T->size = 1;
  if (T->left) T->size += T->left->size;
  if (T->right) T->size += T->right->size;
}

static void free_node(cgs_tree_node* t, cgs_free_item i_f, cgs_free_item k_f) {
    if (!t) return;
    i_f(t->data);
    k_f(t->key);
    free(t);
}

static void recursive_delete_node(cgs_tree_node* t, cgs_free_item i_f, cgs_free_item k_f) {
    if (!t) return;
    recursive_delete_node(t->left, i_f, k_f);
    recursive_delete_node(t->right, i_f, k_f);
    free_node(t, i_f, k_f);
    t = NULL;
}

void split(cgs_tree_node *t, void* k, cgs_tree_node **l, cgs_tree_node **r, cgs_compare_key compare)
{
  if (t == NULL) {
    *l = NULL;
    *r = NULL;
    return;
  }
  if (compare(k, t->key) < 0) {
    split(t->left, k, l, &t->left, compare);
    *r = t;
  } else {
    split(t->right, k, &t->right, r, compare);
    *l = t;
  }
  fix_size(t);
}

static cgs_tree_node* insert_node(cgs_tree_node* t, cgs_tree_node* to_insert, cgs_compare_key compare) {
    if (!t) {
        t = to_insert;
        return t;
    }
    if (rand() % (t->size + 1) == 0) {
      // insert at root
      split(t, to_insert->key, &to_insert->left, &to_insert->right, compare);
      fix_size(to_insert);
      return to_insert;
    }
    
    if (compare(to_insert->key, t->key) < 0) t->left = insert_node(t->left, to_insert, compare);
    else t->right = insert_node(t->right, to_insert, compare);
    fix_size(t);
    return t;
}

static cgs_tree_node* join_node(cgs_tree_node* l, cgs_tree_node* r, cgs_compare_key compare) {
    if (l == NULL) return r;
    if (r == NULL) return l;
    int random = rand() % (l->size + r->size);
    
    if (random < l->size) {
      // L stays root
      l->right = join_node(l->right, r, compare);
      fix_size(l);
      return l;
    } else {
      // R stays root
      r->left = join_node(l, r->left, compare);
      fix_size(r);
      return r;
    }
}


static cgs_tree_node* remove_node(cgs_tree_node* t, const void* k, cgs_compare_key compare, cgs_free_item i_f, cgs_free_item k_f) {
    if (!t) return NULL;
    int c = compare(k, t->key);
    if (c < 0){
        t->left = remove_node(t->left, k, compare, i_f, k_f);
    } else if (c > 0) {
        t->right = remove_node(t->right, k, compare, i_f, k_f);
    } else {
        cgs_tree_node* temp = t;
        t = join_node(t->left, t->right, compare);
        free_node(temp, i_f, k_f);
        return t;
    }
    fix_size(t);
    return t;
}

static cgs_tree_node* find_node(cgs_tree_node* t, const void* k, cgs_compare_key compare) {
    if (!t) return NULL;
    int c = compare(k, t->key);
    if (c < 0){
        return find_node(t->left, k, compare);
    } else if (c > 0) {
        return find_node(t->right, k, compare);
    } else {
        return t;
    }
}


static void bst_insert(cgsBST* self, const void* k, const void* d) {
    cgs_tree_node* new_node = malloc(sizeof(cgs_tree_node));
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->size = 1;
    new_node->key = self->key_allocater(k);
    new_node->data = self->allocater(d);
    
    self->root = insert_node(self->root, new_node, self->compare);
    
    self->size = self->root->size;
}

static void bst_remove(cgsBST* self, const void* k) {
    if (!self->find(self, k)) return;
    
    self->root = remove_node(self->root, k, self->compare, self->free_item, self->free_key);
    self->size--;
}

static void* bst_find(cgsBST* self, const void* k) {
    if (self->size == 0) return NULL;
    cgs_tree_node* found = find_node(self->root, k, self->compare);
    if (found)
        return found->data;
    else
        return NULL;
}


static int bst_empty(cgsBST* self) {
    return (self->size == 0);
}

static void bst_delete(cgsBST* self)  {
    recursive_delete_node(self->root, self->free_item, self->free_key);
    free(self);
    self = NULL;
}

cgsBST* bst_create(cgs_item_functions* f) {
    cgsBST* b = malloc(sizeof(cgsBST));
    
    b->root = NULL;
    b->size = 0;
    
    b->delete_bst = bst_delete;
    b->insert = bst_insert;
    b->remove = bst_remove;
    b->find   = bst_find;
    b->empty  = bst_empty;
    
    b->allocater = f->item_allocater;
    b->key_allocater = f->key_allocater;
    b->free_item = f->free_item;
    b->free_key = f->free_key;
    b->compare = f->compare;
    
    return b;
}
