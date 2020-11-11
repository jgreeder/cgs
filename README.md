#  C General Data Structures (CGS)

A (hopefully) useful library that implements some elementary 
data structures in a generic way for the C programming language. 
Any data type can be stored in these data structures, but it is up to the
caller to keep track of what data type is stored. 

## Headers
	list.h
	hash.h
	bst.h
	stack.h
	queue.h
	pqueue.h
	util.h

## Foreword
The basis of all these structures rely on the `cgs_item_functions` struct in `util.h` (you don't directly have to include it). 
Most data structures only make use of:

* `item_allocater`
* `free_item`
* `compare`

But `key_allocater` and `free_key` are used for some (i.e. BST).

For some data structures that can be implemented with different base structures 
(i.e. stack, queue) the struct `cgs_function_calls` is defined so that any base
data structure can be used as long as it provides those functions. All data
structures contain function pointers to there member functions.
For example, `list->remove_front(list)`. Therefore you need to pass the data structure as the first param.

All data structures make explicit copys of the data given when inserting, which is why an allocation function must be specified, along with a function that frees the allocated data.

When retrieving an item from any data structure it is `undefined behaviour` to free them directly. Use the supplied remove function for freeing the data.

### cgs\_item_functions
* `void  (*cgs_free_item)(void*);`
* `void  (*cgs_print_item)(void*);`
* `void* (*cgs_allocate_item)(const void*);`
* `int   (*cgs_compare_key)(const void*, const void*);`


## Linked List

### Methods

* `cgsList* cgs_list_create(cgs_item_functions*)`
* `void insert_front(cgsList* self, const void* d)`
* `void insert_at(cgsList* self, const void* d, const int i)`
* `void insert_priority(cgsList* self, const void* d)`
* `void insert_back(cgsList* self, const void* d)`
* `void remove_front(cgsList* self)`
* `void remove_back(cgsList* self)`
* `int  empty(cgsList* self)`
* `void* front(cgsList* self)`
* `void* at(cgsList* self, const int i)`
* `void* back(cgsList* self)`
* `void  delete_list(cgsList* self)`

### Usage
You must supply at least a `cgs_allocate_item` function and a `cgs_free_item` function
in order to use. Optionally, you can also specify a `cgs_compare_key` to be able to do priority inserts.

To store nesting Lists use the predefined variable `CGS_LST_STORE`. This contains the neccessary
function pointers to give to the `cgs_list_create` function. (A default comparator is given that compares
list sizes. Feel free to change).

### Example
	cgsList list = cgs_list_create(&(cgs_item_functions){<item_allocater>, <free_item>, <compare>})
	list->insert_front(list, <pointer/address to item>


## Hash Table

### Methods

* `cgsHashTable* cgs_ht_create(cgs_item_functions*)`
* `void  cgs_delete_table(HashTable*)`
* `void  (*insert)(cgsHashTable* self, const char* k, const void* v)`
* `void  (*update)(cgsHashTable* self, const char* k, const void* v)`
* `void  (*remove)(cgsHashTable* self, const char* k)`
* `void* (*find)  (cgsHashTable* self, const char* k)`
* `int   (*count) (cgsHashTable* self, const char* k)`

### Usage
Currently, only `char*` keys are accepted. But any data type can by used 
for the value. (Even a hash table!).

Only a `cgs_allocate_item` function and a `cgs_free_item` function are required
to allocate and free the data type stored.

In order to have a hash table that stores hash tables you can use the
predefined `CGS_HT_STORE` variable as the input to `ht_create`. One benefit of using `void*` is that a hashtable of hashtables doesn't have to store the same
hashtable 

### Example
	cgsHashTable* ht = cgs_ht_create(&CGS_HT_STORE); // Hashtable of hashtables
	ht->insert(ht, "Key 1", &(cgs_item_functions){<item_allocater>, <free_item>});
	ht->insert(ht, "Key 2", &(cgs_item_functions){<item_allocater2>, <free_item2>});
	cgsHashTable* inner = ht->find(ht, "Key 1");
	inner->insert(inner, "Inner key", &<item to insert>);
	

## BST

### Methods

### Usage

## Stack

### Methods

* `cgsStack* stack_create(void* ds, cgs_function_calls* calls);`
* `void  delete_stack(struct cgsStack* self);`
* `void  push (struct cgsStack* self, const void* d);`
* `void  pop  (struct cgsStack* self);`
* `void* top  (struct cgsStack* self);`
* `int   empty(struct cgsStack* self);`

### Usage

The stack data structure is implemented by using another data structure. The only requirement that
must be met is providing `cgs_function_calls` that contains the function pointers to respective functions.
For example, if a list is used, the insert call would be set to insert\_front/insert\_back, etc.

To use a cgsList as the base container, use the predefined `CGS_LIST_STACK` variable as the second parameter

### Example
	cgsStack* stack = cgs_stack_create(cgs_list_create(&(cgs_item_functions){<item_allocater>...}), 
								   CGS_LIST_STACK);
	stack->push(stack, &<item>);
	TYPE* item = stack->top(stack);
	stack->pop(stack);
	

## Queue

### Methods

* `cgsQueue* queue_create(void*, cgs_function_calls*);`
* `void  delete_queue (struct cgsQueue* self);`
* `void  enqueue(struct cgsQueue* self, const void* d);`
* `void  dequeue(struct cgsQueue* self);`
* `int   empty(struct cgsQueue* self);`
* `void* front(struct cgsQueue* self);`

### Usage

The queue data structure is implemented by using another data structure. The only requirement that must be met is providing `cgs_function_calls` that contains the function pointers to respective functions.
For example, if a list is used, the insert call would be set to insert\_front/insert\_back, etc.

To use a cgsList as the base container, use the predefined `CGS_LIST_QUEUE` variable as the second parameter. The first parameter should be a pointer to the data structure.

### Example
	cgsQueuee* queue = cgs_queue_create(cgs_list_create(&(cgs_item_functions){<item_allocater>...}), 
								   CGS_LIST_QUEUE);
	queue->enqueue(stack, &<item>);
	TYPE* item = stack->front(stack);
	stack->dequeue(stack);
