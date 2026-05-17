#include "port.h"
#include "scheduler.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define IRQ_COUNT 10

static isr_t handlers[IRQ_COUNT];
static sigset_t irq_masks[IRQ_COUNT];

volatile int in_isr = 0;
volatile int return_to_isr = 0;

int current_irq = 0;
ucontext_t *current_ucontext = NULL;

static void dispatcher(int sig, siginfo_t *info, void *ucontext_ptr) {
    int irq = sig - SIGRTMIN;
    if (irq < 0 || irq >= IRQ_COUNT) return;
    current_irq = irq;
    current_ucontext = (ucontext_t *)ucontext_ptr;
    if (handlers[irq])
        handlers[irq](sig, info, ucontext_ptr);
}

int port_irq_register(int irq, isr_t handler) {
    if (irq < 0 || irq >= IRQ_COUNT) return -1;
    handlers[irq] = handler;

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = dispatcher;
    sa.sa_flags = SA_SIGINFO;
    sigfillset(&sa.sa_mask);

    sigemptyset(&irq_masks[irq]);
    for (int i = irq; i < IRQ_COUNT; i++) {
        sigaddset(&irq_masks[irq], SIGRTMIN + i);
    }

    return sigaction(SIGRTMIN + irq, &sa, NULL);
}

void port_enter_isr(int irq, ucontext_t *ctx) {
    if (!in_isr) {
        task_t *cur = scheduler_current();
        if (cur) {
            cur->ctx = *ctx;
            scheduler_mark_ready(cur);
        }
    } else {
        return_to_isr = 1;
    }
    in_isr = 1;
    sigprocmask(SIG_SETMASK, &irq_masks[irq], NULL);
}

void port_leave_isr(void) {
    if (!return_to_isr) {
        scheduler_dispatch();
    }
}

void port_init(void) {
    in_isr = 0;
    return_to_isr = 0;
}