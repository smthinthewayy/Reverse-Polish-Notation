#include <stdio.h>
#include <stdlib.h>

#include "stack_c.h"

node_c new_node_c(char c) {
    node_c n;
    n = (node_c) malloc(sizeof(node_char));
    n->c = c;
    n->prev = NULL;
    return n;
}

stack_c new_stack_c() {
    stack_c s;
    s = (stack_c) malloc(sizeof(stack_char));
    s->top = NULL;
    return s;
}

void add_stack_node_c(stack_c s, char c) {
    node_c n = new_node_c(c);
    n->prev = s->top;
    s->top = n;
}

node_char extract_stack_node_char(stack_c s) {
    node_char n = {0, NULL};

    if (s->top != NULL) {
        node_c aux;
        aux = s->top;
        s->top = s->top->prev;
        n = *aux;
        free(aux);
    }

    return n;
}

void destroy_stack_c(stack_c s) {
    node_char n;
    while (s->top != NULL) {
        n = extract_stack_node_char(s);
    }
    free(s);
}

int is_empty_c(stack_c s) {
    return s->top == NULL;
}
