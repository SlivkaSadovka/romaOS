#ifndef ROMAOS_H
#define ROMAOS_H

#include <stdint.h>
#include <signal.h>
#include <ucontext.h>
#include "list.h"
#include "task.h"
#include "scheduler.h"
#include "port.h"
#include "resource.h"

// ──────────────────────────────────
//  Макросы задач
// ──────────────────────────────────
#define TASK(name, prio)                                 \
    void name##_func(void);                               \
    task_t name;                                          \
    __attribute__((constructor)) static void name##_init(void) {  \
        task_init(&name, prio, name##_func);              \
    }                                                     \
    void name##_func(void)

// ──────────────────────────────────
//  Макросы ресурсов
// ──────────────────────────────────
#define RESOURCE(name, ceiling)                               \
    resource_t name;                                          \
    __attribute__((constructor)) static void name##_init(void) { \
        resource_init(&name, ceiling);                       \
    }

// ──────────────────────────────────
//  Макросы прерываний
// ──────────────────────────────────
#define ISR(name, irq)                                                     \
    void name##_handler(int sig, siginfo_t *info, void *ctx);              \
    __attribute__((constructor)) static void name##_register(void) {       \
        port_irq_register(irq, name##_handler);                           \
    }                                                                      \
    void name##_handler(int sig, siginfo_t *info, void *ctx)

// ──────────────────────────────────
//  Системные вызовы (API)
// ──────────────────────────────────

/* Запуск ОС с первой задачей */
__attribute__((noreturn)) void start_os(task_t *task);
#define StartOS(task) start_os(&(task))

/* Завершить текущую задачу */
#define TerminateTask() terminate_task()

/* Активировать задачу (перевести из SUSPENDED в READY) */
#define ActivateTask(t) activate_task(&(t))

/* Захватить / освободить ресурс */
#define GetResource(r)    resource_get(&(r))
#define ReleaseResource(r) resource_release(&(r))

/* Вход / выход из обработчика прерывания */
#define ENTER_ISR()        port_enter_isr(current_irq, current_ucontext)
#define LEAVE_ISR()        port_leave_isr()
#define ISRActivateTask(t) activate_task(&(t))

/* Завершение всей ОС */
#define ShutdownOS()      exit(0)

#endif // ROMAOS_H