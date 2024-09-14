#include "minishell.h"
// char get_nfs(char *expanded)
// {
//     int i;

//     i = 0;
//     while (expanded[i])
//     {
//          if(expanded[i])
//     }
// }

// void brace_expansion(Token **token)
// {
//     int i;

//     i = 0;
//     while ((*token)[i])
//     {
//         if((*token)[i] == '$')

//     }
// }

char *get_word_to_expand(char *str)
{
    int     i;
    char    *word;
    int     length;

    i = 0;
    length = 0;
    while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '"' && str[i] != '\'')
    {
        i++;
        length++;
    }
    word = malloc(length + 1); 
    i = 0;
    while(i < length)
    {
        word[i] = str[i]; 
        i++;
    }
    word[i] = '\0';
    return (word);
}


char *expand_single_quote(Token *token)
{
    char	*expanded;
    int		i;
    int 	j;

	j = 0;
    i = 0;
    expanded = malloc(strlen(token->value) - 1);
    if (!expanded)
        return (NULL);
    while (token->value[i])
    {
		if (token->value[i] != '\'')
		{
        	expanded[j] = token->value[i];
			j++;
		}
		i++;  
    }
	expanded[j] = '\0';
	return (expanded);
}

void expand_double_quote(Token *token)
{
    int			i;
	int			type;
	char	*word;

    i = 0;
    while (token->value[i])
    {
        if (token->value[i] == '$')
        {
            word = get_word_to_expand(token->value + i + 1);
			type = get_token_type(word, 0);
            if (type == TOKEN_COMMAND)
            {
                printf("Permission denied.\n");
                exit(1);
            }
            if (!getenv(word))
                token->expanded_value = NULL;
            else
                token->expanded_value = getenv(word);
        }
        i++;
    }
}

void expand(Token *tokens)
{
    Token	*temp;
    int		i;

    temp = tokens;
    i = 0;
    while (temp)
    {
        if (temp->type == TOKEN_SINGLE_QUOTED)
			expand_double_quote(temp);
        temp = temp->next;
    }
}
