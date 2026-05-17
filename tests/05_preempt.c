#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "../include/romaos.h"

// Явно объявляем задачи и функции
void high_prio_func(void);
void low_prio_func(void);
void my_irq_handler(int sig, siginfo_t *info, void *ctx);

task_t high_prio_task;
task_t low_prio_task;

// Функция высокоприоритетной задачи
void high_prio_func(void) {
    printf("High priority task running!\n");
    terminate_task();
}

// Функция низкоприоритетной задачи
void low_prio_func(void) {
    printf("Low priority task started.\n");
    // Имитируем прерывание
    raise(SIGRTMIN+0);
    // Сюда вернёмся только после завершения high_prio_task
    printf("Low priority task resumed after ISR.\n");
    terminate_task();
}

// Обработчик прерывания
void my_irq_handler(int sig, siginfo_t *info, void *ctx) {
    ENTER_ISR();
    printf("IRQ 0: activating high priority task.\n");
    activate_task(&high_prio_task);
    LEAVE_ISR();
}

int main(void) {
    task_init(&high_prio_task, 0, high_prio_func);
    task_init(&low_prio_task, 10, low_prio_func);

    port_init();                          // инициализируем (обнуляем handlers)
    port_irq_register(0, my_irq_handler); // теперь регистрируем обработчик

    scheduler_init();
    scheduler_add(&low_prio_task);
    scheduler_start();

    return 0;
}