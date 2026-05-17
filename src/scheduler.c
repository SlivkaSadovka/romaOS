#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

static list_t ready_list;      // очередь готовых задач
static task_t *current_task = NULL;

void scheduler_init(void) {
    list_init(&ready_list);
    current_task = NULL;
}

void scheduler_add(task_t *task) {
    if (task->state == TASK_READY || task->state == TASK_RUNNING) {
        return;
    }
    if (task->ctx.uc_stack.ss_sp == NULL) {
        if (getcontext(&task->ctx) == -1) {
            fprintf(stderr, "getcontext failed\n");
            exit(1);
        }
        task->ctx.uc_stack.ss_sp = task->stack;
        task->ctx.uc_stack.ss_size = STACK_SIZE;
        task->ctx.uc_link = NULL;
        makecontext(&task->ctx, task->entry, 0);
    }
    task->state = TASK_READY;
    list_add_tail(&ready_list, &task->sched_node);
}

void scheduler_remove(task_t *task) {
    if (list_node_in_list(&task->sched_node)) {
        list_del(&task->sched_node);
    }
    task->state = TASK_SUSPENDED;
}

static task_t *find_highest_prio(task_t *exclude) {
    task_t *best = NULL;
    list_node_t *pos;
    list_for_each(&ready_list, pos) {
        task_t *t = list_entry(pos, task_t, sched_node);
        if (t == exclude) continue;
        if (t->state == TASK_READY) {
            if (best == NULL || t->prio < best->prio) {
                best = t;
            }
        }
    }
    return best;
}

__attribute__((noreturn)) void scheduler_start(void) {
    if (list_empty(&ready_list)) {
        printf("No ready tasks!\n");
        exit(1);
    }
    task_t *first = find_highest_prio(NULL);
    if (!first) {
        printf("No ready tasks!\n");
        exit(1);
    }

    list_del(&first->sched_node);
    current_task = first;
    current_task->state = TASK_RUNNING;
    setcontext(&current_task->ctx);
    __builtin_unreachable();
}

void yield(void) {
    if (!current_task) return;

    if (current_task->state == TASK_RUNNING) {
        current_task->state = TASK_READY;
        if (!list_node_in_list(&current_task->sched_node)) {
            list_add_tail(&ready_list, &current_task->sched_node);
        }
    }
    task_t *next = find_highest_prio(current_task);
    if (!next) {
        current_task->state = TASK_RUNNING;
        if (list_node_in_list(&current_task->sched_node)) {
            list_del(&current_task->sched_node);
        }
        return;
    }

    list_del(&next->sched_node);
    next->state = TASK_RUNNING;

    task_t *old = current_task;
    current_task = next;

    swapcontext(&old->ctx, &next->ctx);
}

__attribute__((noreturn)) void terminate_task(void) {
    if (!current_task) {
        exit(1);
    }
    if (list_node_in_list(&current_task->sched_node)) {
        list_del(&current_task->sched_node);
    }
    current_task->state = TASK_SUSPENDED;

    task_t *next = find_highest_prio(NULL);
    if (!next) {
        printf("All tasks finished, exiting.\n");
        exit(0);
    }
    list_del(&next->sched_node);
    next->state = TASK_RUNNING;
    current_task = next;
    setcontext(&next->ctx);
}

void activate_task(task_t *task) {
    if (task->state == TASK_SUSPENDED) {
        scheduler_add(task);
    }
}