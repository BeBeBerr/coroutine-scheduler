//
//  cssysctrl.c
//  coroutine_scheduler
//
//  Created by 王路远 on 2018/11/18.
//  Copyright © 2018 王路远. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "cssysctrl.h"
#include "runloop.h"

cs_system_controller g_cs_system_controller; // should be singleton

// add a new task to the system
// return new task id
int add_new_task(task_t *task_ptr) {
    g_cs_system_controller.last_id += 1;
    
    if (g_cs_system_controller.first_task == NULL) {
        // no task yet
        g_cs_system_controller.first_task = task_ptr;
        g_cs_system_controller.last_task = task_ptr;
    } else {
        g_cs_system_controller.last_task->next_task = task_ptr;
        g_cs_system_controller.last_task = task_ptr; // set the new tail
    }
    
    return g_cs_system_controller.last_id;
}

// return NULL if task not found
task_t *get_task_by_id(int tid) {
    task_t *tmp = g_cs_system_controller.first_task;
    task_t *result = NULL;
    while (tmp != NULL) {
        if (tmp->tid == tid) {
            // found the task
            result = tmp;
            break;
        }
    }
    return result;
}


void init_system_controller() {
    g_cs_system_controller.first_task = NULL;
    g_cs_system_controller.last_task = NULL;
    g_cs_system_controller.last_id = 0;
    g_cs_system_controller.add_new_task_func = add_new_task;
    
    int runloop_id = create_task(start_run_loop);
    g_cs_system_controller.runloop = get_task_by_id(runloop_id); // set the run loop ptr

}



