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
#include "csshell.h"

cs_system_controller g_cs_system_controller; // should be singleton

// Add a new task to the system
// Return new task id
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

// Return NULL if task not found
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

// Kill a task
// To kill a task, just remove it from the tasks list
void kill_task_by_id(int tid) {
    if (tid == g_cs_system_controller.runloop->tid) {
        print_error("%s", "Can not kill the system run loop!");
        return;
    }
    
    task_t *task_ptr = NULL;
    if (tid == g_cs_system_controller.first_task->tid) {
        // is the first node
        task_ptr = g_cs_system_controller.first_task;
        g_cs_system_controller.first_task = g_cs_system_controller.first_task->next_task; // change the header pointer
    } else {
        task_t *previous_ptr = g_cs_system_controller.first_task;
        while (previous_ptr->next_task != g_cs_system_controller.last_task) {
            if (tid == previous_ptr->next_task->tid) {
                task_ptr = previous_ptr->next_task;
                previous_ptr->next_task = task_ptr->next_task; // remove the task from the list
                break;
            }
            previous_ptr = previous_ptr->next_task;
        }
        if (tid == g_cs_system_controller.last_task->tid) {
            // the task is the last one
            task_ptr = previous_ptr->next_task;
            g_cs_system_controller.last_task = previous_ptr;
            g_cs_system_controller.last_task->next_task = NULL;
        }
    }
    if (task_ptr == NULL) {
        // task not found
        print_error("%s", "task not found.");
    } else {
        // free resources
        free(task_ptr);
    }
}


void init_system_controller() {
    g_cs_system_controller.first_task = NULL;
    g_cs_system_controller.last_task = NULL;
    g_cs_system_controller.last_id = 0;
    g_cs_system_controller.add_new_task_func = add_new_task;
    g_cs_system_controller.get_task_by_id_func = get_task_by_id;
    g_cs_system_controller.kill_task_by_id_func = kill_task_by_id;
    
    int runloop_id = create_task(start_run_loop, "Run Loop");
    g_cs_system_controller.runloop = get_task_by_id(runloop_id); // set the run loop ptr
    
    create_task(shell_task, "Shell"); // create shell task

}



