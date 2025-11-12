#ifndef T_STRUCTS_INCLUDED
#define T_STRUCTS_INCLUDED

#include "t_types.h"

typedef struct stack stack_t;
typedef struct queue queue_t;

stack_t *create_stack(size_t data_size);
void delete_stack(stack_t *dead_stack);
void stack_push(stack_t *stack, void *data);
void stack_pop(stack_t *stack);
void *stack_peek(stack_t *stack);
size_t stack_get_size(stack_t *stack);

queue_t *create_queue(size_t data_size);
void delete_queue(queue_t *dead_queue);
void queue_push(queue_t *queue, void*data);
void queue_pop(queue_t *queue);
void *queue_peek(queue_t *queue);
size_t queue_get_size(queue_t *queue);

#endif

