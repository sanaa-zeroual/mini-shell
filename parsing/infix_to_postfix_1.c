#include "minishell.h"

void push_back_stack(t_stack **src, t_stack **dest)
{
	t_stack *node;
	t_stack *tmp;

    if (!src || !*src)
        return;
    node = pop_stack(src);
    if (!node)
        return;
    if (*dest == NULL)
        *dest = node;
    else
    {
        tmp = *dest;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = node;
    }
}

t_stack *create_node(Token *token)
{
    t_stack *node;

    node = malloc(sizeof(t_stack));
    if (!node)
        return (NULL);
    node->node = malloc(sizeof(Token));
    if (!node->node)
    {
        free(node);
    return (NULL);
    }
    node->node->token = token;
    node->next = NULL;
    return node;
}

void enqueue(t_queue **queue, Token *token)
{
    t_queue *new_node;
    t_queue *temp;

    new_node = malloc(sizeof(t_queue));
    new_node->node = token;
    new_node->next = NULL;
    if (*queue == NULL)
        *queue = new_node;
    else
    {
        temp = *queue;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_node;
    }
}

void push_to_stack(t_stack **src, t_stack **dest)
{
    t_stack *head;

    head = pop_stack(src);
    head->next = *dest;
    *dest = head;
}

int get_precedence(int type)
{
    if (type == TOKEN_OPEN_PARENTH || type == TOKEN_CLOSE_PARENTH)
        return (4);
    if (type == TOKEN_DOUBLE_AMP || type == TOKEN_DOUBLE_PIPE)
        return (3);
    if (type == TOKEN_PIPE)
        return (2);
    if (type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE_DOC 
        || type == TOKEN_REDIR_OUT || type == TOKEN_REDIR_IN)
        return (1);
    return (0);
}

int check_precedence(t_stack *stack, int token_type)
{
	while (stack)
	{
		if (get_precedence(token_type) >= get_precedence(stack->node->token->type))
			return (0);
		stack = stack->next;
	}
	return (1);
}

t_queue *generate_postfix(t_parser *tokens)
{
    t_stack *head;
    t_queue *output_queue;
    t_stack *operator_stack;
    t_stack *popped;

    head = NULL;
    output_queue = NULL;
    operator_stack = NULL;
    transfer_tokens_to_stack(tokens, &head);
    while (head)
    {
        if (is_operand(head->node->token))
        {
            enqueue(&output_queue, head->node->token);
            head = head->next;
        }
        else
        {
            if (!operator_stack)
                push_to_stack(&head, &operator_stack);
            else if (operator_stack)
            {
                if (is_operator(head->node->token) && is_operator(operator_stack->node->token) && head->node->token->type != TOKEN_CLOSE_PARENTH)
                {
                    while (operator_stack && (!check_precedence(operator_stack, head->node->token->type)))
                    {
                        popped = pop_stack(&operator_stack);
                        enqueue(&output_queue, popped->node->token);
                    }
                    push_to_stack(&head, &operator_stack);
                }
                if (head->node->token->type == TOKEN_CLOSE_PARENTH)
                {
                    while (operator_stack && operator_stack->node->token->type != TOKEN_OPEN_PARENTH)
                    {
                        popped = pop_stack(&operator_stack);
                        enqueue(&output_queue, popped->node->token);
                    }
                    pop_stack(&operator_stack);
                    head = head->next;
                }
            }
        }
    }
    while (operator_stack)
    {
        popped = pop_stack(&operator_stack);
        enqueue(&output_queue, popped->node->token);
    }
    return output_queue;
}