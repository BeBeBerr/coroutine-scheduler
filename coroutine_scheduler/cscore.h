//
//  cscore.h
//  coroutine_scheduler
//
//  Created by 王路远 on 2018/11/18.
//  Copyright © 2018 王路远. All rights reserved.
//

// Coroutine Scheduler Core Functions

#ifndef cscore_h
#define cscore_h

#include <stdio.h>
#include "cstype.h"

void spin_once(void);
void switch_task(task_t *t_from, task_t *t_to);
int create_task(task_handler_t handler);
void exit_task();
// void start_task(task_t *task);
void start_scheduler(void);
void cs_sleep(long sleep_seconds);


// Colorful printing
#define RED   "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW   "\x1B[33m"
#define BLUE   "\x1B[34m"
#define MAGENTA   "\x1B[35m"
#define CYAN   "\x1B[36m"
#define WHITE   "\x1B[37m"
#define RESET "\x1B[0m"

#ifdef DEBUG
// Xocode doesn't support colorful printing
#define print_error(fmt, args...) printf("Error: " fmt "\n", ##args)
#else
#define print_error(fmt,args...) printf(RED "Error: " fmt RESET "\n", ##args)
#endif

#endif /*cscore_h*/
