#include "minishell.h"

int	main()
{
    char	    *input;
    Token	    **tokens;
    t_queue     *queue;
    t_ast       *ast;
    t_parser    *parsed;

	(void)ast;
	tokens = NULL;
	while (1)
	{
		handle_signal();
		input = readline("Minishell$ ");
		if (!input)
			break ;
        if (!strlen(input))
            main();
        add_history(input);
		tokens = tokenize(input);
        expand(*tokens);
		parsed = analyse_tokens(tokens);
		queue = generate_postfix(parsed);
		ast = generate_ast_from_postfix(queue);
		// print_ast(ast,5);
	}
	return (0);
}