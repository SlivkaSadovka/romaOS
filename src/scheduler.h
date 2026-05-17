#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

extern list_t ready_list;
void scheduler_init(void);
void scheduler_add(task_t *task);
void scheduler_remove(task_t *task);
__attribute__((noreturn)) void scheduler_start(void);
void yield(void);
__attribute__((noreturn)) void terminate_task(void);
void activate_task(task_t *task);

task_t *scheduler_current(void);
__attribute__((noreturn)) void scheduler_dispatch(void);
void scheduler_mark_ready(task_t *task);
void scheduler_reschedule(task_t *task);
task_t *find_highest_prio_except(task_t *exclude);

#endif