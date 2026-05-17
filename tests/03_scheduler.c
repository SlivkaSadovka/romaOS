#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "task.h"

task_t t1, t2;

void f1(void) {
    printf("Task1\n");
    yield();
    printf("Task1 done\n");
    terminate_task();
}

void f2(void) {
    printf("Task2\n");
    yield();
    printf("Task2 done\n");
    terminate_task();
}

int main(void) {
    task_init(&t1, 0, f1);
    task_init(&t2, 1, f2);
    scheduler_init();
    scheduler_add(&t1);
    scheduler_add(&t2);
    scheduler_start();
    return 0;
}