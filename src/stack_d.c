#include <stdio.h>
#include <stdlib.h>

#include "stack_d.h"

node_d new_node_d(double num) {
    node_d n;
    n = (node_d) malloc(sizeof(node_double));
    n->num = num;
    n->prev = NULL;
    return n;
}

stack_d new_stack_d() {
    stack_d s;
    s = (stack_d) malloc(sizeof(stack_double));
    s->top = NULL;
    return s;
}

void add_stack_node_d(stack_d s, double num) {
    node_d n = new_node_d(num);
    n->prev = s->top;
    s->top = n;
}

node_double extract_stack_node_double(stack_d s) {
    node_double n = {0, NULL};

    if (s->top != NULL) {
        node_d aux;
        aux = s->top;
        s->top = s->top->prev;
        n = *aux;
        free(aux);
    }

    return n;
}

void destroy_stack_d(stack_d s) {
    node_double n;
    while (s->top != NULL) {
        n = extract_stack_node_double(s);
    }
    free(s);
}

int is_empty_d(stack_d s) {
    return s->top == NULL;
}
