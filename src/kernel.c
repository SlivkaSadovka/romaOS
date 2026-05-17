#include <stdio.h>
#include <stdlib.h>
#include "task.h"

#define __noreturn __attribute__((noreturn))

__noreturn void os_start(task_t *task) {
  printf("Starting romaOS...\n");

  task->state = TASK_RUNNING;
  task->entry();

  printf("Task finished, shutting down OS...\n");
  exit(0);
}
