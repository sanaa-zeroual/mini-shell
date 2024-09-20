# include "minishell.h"

void error_handling(int exit_status)
{
	if (exit_status == '1')
		printf("zsh: parse error near `|'");
	else if (exit_status == 130)
		printf("zsh: Error");
	exit (exit_status);
}


Token *get_last_token(Token *token)
{
	if (token == NULL)
		return NULL;
	while (token->next != NULL)
		token = token->next;
	return (token);
}

int check_quote(char *str, char c)
{
	int count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	if (!(count % 2))
		return (1);
	return (0);
}

int handle_quotes(Token *tokens)
{
	while (tokens)
	{
		if (!check_quote(tokens->value, '"') || !check_quote(tokens->value, '\''))
		{
			printf ("Syntax Error: unclosed quoted\n");
			return (1);
		}
		tokens = tokens->next;
	}
	return (0);
}

int handle_parentheses(Token *token)
{
	Token *current;
    int open_parentheses;
	
	current = token;
	while(current)
	{
		if(!strcmp("()", token->value))
		{
			printf("Syntax Error.\n");
			return (1);
		}
		current = current->next;
	}
	current = token;
	open_parentheses = 0;
	if (current->type == TOKEN_OPEN_PARENTH)
        open_parentheses++;
    else if (current->type == TOKEN_CLOSE_PARENTH)
        open_parentheses--;
    if (open_parentheses < 0)
    {
        printf("Syntax Error.\n");
        return 1;
    }
	return (0);
}

int handle_consecutive_operator(Token *tokens)
{
    Token *current;
    Token *prev;

    current = tokens->next;
    prev = tokens;
    while (current)
    {
        if ((prev->type == TOKEN_PIPE || prev->type == TOKEN_DOUBLE_PIPE || prev->type == TOKEN_DOUBLE_PIPE ||
             prev->type == TOKEN_DOUBLE_AMP || prev->type == TOKEN_REDIR_OUT ||
             prev->type == TOKEN_REDIR_IN || prev->type == TOKEN_REDIR_APPEND ||
             prev->type == TOKEN_REDIR_HERE_DOC) &&
            (current->type == TOKEN_PIPE || current->type == TOKEN_DOUBLE_PIPE ||
             current->type == TOKEN_DOUBLE_AMP || current->type == TOKEN_REDIR_OUT ||
             current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_APPEND ||
             current->type == TOKEN_REDIR_HERE_DOC))
        {
            printf("Syntax Error.\n");
            return 1;
        }
        prev = current;
        current = current->next;
    }

    return 0;
}


int lst_size(Token *stack)
{
	int size;

	size = 0;
	while (stack)
	{
		size++;
		stack = stack->next;
	}
	return (size);
}

int handle_operators_bg_en(Token *tokens)
{
	Token *last_node;

	last_node = get_last_token(tokens);
	if ((is_operator(tokens) && lst_size(tokens) == 1 && tokens->type != TOKEN_REDIR_HERE_DOC) || (is_operator(last_node) && last_node->type != TOKEN_REDIR_HERE_DOC) || tokens->type == TOKEN_PIPE || tokens->type == TOKEN_DOUBLE_AMP ||tokens->type == TOKEN_DOUBLE_PIPE)
	{
		printf("Syntax Error.\n");
		return (1);
	}
	return (0);
}
//
char *ft_strncpy(char *dest, char *src, int n)
{
	int i;

	i = 0;
    while (i < n) 
	{
        dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
    return (dest);
}

int check_token(char *str, char c)
{
    int		i;
    int		j;
    char	*copied;
	int		result;

	i = 1;
	j = 0;
    while (str[i] && str[i] != c)
    {
        i++;
        j++;
    }
    copied = malloc(j);
    if (copied == NULL)
        return 0;
    ft_strncpy(copied, str + 1, j);
    copied[j] = '\0';
	if(get_token_type(copied, 0) == TOKEN_COMMAND || get_token_type(copied, 0) == TOKEN_ARGUMENT || get_token_type(copied, 0) == TOKEN_UNKNOWN || get_token_type(copied, 0) == TOKEN_OPTION)
    	result = 0;
	else
		result = 1;
    free(copied);
    return result;
}

char quote_type(const char *str)
{
	char c;
	int i;

	i = 0;
	c = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
	{
		i++;
	}
	if (str[i] == '"' || str[i] == '\'')
		c = str[i];
	return (c);
}

void update_tokens(Token **tokens)
{
    Token	*current;
    char	quote;

	current = *tokens;
    while (current)
    {
        if (current->type == TOKEN_DOUBLE_QUOTED || current->type == TOKEN_SINGLE_QUOTED)
        {
            quote = quote_type(current->value);
            if (check_token(current->value, quote))
            {
                current->value = ft_strtrim(current->value, char_to_string(quote, 0));
			}
        }
        current = current->next;
    }
}

int ft_strchr_sec(char *string, char c)
{
	int i;

	i = 0;
	while(string[i])
	{
		if(string[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int is_rederection(int token_type)
{
	if (token_type == TOKEN_REDIR_APPEND || token_type == TOKEN_REDIR_IN || token_type == TOKEN_REDIR_OUT || token_type == TOKEN_REDIR_HERE_DOC)
		return (1);
	return (0);
}

int random_case(Token *tokens)
{
	while (tokens)
	{
		if ((is_rederection(tokens->type) && tokens->type != TOKEN_REDIR_HERE_DOC) && (ft_strchr_sec(tokens->next->value,'(') || ft_strchr_sec(tokens->next->value,')')))
		{
			printf ("Syntax Error.\n");
			return (1);
		}
		tokens = tokens->next;
	}
	return (0);
}

int check_syntax_errors(Token *tokens)
{
	if (handle_quotes(tokens))
		return(1);
	// update_tokens(&tokens);
	if (handle_parentheses(tokens))
		return(2);
	if (handle_operators_bg_en(tokens))
		return (5);
    // printf("check\n");
	if (handle_consecutive_operator(tokens))
		return (4);
	if (random_case(tokens)) //> hola(ls && pwd)  
		return (2);
	return (0);
}