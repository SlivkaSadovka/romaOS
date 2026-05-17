#include "resource.h"
#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>

void resource_init(resource_t *res, int ceiling) {
    res->ceiling = ceiling;
    res->node.next = NULL;
    res->node.prev = NULL;
    list_init(&res->wait_queue);
}

// Найти задачу с наивысшим приоритетом в очереди ожидания
static task_t *find_highest_prio_in_queue(list_t *queue) {
    if (list_empty(queue)) return NULL;
    task_t *best = NULL;
    list_node_t *pos;
    list_for_each(queue, pos) {
        task_t *t = list_entry(pos, task_t, sched_node);
        if (best == NULL || t->prio < best->prio) {
            best = t;
        }
    }
    return best;
}

// Найти минимальный (наивысший) приоритет среди ресурсов задачи
static int min_ceiling(list_t *resources) {
    int min = 255;
    list_node_t *pos;
    list_for_each(resources, pos) {
        resource_t *r = list_entry(pos, resource_t, node);
        if (r->ceiling < min) min = r->ceiling;
    }
    return min;
}

void resource_get(resource_t *res) {
    task_t *cur = scheduler_current();
    if (!cur) return;

    if (list_node_in_list(&res->node)) {
        // ресурс занят – блокируемся
        cur->state = TASK_WAITING;
        list_add_tail(&res->wait_queue, &cur->sched_node);

        task_t *next = find_highest_prio_except(cur);
        if (!next) {
            printf("Deadlock detected, exiting.\n");
            exit(1);
        }
        // сохраняем контекст cur и переключаемся на next
        swapcontext(&cur->ctx, &next->ctx);
        // после пробуждения ресурс уже в списке cur->resources (добавлен при освобождении)
        return;
    }

    // ресурс свободен – захватываем
    list_add_tail(&cur->resources, &res->node);
    if (res->ceiling < cur->prio) {
        cur->prio = res->ceiling;
        scheduler_reschedule(cur);
    }
}

void resource_release(resource_t *res) {
    task_t *cur = scheduler_current();
    if (!cur) return;

    // удаляем ресурс из списка задачи
    list_del(&res->node);

    // восстанавливаем приоритет
    if (list_empty(&cur->resources)) {
        cur->prio = cur->base_prio;
    } else {
        cur->prio = min_ceiling(&cur->resources);
    }

    // будим самую приоритетную задачу из очереди ожидания
    task_t *waiter = find_highest_prio_in_queue(&res->wait_queue);
    if (waiter) {
        list_del(&waiter->sched_node);            // убираем из очереди ожидания
        waiter->state = TASK_READY;
        // передаём ресурс ожидающей задаче
        list_add_tail(&waiter->resources, &res->node);
        if (res->ceiling < waiter->prio) {
            waiter->prio = res->ceiling;
        }
        // помещаем waiter в ready_list
        if (!list_node_in_list(&waiter->sched_node)) {
            list_add_tail(&ready_list, &waiter->sched_node);
        }
        // переключаемся на waiter, сохраняя контекст текущей задачи
        swapcontext(&cur->ctx, &waiter->ctx);
        // сюда вернёмся, когда waiter уступит или завершится
    }

    // переставляем текущую задачу в очереди (приоритет мог измениться)
    scheduler_reschedule(cur);
}