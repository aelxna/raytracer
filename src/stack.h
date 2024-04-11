#ifndef STACK_H
#define STACK_H

typedef struct Node node_t;

struct Node {
	float n;
	node_t *next;
};

typedef struct {
	int len;
	node_t *head;
} stack_t;

void stack_init(stack_t *st);

float pop(stack_t *st);

float peek(stack_t *st);

void push(stack_t *st, float n);

void free_stack(stack_t *st);

#endif

