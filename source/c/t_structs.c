#include "t_types.h"
#include "t_memory.h"

typedef struct linked_node {
  void *data;
  size_t data_size;
  struct linked_node *next_node;
} linked_node_t;

typedef struct doubly_linked_node {
  void *data;
  size_t data_size;
  struct doubly_linked_node *previous_node;
  struct doubly_linked_node *next_node;
} doubly_linked_node_t;

typedef struct stack {
  linked_node_t *top;
  size_t data_size;
  size_t size;
} stack_t;

typedef struct queue {
  linked_node_t *head;
  linked_node_t *tail;
  size_t data_size;
  size_t size;
} queue_t;

static linked_node_t *create_linked_node(void *data, size_t data_size);
static doubly_linked_node_t *create_doubly_linked_node(void *data, size_t data_size);
static void delete_linked_node(linked_node_t *new_node);
static void delete_doubly_linked_node(doubly_linked_node_t *new_node);

stack_t *create_stack(size_t data_size);
void delete_stack(stack_t *stack);
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

static linked_node_t *create_linked_node(void *data, size_t data_size) {
  linked_node_t *new_node = heap_alloc(sizeof(linked_node_t));

  if (new_node == NULL_PTR) {
    return NULL_PTR;
  }

  new_node->next_node = NULL_PTR;
  new_node->data_size = data_size;
  new_node->data = heap_alloc(data_size);

  if (new_node->data == NULL_PTR) {
    heap_free(new_node, sizeof(linked_node_t));
    return NULL_PTR;
  }

  memory_copy(data, new_node->data, data_size);
  
  return new_node;
}

static doubly_linked_node_t *create_doubly_linked_node(void *data, size_t data_size) {
  doubly_linked_node_t *new_node = heap_alloc(sizeof(doubly_linked_node_t));

  if (new_node == NULL_PTR) {
    return NULL_PTR;
  }

  new_node->data = heap_alloc(data_size);
  
  if (new_node->data == NULL_PTR) {
    heap_free(new_node, sizeof(doubly_linked_node_t));
    return NULL_PTR;
  }
  
  new_node->data_size = data_size;
  new_node->next_node = NULL_PTR;
  new_node->previous_node = NULL_PTR;
  memory_copy(data, new_node->data, data_size);
  
  return new_node;
}

static void delete_linked_node(linked_node_t *dead_node) {
  if (dead_node == NULL_PTR) {
    return;
  }

  heap_free(dead_node->data, dead_node->data_size);
  heap_free(dead_node, sizeof(linked_node_t));  
}

static void delete_doubly_linked_node(doubly_linked_node_t *dead_node) {
  if (dead_node == NULL_PTR) {
    return;
  }

  heap_free(dead_node->data, dead_node->data_size);
  heap_free(dead_node, sizeof(doubly_linked_node_t));  
}

stack_t *create_stack(size_t data_size) {
  stack_t *new_stack;

  new_stack = heap_alloc(sizeof(stack_t));

  if (new_stack == NULL_PTR) {
    return NULL_PTR;
  }

  new_stack->top = NULL_PTR;
  new_stack->size = 0;
  new_stack->data_size = data_size;

  return new_stack;
}

void delete_stack(stack_t *dead_stack) {
  while (stack_get_size(dead_stack) != 0) {
    stack_pop(dead_stack);
  }

  heap_free(dead_stack, sizeof(stack_t));
}

void stack_push(stack_t *stack, void *data) {
  linked_node_t *new_node;

  if (stack == NULL_PTR) {
    return;
  }

  new_node = create_linked_node(data, stack->data_size);

  if (new_node == NULL_PTR) {
    return;
  }

  new_node->next_node = stack->top;
  stack->top = new_node;
  stack->size++;
}

void stack_pop(stack_t *stack) {
  linked_node_t *dead_node;
  
  if ((stack == NULL_PTR) || (stack->top == NULL_PTR)) {
    return;
  }

  dead_node = stack->top;
  stack->top = stack->top->next_node;
  delete_linked_node(dead_node);
  stack->size--;
}

void *stack_peek(stack_t *stack) {
  if ((stack == NULL_PTR) || (stack->top == NULL_PTR)) {
    return NULL_PTR;
  }

  return stack->top->data;  
}

size_t stack_get_size(stack_t *stack) {
  if (stack == NULL_PTR) {
    return 0;
  }

  return stack->size;
}

queue_t *create_queue(size_t data_size) {
  queue_t *new_queue = heap_alloc(sizeof(queue_t));
  if (new_queue == NULL_PTR) {
    return NULL_PTR;
  }

  new_queue->head = NULL_PTR;
  new_queue->tail = NULL_PTR;
  new_queue->size = 0;
  new_queue->data_size = data_size;

  return new_queue;
}

void delete_queue(queue_t *dead_queue) {
  linked_node_t *current_node;
  linked_node_t *next_node;

  if (dead_queue == NULL_PTR) {
    return;
  }

  current_node = dead_queue->head;

  while (current_node != NULL_PTR) {
    next_node = current_node->next_node;
    delete_linked_node(current_node);
    current_node = next_node;
  }

  heap_free(dead_queue, sizeof(queue_t));
}

void queue_push(queue_t *queue, void *data) {
  linked_node_t *new_node;

  if (queue == NULL_PTR) {
    return;
  }

  new_node = create_linked_node(data, queue->data_size);

  if (new_node == NULL_PTR) {
    return;
  }

  new_node->next_node = NULL_PTR;

  if (queue->tail == NULL_PTR) {
    queue->head = new_node;
    queue->tail = new_node;
    queue->size++;
    return;
  }

  queue->tail->next_node = new_node;
  queue->tail = new_node;
  queue->size++;
}

void queue_pop(queue_t *queue) {
  linked_node_t *dead_node;

  if ((queue == NULL_PTR) || (queue->head == NULL_PTR)) {
    return;
  }

  dead_node = queue->head;
  queue->head = dead_node->next_node;

  if (queue->head == NULL_PTR) {
    queue->tail = NULL_PTR;
  }

  delete_linked_node(dead_node);
  queue->size--;
}

void *queue_peek(queue_t *queue) {
  if ((queue == NULL_PTR) || (queue->head == NULL_PTR)) {
    return NULL_PTR;
  }
  
  return queue->head->data;
}

size_t queue_get_size(queue_t *queue) {
  if (queue == NULL_PTR) {
    return 0;
  }

  return queue->size;
}

