#ifndef SRC_STACK_C_H_
#define SRC_STACK_C_H_

typedef struct node_char {
    char c;
    struct node_char* prev;
} node_char;

typedef node_char* node_c;

typedef struct stack_char {
    node_c top;
} stack_char;

typedef stack_char* stack_c;

node_c new_node_c(char c);
stack_c new_stack_c();
void add_stack_node_c(stack_c s, char c);
node_char extract_stack_node_char(stack_c s);
void destroy_stack_c(stack_c s);
int is_empty_c(stack_c s);

#endif  // SRC_STACK_C_H_
