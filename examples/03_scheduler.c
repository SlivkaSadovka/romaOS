#include <stdio.h>
#include <stdlib.h>
#include "../src/task.h"
#include "../src/scheduler.h"

// Глобальные задачи (пока так)
task_t taskA, taskB;

void taskA_func(void) {
    for (int i = 0; i < 5; i++) {
        printf("Task A: %d\n", i);
        yield();   // отдаём управление планировщику
    }
    printf("Task A finished.\n");
    exit(0);
}

void taskB_func(void) {
    for (int i = 0; i < 5; i++) {
        printf("Task B: %d\n", i);
        yield();
    }
    printf("Task B finished.\n");
    exit(0);
}

int main(void) {
    // Настраиваем задачи
    taskA.prio = 0;            // высший приоритет
    taskA.entry = taskA_func;
    taskA.state = TASK_READY;

    taskB.prio = 1;            // ниже, чем у A
    taskB.entry = taskB_func;
    taskB.state = TASK_READY;

    // Инициализация ОС
    scheduler_init();
    // Добавляем задачи (порядок не важен)
    scheduler_add(&taskA);
    scheduler_add(&taskB);
    // Запускаем планировщик
    scheduler_start();

    return 0; // никогда
}