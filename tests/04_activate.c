#include <stdio.h>
#include "../src/task.h"
#include "../src/scheduler.h"

task_t taskA, taskB;

void taskB_func(void) {
    printf("Task B started.\n");
    for (int i = 0; i < 3; i++) {
        printf("Task B: %d\n", i);
        yield();
    }
    printf("Task B finished.\n");
    terminate_task();
}

void taskA_func(void) {
    printf("Task A started.\n");
    // Активируем задачу B
    activate_task(&taskB);
    for (int i = 0; i < 3; i++) {
        printf("Task A: %d\n", i);
        yield();
    }
    printf("Task A finished.\n");
    terminate_task();
}

int main(void) {
    task_init(&taskA, 0, taskA_func);
    task_init(&taskB, 1, taskB_func);

    scheduler_init();
    scheduler_add(&taskA);
    scheduler_start();
    return 0;
}