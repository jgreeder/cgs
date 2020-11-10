//
//  stack.h
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#ifndef stack_h
#define stack_h

#include "util.h"

typedef struct cgsStack {
    void* storage;
    
    void  (*delete_stack) (struct cgsStack*);
    void  (*push) (struct cgsStack* self, const void* d);
    void  (*pop)  (struct cgsStack* self);
    int   (*empty)(struct cgsStack* self);
    void* (*top)  (struct cgsStack* self);
    
    cgs_insert_call s_push;
    cgs_remove_call s_pop;
    cgs_empty_call  s_empty;
    cgs_front_call  s_top;
    cgs_free_call   s_free;

}cgsStack;

cgsStack* stack_create(void*, cgs_function_calls*);


#endif /* stack_h */
