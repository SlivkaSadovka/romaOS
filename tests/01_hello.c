#include <stdio.h>
#include "task.h"
#include "scheduler.h"

void hello_task(void) {
    printf("Hello, RomaOS!\n");
    terminate_task();
}

int main(void) {
    task_t hello;
    task_init(&hello, 0, hello_task);

    scheduler_init();
    scheduler_add(&hello);
    scheduler_start();

    return 0;
}