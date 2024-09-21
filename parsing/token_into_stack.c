#include "minishell.h"

t_stack *new_stack_node(t_parser *token)
{
    t_stack *new_node;
    
    new_node = (t_stack *)malloc(sizeof(t_stack));
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
    t_stack		*temp;
    t_parser	*token;

    if (!*stack)
        return NULL;
    temp = *stack;
    token = temp->node;
	token->arguments = temp->node->arguments;
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
        node->node->arguments = current->arguments;
        push_back_stack(&node, stack);
        current = current->next;
    }
}
