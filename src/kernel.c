#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "scheduler.h"

#define __noreturn __attribute__((noreturn))

__noreturn void os_start(task_t *task) {
  printf("Starting romaOS...\n");

  scheduler_init();
  scheduler_add(task);
  scheduler_start();

  printf("Task finished, shutting down OS...\n");
  exit(0);
}
