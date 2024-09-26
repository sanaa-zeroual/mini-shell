#include "../minishell.h"

Token *create_token(char *type, char *value)
{
    Token *token = (Token *)malloc(sizeof(Token));
    token->type = strdup(type);
    token->value = strdup(value);
    token->next = NULL;
    return token;
}

Token *peek_stack(t_stack *stack)
{
    if (!stack)
        return NULL;
    return create_token(stack->token->type, stack->token->value);
}

t_ast *generate_Ast(Token *tokens)
{

	Token *current_token = tokens;
	t_stack *operator_stack = NULL;
	t_queue *output_queue = NULL;
	t_queue *queue;
	t_ast *ast_stack = NULL;
	ASTNode *node;
	while (current_token)
	{
	    if (current_token->type == 	TOKEN_COMMAND || current_token->type == TOKEN_UNKNOWN
		|| current_token->type == TOKEN_OPTION || current_token->type == TOKEN_ARGUMENT)
		{
	        output_queue = push_queue(output_queue, current_token);
	    } else if (current_token->type == TOKEN_OPEN_PARENTH)
		{
	        operator_stack = push_stack(operator_stack, current_token);
	    } else if (current_token->type == TOKEN_CLOSE_PARENTH)
		{
	        while (peek_stack(operator_stack)
		&& peek_stack(operator_stack)->type != TOKEN_OPEN_PARENTH)
		{
	            output_queue = push_queue(output_queue,
			pop_stack(&operator_stack));
	        }
	        pop_stack(&operator_stack);
	    } else if (is_operator(current_token)) {
	        while (peek_stack(operator_stack)
		&& is_operator(peek_stack(operator_stack)) &&
	               get_precedence(peek_stack(operator_stack)) >= get_precedence(current_token)) {
	            output_queue = push_queue(output_queue,
			pop_stack(&operator_stack));
	        }
	        operator_stack = push_stack(operator_stack, current_token);
	    }
	    current_token = current_token->next;
	}
	while (peek_stack(operator_stack))
	    output_queue = push_queue(output_queue, pop_stack(&operator_stack));
	queue = output_queue;
	while (queue)
	{
	    if (is_operator(queue->token))
		{
	        ASTNode *right;
	        ASTNode *left;
	        right = pop_ast(&ast_stack);
	        left = pop_ast(&ast_stack);
	        node = create_ast_node(queue->token->type, queue->token->value,
			left, right);
	        push_ast_stack(&ast_stack, node);
	    } else
		{
	        node = create_ast_node(queue->token->type, queue->token->value,
			NULL, NULL);
	        push_ast_stack(&ast_stack, node);
	    }
	    queue = queue->next;
	}
	node = pop_ast(&ast_stack);
	free_tokens(*tokens);

	return ast_stack;
}