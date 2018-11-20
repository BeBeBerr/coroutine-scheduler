//
//  csshell.c
//  coroutine_scheduler
//
//  Created by 王路远 on 2018/11/19.
//  Copyright © 2018 王路远. All rights reserved.
//

#include <string.h>
#include <stdlib.h>
#include "csshell.h"
#include "cscore.h"
#include "unistd.h"
#include "cssysctrl.h"
#include "tasks.h"

#define Buf_Size 100

extern cs_system_controller g_cs_system_controller;

void print_all_tasks() {
    task_t *tmp = g_cs_system_controller.first_task;
    print_yellow("%s", "TID\tDESC");
    while (tmp != NULL) {
        int tid = tmp->tid;
        char *desc = tmp->desc;
        if (strlen(desc) == 0) {
            desc = "--";
        }
        tmp = tmp->next_task;
        printf("%d\t%s\n", tid, desc);
    }
}

void run_task(char *task_name) {
    int tid = -1;
    if (strcmp(task_name, "A") == 0) {
        tid = create_task(task_A, task_name);
    } else if (strcmp(task_name, "B") == 0) {
        tid = create_task(task_B, task_name);
    }
    
    if (tid < 0) {
        print_error("%s", "task didn't find.");
    } else {
        print_yellow("%s %d", "task created, tid:", tid);
    }
}

void handle_command(char *input) {
    char *command = strtok(input, " \n"); // the first word separated by 'space' is the command
    //printf("command:%s", command);
    if (strcmp(command, "run") == 0) {
        // run a task
        run_task(strtok(NULL, " \n"));
    } else if (strcmp(command, "kill") == 0) { // kill a task
        // get task id by split the string again
        int tid = atoi(strtok(NULL, " \n")); // as we have splited once, we should pass NULL
        printf("killing task %d\n", tid);
        g_cs_system_controller.kill_task_by_id_func(tid);
    } else if (strcmp(command, "ts") == 0) { // show all tasks
        print_all_tasks();
    }
}

void shell_task() {
    char reading[Buf_Size];
    while (1) {
        cs_read_terminal(reading, Buf_Size);
        handle_command(reading);
        spin_once();
    }
}
