#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

void scheduler_init(void);
void scheduler_add(task_t *task);
void scheduler_remove(task_t *task);
__attribute__((noreturn)) void scheduler_start(void);
void yield(void);
__attribute__((noreturn)) void terminate_task(void);
void activate_task(task_t *task);

#endif