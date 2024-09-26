#include "../minishell.h"

int is_operator(Token *node)
{
    if (node->type == TOKEN_DOUBLE_QUOTED || node->type == TOKEN_SINGLE_QUOTED || node->type == TOKEN_COMMAND || node->type == TOKEN_OPTION || node->type == TOKEN_BUILT_IN || node->type == TOKEN_UNKNOWN || node->type == TOKEN_ARGUMENT)
        return (0);
    return (1);
}

int is_operand(Token *node)
{
    if (node->type == TOKEN_UNKNOWN || node->type == TOKEN_DOUBLE_QUOTED || node->type == TOKEN_SINGLE_QUOTED || node->type == TOKEN_COMMAND || node->type == TOKEN_BUILT_IN || node->type == TOKEN_ARGUMENT || node->type == TOKEN_OPTION)
        return (1);
    return (0);
}

t_stack *push_stack(t_stack *stack, Token *node)
{
    t_stack *head;

    head = malloc(sizeof(t_stack));
    if (!head)
        return (NULL);
    
    // Allocate memory for head->node before using it
    head->node = malloc(sizeof(Token)); // Assuming 'node' is a pointer to Token
    if (!head->node) {
        free(head);  // Clean up previously allocated memory
        return (NULL);
    }

    head->node->token = node;
    head->next = stack;
    return head;
}


t_stack *pop_stack(t_stack **stack)
{
    if (!stack || !*stack)
        return NULL;
    
    t_stack *node = *stack;
    *stack = (*stack)->next;
    node->next = NULL;
    return node;
}