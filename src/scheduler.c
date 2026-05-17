#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#define MAX_TASK 2

static task_t *tasks[MAX_TASKS];
static int task_count = 0;

static int current_task = 0;

void scheduler_init(void) {
  task_count = 0;
  current_task = 0;
}

void scheduler_add(task_t *task) {
  if (task_count >= MAX_TASKS) {
    printf("Too many tasks!\n");
    exit(1);
  }
  tasks[task_count++] = task;
}

__atribute__((noreturn)) void scheduler_start(void) {
  for (int i = 0; i < task_count; i++) {
    task_t *t = task[i];
    if (getcontext(&t->ctx) == -1) {
      printf("getcontext failed");
      exit(1);
    }

    // stack for current task
    t->ctx.uc_stack.ss_sp = t->stack;
    t->ctx.uc_stack.ss_size = STACK_SIZE;
    t->ctx.uc_link = NULL;
    makecontext(&t->ctx, t->entry, 0);
  }

  current_task = 0;
  task[0]->state = TASK_RUNNING;
  setcontext(&task[0]->ctx);
}
