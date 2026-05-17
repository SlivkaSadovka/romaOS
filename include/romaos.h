#ifndef ROMAOS_H
#define ROMAOS_H

#include "../src/task.h"
#include "../src/scheduler.h"
#include "../src/port.h"

#define TASK(name, prio) \
    void name##_func(void); \
    task_t name = { .prio = prio, .base_prio = prio, .entry = name##_func, \
                    .state = TASK_SUSPENDED, .sched_node = {NULL,NULL}, \
                    .ctx = {0}, .stack = {0} }; \
    void name##_func(void)

#define ACTIVATE_TASK(t) activate_task(&(t))
#define TERMINATE_TASK() terminate_task()
#define YIELD() yield()


#define ISR(name, irq) \
    void name##_handler(int sig, siginfo_t *info, void *ctx); \
    __attribute__((constructor)) static void name##_register(void) { \
        port_irq_register(irq, name##_handler); \
    } \
    void name##_handler(int sig, siginfo_t *info, void *ctx)

#define ENTER_ISR()   port_enter_isr(current_irq, current_ucontext)
#define LEAVE_ISR()   port_leave_isr()
#define ISR_ACTIVATE(t) activate_task(&(t))

#define OS_START(task) do { \
    port_init(); \
    scheduler_init(); \
    scheduler_add(&(task)); \
    scheduler_start(); \
} while(0)

#endif