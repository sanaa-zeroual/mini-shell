#include "minishell.h"

void print_ast(t_ast *ast, int depth)
{
    int i;

    i = 0;
    if (ast == NULL)
        return;
    while (i < depth)
    {
        printf("    ");
        i++;
    }
    if (ast->node && ast->node->token)
        printf("Token: %s (Type: %d)\n", ast->node->token->value, ast->node->token->type);
    else
        printf("NULL Token\n");
    print_ast(ast->left, depth + 1);
    print_ast(ast->right, depth + 1);
}

t_queue *push_queue(t_queue *queue, Token *node)
{
    t_queue *new_node;
    t_queue *temp;

    new_node = malloc(sizeof(t_queue));
    if (!new_node)
        return (NULL);
    new_node->node = node;
    new_node->next = NULL;
    if (!queue)
        return new_node;
    temp = queue;
    while (temp->next)
        temp = temp->next;
    temp->next = new_node;
    return queue;
}
  
t_ast *push_to_ast_stack(t_ast *ast_stack, t_ast *ast_node)
{
    ast_node->next = ast_stack;
    return (ast_node);
}

t_ast *pop_ast_stack(t_ast **ast_stack)
{
    t_ast *head = *ast_stack;
    if (head)
        *ast_stack = head->next;
    return head;
}

void print_queue(t_queue *queue)
{
    while (queue)
    {
        printf("queue_data = %s\n", queue->node->value);
        queue = queue ->next;
    }
}

void print_tokens(t_parser *tokens)
{
    int i;

    while (tokens)
    {
        printf ("=>data = %s type = %d\n", (tokens)->token->value, (tokens)->token->type);
        i = 0;
        if (tokens->arguments)
        {
            while (tokens->arguments[i])
            {
                printf("===>argument = %s type = %d\n", tokens->arguments[i]->value , tokens->arguments[i]->type);
                i++;
            }
        }
        (tokens) = (tokens)->next;
    }
}
void ft_clean(t_queue *queue, char *input,t_ast *ast, t_parser *parser)
{
    if (queue)
        free(queue);
    if (ast)
        free(ast);
    if (parser)
        free(parser);
    if (input)
        free(input);
    handle_ctrl_c();
}

int	main(void)
{
	char	    *input;
	Token	    **tokens;
    t_queue     *queue;
    t_ast       *ast;
    // int         errno;
    t_parser    *parsed;

	tokens = NULL;
	while (1)
	{
		handle_signal();
		input = readline("Minishell$ ");
		if (!input)
			break ;
        add_history(input);
		tokens = tokenize(input);
        // while(*tokens)
        // {
        //     printf("token->data = %s token->type = %d\n",(*tokens)->value, (*tokens)->type);
        //     (*tokens) = (*tokens)->next;
        // }
        // exit(1);
        // errno = check_syntax_errors(*tokens);
        // if (errno)
        //     main();
        // expand(*tokens);
        parsed = analyse_tokens(tokens);
        queue = generate_postfix(parsed);
        print_queue(queue);
        exit(1);
		ast = generate_ast_from_postfix(queue);
		print_ast(ast, 5);
	}
	return (0);
}
// print_tokens(parsed);
// exit(1);

// print_tokens(parsed);
// exit(1);