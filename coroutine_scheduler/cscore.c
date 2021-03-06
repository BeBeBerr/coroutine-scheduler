//
//  cscore.c
//  coroutine_scheduler
//
//  Created by 王路远 on 2018/11/18.
//  Copyright © 2018 王路远. All rights reserved.
//

#include <strings.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

#include "cscore.h"
#include "cssysctrl.h"


enum register_index {
    rbx = 0,
    rsp = 1,
    rbp = 2,
    r12 = 3,
    r13 = 4,
    r14 = 5,
    r15 = 6,
    pc = 7,
};


// From context_handler.s
extern int save_context(context_t *ctx_from);
extern void restore_context(context_t *ctx_to);

extern cs_system_controller g_cs_system_controller;

void switch_task(task_t *t_from, task_t *t_to) {
    // return 0 if is saving
    int is_restored = save_context(&(t_from->ctx));
    if (is_restored == 0) {
        // is saving
        restore_context(&(t_to->ctx)); // switch to another task
    } else {
        // is being restored
        return; // just return back and continue
    }
}

// Switch current running task to run loop.
void spin_once() {
    // all tasks should be managed by run loop
    switch_task(g_cs_system_controller.running_task, g_cs_system_controller.runloop);
}

// Create a new coroutine task
int create_task(task_handler_t handler, char *desc) {
    
    task_t *task = malloc(sizeof(task_t));
    
    int stack_size = (1 << 20); // 1M
    void *stack_ptr = malloc(stack_size);
    
    task->next_task = NULL;
    task->state = cs_task_state_ready;
    
    task->desc[0] = '\0';
    if (desc != NULL) {
        strcpy(task->desc, desc); // set the description
    }
    
    int tid = g_cs_system_controller.add_new_task_func(task);
    
    task->tid = tid;
    task->hander = handler;
  
    // stack pointer shoud point to the high address
    // Note: the type of stack_ptr is void *
    // so, add 1 to this pointer, it will only offset 1 byte.
    task->stack = stack_ptr + stack_size;
    
    // on macOS, stack is required to be aligned by 16 bytes.
    // add a padding to make it satisfied with the requiremnt.
    // should only add once (when init the stack)
    task->stack = task->stack - 8;
    
    memset(&(task->ctx), 0, sizeof(context_t)); // set values to 0 at the beginning
    task->ctx.buffer[rsp] = (long)task->stack; // set initial stack pointer
    task->ctx.buffer[pc] = (long)handler; // set initial pc to the function address, so it can jump to the function
    return tid;
}

// Start running a new coroutine task
void start_task(task_t *task) {
    restore_context(&(task->ctx)); // set the context (registers) so the handler function will start running
}

// Get the running task ID
int get_current_tid() {
    return g_cs_system_controller.running_task->tid;
}

void start_scheduler() {
    printf("Welcome!\n");
    if (g_cs_system_controller.runloop == NULL) {
        print_error("Empty run loop pointer. Check if initialized the system controller.");
    }
    start_task(g_cs_system_controller.runloop);
}


// We have to call this function before a task 'returns'.
// The reason is, the tasks are created and runned by jump to its address,
// so they don't have callers.
// They cannot return, but they can 'exit'.
void exit_task() {
    task_t *running_task = g_cs_system_controller.running_task;
    g_cs_system_controller.kill_task_by_id_func(running_task->tid);
    spin_once();
}


// Standard sleep function will block the whole system (as we have only 1 thread).
// Use cs_sleep instead.
// This function cannot ensure high precision, as other tasks may takes too much time to run.
void cs_sleep(long sleep_seconds) {
    if (sleep_seconds <= 0) {
        print_error("%s", "cannot sleep a negative or zero value of time.");
        return;
    }
    
    task_t *task_to_sleep = g_cs_system_controller.running_task; // get the caller task
    task_to_sleep->state = cs_task_state_sleeping; // set state to sleeping
    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long current_time = tv.tv_sec; // get current time stamp
    long expired_time = current_time + sleep_seconds;
    
    task_to_sleep->sleep_expired_time = expired_time; // set wake up time
}

// Read function will block the whole system
// This function will use the unblocking version, and always spin
long cs_read_terminal(void *buf, long size) {
    int fd = open("/dev/tty", O_RDONLY | O_NONBLOCK); // open standard input
    if (fd < 0) {
        print_error("%s", "open /dev/tty error.");
        return -1;
    }
    
    while (1) {
        long n = read(fd, buf, size); // try to read once
        if (n >= 0) {
            return n;
        } else {
            spin_once(); //try angain later
        }
    }
}

// To make the terminal cleaner,
// Use this function to 'print' messages to a named pipe
void cs_output(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char str[200];
    vsprintf(str, fmt, args);
    va_end(args);
    
    int pipe_fd = open("./cs_output", O_WRONLY);
    if (pipe_fd < 0) {
        // named pipe haven't created yet
        system("mkfifo ./cs_output");
        pipe_fd = open("./cs_output", O_WRONLY);
    }
    
    write(pipe_fd, str, strlen(str));
    // close(pipe_fd); // Do not close the pipe.
}
