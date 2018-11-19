//
//  cssysctrl.h
//  coroutine_scheduler
//
//  Created by 王路远 on 2018/11/18.
//  Copyright © 2018 王路远. All rights reserved.
//

#ifndef cssysctrl_H
#define cssysctrl_H

#include "cstype.h"


// Control the whole system
typedef struct {
    task_t *first_task; // the header ptr of the tasks list
    task_t *last_task; // the tail ptr of the tasks list
    task_t *runloop;
    task_t *running_task; // the current running task
    int last_id; // should only increase to make all task id unique
    
    int(*add_new_task_func)(task_t *);
} cs_system_controller;


void init_system_controller(void);

#endif /*cssysctrl_H*/
