#ifndef SRC_STACK_D_H_
#define SRC_STACK_D_H_

typedef struct node_double {
    double num;
    struct node_double* prev;
} node_double;

typedef node_double* node_d;

typedef struct stack_double {
    node_d top;
} stack_double;

typedef stack_double* stack_d;

node_d new_node_d(double num);
stack_d new_stack_d();
void add_stack_node_d(stack_d s, double num);
node_double extract_stack_node_double(stack_d s);
void destroy_stack_d(stack_d s);
int is_empty_d(stack_d s);

#endif  // SRC_STACK_D_H_
