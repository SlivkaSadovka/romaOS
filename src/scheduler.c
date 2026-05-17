#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#define MAX_TASKS 2

static task_t *task_table[MAX_TASKS];
static int task_count = 0;
static int current_idx = -1;

task_t *current_task = NULL;

void scheduler_init(void) {
  printf("[DEBUG] scheduler_init\n");
  task_count = 0;
  current_idx = -1;
  current_task = NULL;
}

void scheduler_add(task_t *task) {
  printf("[DEBUG] scheduler_add: task=%p prio=%d\n", (void*)task, task->prio);
  if (task_count >= MAX_TASKS) {
    printf("Too many tasks!\n");
    exit(1);
  }
  task_table[task_count] = task;
  task_count++;

  if (getcontext(&task->ctx) == -1) {
    printf("getcontext failed\n");
    exit(1);
  }
  task->ctx.uc_stack.ss_sp = task->stack;
  task->ctx.uc_stack.ss_size = STACK_SIZE;
  task->ctx.uc_link = NULL;
  makecontext(&task->ctx, task->entry, 0);
  task->state = TASK_READY;
}

static int find_highest_prio_except(int exclude_idx) {
  int best = -1;
  for (int i = 0; i < task_count; i++) {
    if (i == exclude_idx) continue;
    if (task_table[i]->state == TASK_READY) {
      if (best == -1 || task_table[i]->prio < task_table[best]->prio) {
        best = i;
      }
    }
  }
  return best;
}

static int find_highest_prio(void) {
    return find_highest_prio_except(-1);
}

static void switch_to(int idx) {
  if (idx == current_idx) {
    return;
  }
  task_t *old = current_task;
  task_t *new_task = task_table[idx];

  if (old) {
    if (swapcontext(&old->ctx, &new_task->ctx) == -1) {
      perror("swapcontext");
      exit(1);
    }
  } else {
    if (setcontext(&new_task->ctx) == -1) {
      perror("setcontext");
      exit(1);
    }
  }
  current_task = new_task;
  current_idx = idx;
}

__attribute__((noreturn)) void scheduler_start(void) {
  printf("[DEBUG] scheduler_start\n");
  int idx = find_highest_prio();
  if (idx == -1) {
    printf("No ready task!\n");
    exit(1);
  }

  printf("[DEBUG] starting task index %d (prio=%d)\n", idx, task_table[idx]->prio);
  current_idx = idx;
  current_task = task_table[idx];
  current_task->state = TASK_RUNNING;

  setcontext(&current_task->ctx);

  fprintf(stderr, "setcontext failed\n");
  exit(1);
}

void yield(void) {
  printf("[DEBUG] yield called, current_idx=%d current_task=%p\n", current_idx, (void*)current_task);
  if (!current_task) return;

  if (current_task->state == TASK_RUNNING) {
    current_task->state = TASK_READY;
    printf("[DEBUG] set state TASK_READY for task %p\n", (void*)current_task);
  }

  int next = find_highest_prio_except(current_idx);
  if (next == -1) {
    printf("[DEBUG] no other ready tasks, keep running current\n");
    printf("No ready tasks, exiting.\n");
    exit(0);
  }

  task_t *old = current_task;
  task_t *new_task = task_table[next];
  printf("[DEBUG] switching from task %p (idx=%d) to task %p (idx=%d, prio=%d)\n", (void*)old, current_idx, (void*)new_task, next, new_task->prio);
  current_idx = next;
  current_task = new_task;
  new_task->state = TASK_RUNNING;

  swapcontext(&old->ctx, &new_task->ctx);
}