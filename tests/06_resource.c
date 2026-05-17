#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "../src/task.h"
#include "../src/scheduler.h"
#include "../src/resource.h"
#include "../src/port.h"

// Явно задаём макросы для входа/выхода из прерывания,
// потому что не включаем romaos.h (чтобы избежать лишних зависимостей)
#define ENTER_ISR()   port_enter_isr(current_irq, current_ucontext)
#define LEAVE_ISR()   port_leave_isr()

resource_t res;

task_t high_task, low_task;

void high_func(void) {
    printf("High tries to get resource\n");
    resource_get(&res);
    printf("High got resource, releasing\n");
    resource_release(&res);
    printf("High finished\n");
    terminate_task();
}

void low_func(void) {
    printf("Low getting resource\n");
    resource_get(&res);
    printf("Low got resource. Activating High via ISR.\n");
    raise(SIGRTMIN+0);
    printf("Low releasing resource\n");
    resource_release(&res);
    printf("Low finished\n");
    terminate_task();
}

void isr_handler(int sig, siginfo_t *info, void *ctx) {
    ENTER_ISR();
    printf("ISR: activating High\n");
    activate_task(&high_task);
    LEAVE_ISR();
}

int main(void) {
    task_init(&high_task, 5, high_func);
    task_init(&low_task, 10, low_func);
    resource_init(&res, 3);  // потолок = 3

    port_init();
    port_irq_register(0, isr_handler);
    scheduler_init();
    scheduler_add(&low_task);
    scheduler_start();
    return 0;
}