#include "romaos.h"

/**
 * Главная функция запуска ОС.
 * Инициализирует порт (прерывания), планировщик,
 * добавляет стартовую задачу и запускает планировщик.
 * Никогда не возвращается.
 */
__attribute__((noreturn)) void start_os(task_t *task) {
    port_init();
    scheduler_init();
    scheduler_add(task);
    scheduler_start();
}