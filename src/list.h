#ifndef ROMAOS_LIST_H
#define ROMAOS_LIST_H

#include <stddef.h>
#include <stdbool.h>

#define container_of(ptr, type, member) ({                      \
    const typeof(((type *)0)->member) *__mptr = (ptr);          \
    (type *)((char *)__mptr - offsetof(type, member));          \
})

typedef struct list_node {
  struct list_node *next;
  struct list_node *prev;
} list_node_t;

typedef list_node_t list_t;

static inline void list_init(list_t *list) {
  list->next = list;
  list->prev = list;
}

static inline bool list_empty(const list_t *list) {
  return list->next == list;
}

static inline void __list_add(list_node_t *node, list_node_t *prev, list_node_t *next) {
  next->prev = node;
  node->next = next;
  node->prev = prev;
  prev->next = node;
}

static inline void list_del(list_node_t *node) {
  node->prev->next = node->next;
  node->next->prev = node->prev;
  node->next = node->prev = NULL;
}

static inline void list_add_tail(list_t *list, list_node_t *node) {
  __list_add(node, list->prev, list);
}

static inline list_node_t *list_pop_first(list_t *list) {
  if (list_empty(list)) return NULL;
  list_node_t *node = list->next;
  list_del(node);
  return node;
}

static inline bool list_node_in_list(const list_node_t *node) {
  return node->next != NULL && node->prev != NULL;
}

#define list_for_each(list, pos) \
for (pos = (list)->next; pos != (list); pos = pos->next)

#define list_entry(node, type, member) container_of(node, type, member)

#define LIST_INIT(list) { .next = (list), .prev = (list) }

#endif