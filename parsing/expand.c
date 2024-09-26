/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 22:25:31 by shebaz            #+#    #+#             */
/*   Updated: 2024/09/25 15:40:25 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_word_to_expand(char *str)
{
	char	*word;
	int		length;
	int		i;

	i = 0;
	length = 0;
	while (str[i] && ((str[i] <= 'z' && str[i] >= 'a') || (str[i] <= 'Z' && str[i] >= 'A') || (str[i] >= '0' && str[i] <= '9')))
	{
		i++;
		length++;
	}
	word = malloc(length + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < length)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	*tidle_expansion(int *i)
{
	char	*result;

	result = getenv("HOME");
	if (!result)
		return (strdup(""));
	*i += 1;
	return (strdup(result));
}

char	*dollar_expand(char *input, int *i)
{
	char	*word;
	char	*result;

	word = get_word_to_expand(input + *i + 1);
	if (!word)
		return (strdup(""));
	result = getenv(word);
	if (!result)
		result = strdup("");
	*i += strlen(word) + 1;
	free(word);
	return (strdup(result));
}

char	*double_quote_expansion(char *input, int *i)
{
	char	*expanded_value;

	expanded_value = strdup("");
	if (!expanded_value)
		return (NULL);
	if (input[*i] == '"')
	{
		expanded_value = ft_strjoin(expanded_value, char_to_string(input[*i], 0));
		(*i)++;
	}
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$')
            expanded_value = ft_strjoin(expanded_value, dollar_expand(input, i));
		else 
		{
            expanded_value = ft_strjoin(expanded_value, char_to_string(input[*i], 0));
            (*i)++;
        }
	}
	if (input[*i] == '"')
	{
		expanded_value = ft_strjoin(expanded_value, char_to_string(input[*i], 0));
		(*i) ++;
	}
	return (expanded_value);
}

char	*single_quote_expansion(char *input, int *i)
{
	char	*expanded_value;

	expanded_value = strdup("");
	if (!expanded_value)
		return (NULL);
	if (input[*i] == '\'')
	{
		expanded_value = ft_strjoin(expanded_value, char_to_string(input[*i], 0));
		(*i) ++;
	}
	while (input[*i] && input[*i] != '\'')
	{
		expanded_value = ft_strjoin(expanded_value, char_to_string(input[*i],
					0));
		(*i)++;
	}
	if (input[*i] == '\'')
	{
		expanded_value = ft_strjoin(expanded_value, char_to_string(input[*i], 0));
		(*i) ++;
	}
	return (expanded_value);
}

char *get_word(char **string, int *counter)
{
	char	*result;
	int		i;
	// int		k;

	i = 0;
	// k = 0;
	if ((*counter) > 1)
	{
		while ((*string)[i] && ((*string)[i] == ' ' || (*string)[i] == '\t'))
			i++;
		while ((*string)[i] && ((*string)[i] != ' ' && (*string)[i] != '\t'))
			i++;
		result = strndup(*string, i);
		(*string) += i;
	}
	else //last word in the string
		result = strndup((*string) , strlen(*string));
	(*counter)--;
	return result;
}
char *handle_quote_up(char *str, char c)
{
	int		i;
    char	*word;

	i = 1;
	if (strchr(str + 1, c))
	{
		while (str[i] && str[i] != c)
			i++;
		while (str[i] && !ft_is_separator(str[i]))
			i++;
		word = strndup(str, i + 1);
	}
	else
	{
		while (str[i] && !ft_is_separator(str[i]))
			i++;
		word = strndup(str, i + 1);
	}
    return word;
}


char *get_output(char *input)
{
	char *word;
	char *str;
	int i;
	int k = 0;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t')
	{
		i++;
		k++;
	}
	i = k;
	while (input[i] && !ft_is_separator(input[i]))
    {
        if (input[i] == '"' || input[i]== '\'')
        {
            str = handle_quote_up(input + i, input[i]);
            i += strlen(str);
        }
        i++;
    }
    word = strndup(input, i);
	return word;
}

char *get_string(char *input, int *i)
{
	char	*word;

	if (input[*i] == '"' || input[*i] == '\'')
	{
		word = handle_quote_up(input + (*i), input[*i]);
		*i += strlen(word);
	}
	else
	{
		word = get_output(input + (*i));
		*i += strlen(word);
	}
	return (word);
}

int get_size_arr(char *input)
{
	int		i;
	char	*word;
	int		size;

	i = 0;
	size = 0;
	while (input[i])
	{
		word = ft_strtrim(get_string(input, &i), " ");
		if (strlen(word) > 0)
			size++;
	}
	return (size);
}

char	**handle_that_shit(char *input)
{
	char	**result;
	int		size;
	int		i;
	int		j;
	
	i = 0;
	j = 0;
	size = get_size_arr(input);
	result = malloc((size + 1) * sizeof(char *));
	if (!result)
		printf("Allocation Failed\n");
	while (input[i])
	{
		result[j] = ft_strtrim(get_string(input, &i), " ");
		j++;
	}
	result[size] = NULL;
	return (result);
}

char *check_input(char *input)
{
	int		i;
	char	*output;

	i = 0;
	output = strdup("");
	while (input[i])
	{
		if (input[i] == '$')
			output = ft_strjoin(output, dollar_expand(input + i, &i));
		else
		{	
			output = ft_strjoin(output, char_to_string(input[i], 0));
			i++;
		}
	}
	return (output);
}
int get_size(char **arr)
{
	int size;

	size = 0;
	while (arr[size])
		size++;
	return (size);
}

char **unquoted_result(char **input)
{
	char	**output;
	int		i;
	int		j;
	char	c;

	i = 0;
	j = 0;
	output = malloc(get_size(input));
	while (input[i])
	{
		j = 0;
		output[i] = strdup("");
		while (input[i][j])
		{
			if (input[i][j] == '"' || input[i][j] == '\'')
			{
				c = input[i][j];
				j++;
				while (input[i][j] != c)
				{
					output[i] = ft_strjoin(output[i], char_to_string(input[i][j], 0));
					j++;
				}
				j++;
			}
			else
			{
				output[i] = ft_strjoin(output[i], char_to_string(input[i][j], 0));
				j++;
			}
		}
		i++;
	}
	return (output);
}

char	**result_traitement(char *input)
{
	// int		i;
	char	**result;

	// i = 0;
	if (!strchr (input, '"') && !strchr(input, '\''))
		result = ft_split(input,' ');
	else
		result = handle_that_shit(input);
	result = unquoted_result(result);
	return result;
}
void	expand(Token *tokens)
{
	char	*result;
	int		i;

	
	while (tokens)
	{
		i = 0;
		result = strdup("");
		if (!result)
			return ;
		tokens->expanded_value = NULL;
		if (!is_operator(tokens))
		{
			while (tokens->value[i])
			{
				if (tokens->value[i] == '"')
					result = ft_strjoin(result,
							double_quote_expansion(tokens->value, &i));
				else if (tokens->value[i] == '\'')
					result = ft_strjoin(result,
							single_quote_expansion(tokens->value, &i));
				else if (tokens->value[i] == '~')
					result = ft_strjoin(result, tidle_expansion(&i));
				else if (tokens->value[i] == '$')
					result = ft_strjoin(result, dollar_expand(tokens->value, &i));
				else
				{
					result = ft_strjoin(result, char_to_string(tokens->value[i],
								0));
					i++;
				}
			}
			tokens->expanded_value = result_traitement(result);
		}
		tokens = tokens->next;
	}
}
