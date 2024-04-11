#include "stack.h"
#include <stdlib.h>

void stack_init(stack_t *st) {
	st->len = 0;
	st->head = NULL;
}

float pop(stack_t *st) {
	if (st->len == 0) {
		return -1;
	} else {
		node_t *top = st->head;
		st->head = top->next;
		float out = top->n;
		free(top);
		st->len--;
		return out;
	}
}

float peek(stack_t *st) {
	return st->head->n;
}

void push(stack_t *st, float n) {
	node_t *top = malloc(sizeof(node_t));
	top->n = n;
	top->next = st->head;
	st->head = top;
	st->len++;
}

void free_stack(stack_t *st) {
	node_t *cur = st->head;
	while (cur) {
		node_t *tmp = cur;
		cur = cur->next;
		free(tmp);
	}
	free(st);
}

