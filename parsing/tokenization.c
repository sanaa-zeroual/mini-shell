#include "../minishell.h"

Token *create_token(TokenType type, const char *value)
{
    Token *token;
	
	token = malloc(sizeof(Token));
    if (!token)
	{
        printf("Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    token->type = type;
    token->value = strdup(value);
    token->next = NULL;
    return token;
}

void add_token(Token **tokens, TokenType type, const char *value)
{
	Token	*new_node;
	Token	*ptr;

	new_node = create_token(type, value);
    if (!(*tokens))
        *tokens = new_node;
    else
	{
        ptr = *tokens;
        while (ptr->next)
            ptr = ptr->next;
        new_node->previous = ptr;
        ptr->next = new_node;
    }
}

void free_tokens(Token *tokens)
{
    Token *current;
    Token *next;

	current = tokens;
    while (current)
	{
        next = current->next;
        free(current->value);
        free(current);
        current = next;
	}
}

int ft_is_separator(char c)
{
	if(c == '>' || c == '<' || c == '\n' || c == '\t' ||c == '|' || c == ' ' || c == '\t')
	    return 1;
	return (0);
}

char *handle_Parentheses(char *str, char c)
{
    int i = 1;
    int j = 0;
    char *word;

    while (str[i] && !isspace(str[i]))
    {
        if (str[i] == '\\' && str[i + 1] == c)
            i += 2;
        else
            i++;
        j++;
    }
    word = malloc(j + 1);
    if (!word)
    {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    word[0] = c;
    strncpy(word + 1, str + 1, j);
    word[j] = '\0';
    return word;
}

// char *handle_quote(char *str)
// {
//     int     i;
//     char    *word;

//     i = 0;
//     while (str[i])
//     {
//         if (str[++i] == '"')
//         {
//             while (str[i] && str[i] != '"')
//                 i++;
//             if (str[i] == '"')
//                 i++;
//         }
//         else if (str[++i] == '\'')
//         {
//             while (str[i] && str[i] != '\'')
//                 i++;
//             if (str[i] == '\'')
//                 i++; 
//         }
//         else
//         {
//             while (str[i] && !ft_is_separator(str[i]))
//                 i++;
//         }
//     }
//     word = strndup(str, i);
//     return word;
// }
char *handle_quote(char *str, char c)
{
	int		i;
    char	*word;

	i = 1;
	if (strchr(str + 1, c))
	{
		while (str[i] && str[i] != c)
			i++;
		while (!ft_is_separator(str[i]))
			i++;
		word = strndup(str, i + 1);
	}
	else
	{
		while (!ft_is_separator(str[i]))
			i++;
		word = strndup(str, i + 1);
	}
    return word;
}

int built_in_checker(const char *str)
{
    if (!strcmp(str,"export") || !strcmp(str,"cd") || !strcmp(str,"pwd") ||
		!strcmp(str,"echo") || !strcmp(str,"unset") || !strcmp(str,"env") || !strcmp(str,"exit"))
        return (1);
    return (0);
}

int get_token_type(const char *token, char c)
{
    if (c)
    {
        if (c == '"')
            return TOKEN_DOUBLE_QUOTED;
        return TOKEN_SINGLE_QUOTED;
    }
    if (built_in_checker(token))
        return TOKEN_BUILT_IN;
    if (get_executable((char *)token))
        return TOKEN_COMMAND;
    if (!strcmp(token, "~"))
        return TOKEN_TILDLE;
    if (!strcmp(token, "<<"))
        return TOKEN_REDIR_HERE_DOC;
    if (!strcmp(token, ">"))
        return TOKEN_REDIR_OUT;
    if (!strcmp(token, "<"))
        return TOKEN_REDIR_IN;
    if (!strcmp(token, ">>"))
        return TOKEN_REDIR_APPEND;
    if (!strcmp(token, "|"))
        return TOKEN_PIPE;
    if (!strcmp(token, "||"))
        return TOKEN_DOUBLE_PIPE;
    if (!strcmp(token, "&&"))
        return TOKEN_DOUBLE_AMP;
    if (!strcmp(token, "(") || !strcmp(token, "[") || !strcmp(token, "{"))
        return TOKEN_OPEN_PARENTH;
    if (!strcmp(token, ")") || !strcmp(token, "]") || !strcmp(token, "}"))
        return TOKEN_CLOSE_PARENTH;
    return TOKEN_UNKNOWN;
}

Token **ft_append_identifier(char *input, Token **token_list, int *tmp, int *j)
{
    int i = 0;
    int k = 0;
    char *value;

    while (input[i] && !ft_is_separator(input[i]))
	{
        if (input[i] == '"' || input[i] == '\'')
		{
            *tmp = 0;
            return token_list;
        }
        i++;
    }
    value = malloc(i + 1);
    if (!value)
	{
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    while (k < i)
	{
        value[k] = input[k];
        k++;
    }
    value[k] = '\0';
    *j = i;
    add_token(token_list, get_token_type(value, 0), value);
    free (value);
    return token_list;
}

char *char_to_string(char c, char c2)
{
    char *string;

    if (!c2)
	{
        string = malloc(2 * sizeof(char));
        if (!string)
		{
            printf("Error: memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        string[0] = c;
        string[1] = '\0';
    }
	else
	{
        string = malloc(3 * sizeof(char));
        if (!string) {
            printf("Error: memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        string[0] = c;
        string[1] = c2;
        string[2] = '\0';
    }
    return string;
}

int ft_strschr(char *string, char *delimiteur, int *l)
{
    int		i;
    char	**splited;

    splited = ft_split(string, '\n');
    i = 0;
    if (!splited)
    {
        printf ("Error\n");
        return (0);
    }
    while (splited[i])
    {
        *l += strlen(splited[i]) ;
        if (!strcmp(splited[i], delimiteur))
            return (1);
        i++;
    }
    return (0);
}

int find_delimiter_in_lines(char *string, char *delimiter, int *l)
{
    int     i;
    char    **splitted;

    splitted = ft_split(string, '\n');
    if (!splitted)
	{
        printf("Allocation Failed\n");
        exit (0);
    }
	i = 0;
    while (splitted[i])
	{
		*l = *l + strlen(splitted[i]);
        if (!strcmp(splitted[i], delimiter))
            return (1);
        i++;
    }
    return (0);
}

char *heredoc_token(char *input, int l)
{
    char	*full_token;
    int		i;

	i = 0;
    full_token = malloc(l * sizeof(char));
    if (!full_token)
        return NULL;
    while (i < l)
	{
        full_token[i] = input[i];
		i++;
	}
	return (full_token);
}

// char *handle_heredoc(char *input, int *n)
// {
//     int		i;
//     int		k = 0;
//     char	*delimiter;
//     int		l = 0;

//     *n = 0;
//     i = 0;
//     while (input[i] && (input[i] == '<' || input[i] == ' ' || input[i] == '\t'))
//         i++;
//     if (ft_is_separator(input[i]))
//     {
//         printf("Syntax Error.\n");
//         exit (1);
//     }
//     k = i;
//     while (input[k] != ' ' && input[k] != '\t' && input[k] != '\n')
//         k++;
//     delimiter = strndup(input + i , k - i);
//     l = k + i;
//     if (!find_delimiter_in_lines(input + k + 1, delimiter, &l))
// 	{
//         printf("Syntax Error.\n");
//         free(delimiter);
//         exit(1);
//     }
//     free(delimiter);
//     return heredoc_token(input, l);
// }

// char *handle_heredoc(char *input, int *n) // until I find how it will be done
// {
//     int		i;
//     int		k = 0;
//     char	*delimiter;
//     char    *buffer;
//     char    *result;

//     *n = 0;
//     i = 0;
//     buffer = strdup(" ");
//     result = strdup("");
//     while (input[i] && (input[i] == '<' || input[i] == ' ' || input[i] == '\t'))
//         i++;
//     k = i;
//     while (input[k] != ' ' && input[k] != '\t' && input[k] != '\n')
//         k++;
//     delimiter = strndup(input + i , k - i);
//     while (strncmp(buffer, delimiter, strlen(delimiter)))
//     {
//         result = ft_strjoin(result, buffer);
//         write(0, "> ", 2);
//         buffer = get_next_line(STDIN_FILENO);
//     }
//     *n = *n + strlen(result);
//     return (result);
// }

char *handle_dollar(char *str)
{
    int i;
    char *word;

    i = 0;
    while (str[i] && !isspace(str[i]))
        i++;
    word = (char *)malloc(i + 1);
    if (!word)
        return NULL;
    word = strncpy(word, str,i);
    return word;
}

// int ft_strlen(char *str)
// {
// 	int i;

// 	i = 0;
// 	while (str[i])
// 		i++;
// 	return (i);
// }


void handle_operators(char *input, Token **tokens, int *j, int *k)
{
	int i;

	i = 0;
	if(input[i] == '>' || input[i] == '<' || input[i] == '|')
	{
		add_token(tokens, get_token_type(char_to_string(input[i], 0), 0), char_to_string(input[i], 0));
        (*j)++;
    }
    else if (input[i] == '>' && input[i + 1] && input[i + 1] == '>')
	{
		add_token(tokens, get_token_type(char_to_string(input[i], 0), 0), char_to_string(input[i], 1));
        (*j) +=2;
	}
    *k = 0;
}

void handle_word(char *input, Token **tokens, int *j, int *k)
{
    int		token_type;
	char	*word;
    int		i;
    char    *str;

    i = 0;
    while (input[i] && !ft_is_separator(input[i]))
    {
        if (input[i] == '"' || input[i]== '\'')
        {
            str = handle_quote(input + i, input[i]);
            i += strlen(str);
        }
        i++;
    }
    word = strndup(input, i);
    token_type = get_token_type(word, 0);
    if ((token_type == 14 || token_type == 11) && *k == 0)
    {
        add_token(tokens, token_type, word);
        *k = 1;
    }
    else
        add_token(tokens, TOKEN_ARGUMENT, word);
    (*j) += strlen(word); 
    free(word);
}


Token **tokenize(char *input)
{
    Token **tokens;
    int i ;
    int k;
    char *word;

	i = 0;
    k = 0;
    tokens = (Token **)malloc(sizeof(Token *));
    if (!tokens)
	{
        printf("Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    *tokens = NULL;
    while (input[i])
    {
        if (input[i] == ' ' || input[i] == '\t')
            i++;
        else if ((input[i] == '\\' && (input[i + 1] == '"' || input[i] == '\''))||(input[i] == '"' || input[i] == '\''))
        {
            word = handle_quote(input + i, input[i]);
            add_token(tokens, get_token_type(word, input[i]), word);
            i += strlen(word);
            free (word);
        }
        // else if (input[i] == '<' && input[i + 1] && input[i+1] == '<')
        // {
        //     word = handle_heredoc(input + i, &k);
        //     add_token(tokens, TOKEN_REDIR_HERE_DOC, word);
        //     i += strlen(word);
        // }
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|' ||
			(input[i] == '>' && input[i + 1] && input[i + 1] == '>'))
			    handle_operators(input + i, tokens, &i, &k);
        else
            handle_word(input + i, tokens, &i, &k);
    }
    return tokens;
}
