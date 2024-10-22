#include "../minishell.h"

void print_ast(t_ast *ast, int depth)
{
    int i;
    int j;

    i = 0;
    if (ast == NULL)
        return;
    while (i < depth)
    {
        printf("    ");
        i++;
    }
    if (ast && ast->data->token)
    {
        j = 0;
        printf ("Token: %s (Type: %d)\n", ast->data->token->value, ast->data->token->type);
        if(ast->data->arguments)
        {
            while (ast->data->arguments[j])
            {
                printf("arg[%d] = %s\n",j, ast->data->arguments[j]);
                j++;
            }
        }
    }
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


// int	main()
// {
//     char	    *input;
//     Token	    **tokens;
//     t_queue     *queue;
//     t_ast       *ast;
//     t_parser    *parsed;

// 	tokens = NULL;
// 	while (1)
// 	{
// 		handle_signal();
// 		input = readline("Minishell$ ");
// 		if (!input)
// 			break ;
//         if (!strlen(input))
//             main();
//         add_history(input);
// 		tokens = tokenize(input);
//         expand(*tokens);
// 		parsed = analyse_tokens(tokens);
// 		queue = generate_postfix(parsed);
// 		ast = generate_ast_from_postfix(queue);
// 		// print_ast(ast,5);
// 	}
// 	return (0);
// }


// execute_ast(ast, envp);

// main();
// exit(1);





        // while(*tokens)
        // {
        //     printf("value = %s\n", (*tokens)->value);
        //     (*tokens) = (*tokens)->next;
        // }
        // exit(1);

        // int	i;
        // while(*tokens)
        // {
        //     i = 0;
        //     printf("=========value = %s\n", (*tokens)->value);
		// 	if ((*tokens)->expanded_value)
		// 	{
		// 		while ((*tokens)->expanded_value[i] != NULL)
		// 		{
		// 			printf("expanded_value = '%s'\n", (*tokens)->expanded_value[i]);
		// 			i++;
		// 		}
		// 	}
        //     (*tokens) = (*tokens)->next;
        // }
        // exit(1);

		
        // errno = check_syntax_errors(*tokens);
        // if (errno)
        //     main();