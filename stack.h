//
//  stack.h
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#ifndef stack_h
#define stack_h

typedef void  (*stack_push_call)(void* self, const void*);
typedef void  (*stack_pop_call)(void* self);
typedef int   (*stack_empty_call)(void* self);
typedef void* (*stack_top_call)(void* self);
typedef void  (*stack_free_call)(void* self);


typedef struct {
    stack_push_call push;
    stack_pop_call  pop;
    stack_empty_call empty;
    stack_top_call top;
    stack_free_call delete_storage;
}stack_function_calls;

typedef struct Stack {
    void* storage;
    
    void  (*delete_stack) (struct Stack*);
    void  (*push) (struct Stack* self, const void* d);
    void  (*pop)  (struct Stack* self);
    int   (*empty)(struct Stack* self);
    void* (*top)  (struct Stack* self);
    
    stack_push_call s_push;
    stack_pop_call  s_pop;
    stack_empty_call s_empty;
    stack_top_call  s_top;
    stack_free_call s_free;

}Stack;

Stack* stack_create(void*, stack_function_calls*);


#endif /* stack_h */
