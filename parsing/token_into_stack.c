#include "minishell.h"
t_stack *new_stack_node(t_parser *token)
{
    t_stack *new_node = (t_stack *)malloc(sizeof(t_stack));
    new_node->node = token;
    new_node->next = NULL;
    return new_node;
}

void push(t_stack **stack, t_parser *token)
{
    t_stack *new_node = new_stack_node(token);
    new_node->next = *stack;
    *stack = new_node;
}

t_parser *pop(t_stack **stack)
{
    if (!*stack)
        return NULL;
    t_stack *temp = *stack;
    t_parser *token = temp->node;
    *stack = temp->next;
    free(temp);
    return token;
}

void transfer_tokens_to_stack(t_parser *token_list, t_stack **stack)
{
    t_parser *current = token_list;
    t_stack *node;
    while (current != NULL)
    {
        node = new_stack_node(current);
        push_back_stack(&node, stack);
        current = current->next;
    }
}
