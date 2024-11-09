/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:36:37 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/07 13:44:39 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_operators(char *input, t_token **tokens, int *j, int *k)
{
	char	*str1;
	char	*str2;
	int		i;

	i = 0;
	str2 = NULL;
	str1 = char_to_string(input[i], 0);
	if (input[i] == '>' && input[i + 1] && input[i + 1] == '>')
	{
		str2 = char_to_string(input[i], 1);
		add_token(tokens, get_token_type(str2, 0), str2, j);
	}
	else
		add_token(tokens, get_token_type(str1, 0), str1, j);
	*k = 0;
}

void	handle_word(char *input, t_token **tokens, int *j, int *k)
{
	int		token_type;
	char	*word;
	int		i;
	char	*str;

	i = 0;
	while (input[i] && !ft_is_separator(input[i]))
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			str = handle_quote(input + i);
			i += ft_strlen(str);
		}
		i++;
	}
	word = ft_strndup(input, i);
	token_type = get_token_type(word, 0);
	if ((token_type == TOKEN_COMMAND || token_type == TOKEN_BUILT_IN)
		&& *k == 0)
	{
		add_token(tokens, token_type, word, j);
		*k = 1;
	}
	else
		add_token(tokens, TOKEN_ARGUMENT, word, j);
}

int	is_ope(char *input)
{
	int	i;

	i = 0;
	if (input[i] == '>' || input[i] == '<' || input[i] == '|'
		|| (input[i] == '>' && input[i + 1] && input[i + 1] == '>'))
		return (1);
	return (0);
}

void	processing_data(t_token **tokens, char *input, int *i, int *k)
{
	char	*word;

	word = NULL;
	if (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	else if ((input[*i] == '\\' && (input[(*i) + 1] == '"'
				|| input[*i] == '\'')) || (input[*i] == '"'
			|| input[*i] == '\''))
	{
		word = handle_quote(input + (*i));
		add_token(tokens, get_token_type(word, input[*i]), word, i);
	}
	else if (input[*i] == '<' && input[(*i) + 1] && input[(*i) + 1] == '<')
		handle_heredoc(tokens, input + (*i), i);
	else if (is_ope(input + (*i)))
		handle_operators(input + (*i), tokens, i, k);
	else
		handle_word(input + (*i), tokens, i, k);
}

t_token	**tokenize(char *input)
{
	t_token	**tokens;
	int		i;
	int		k;

	i = 0;
	k = 0;
	tokens = (t_token **)ft_malloc(sizeof(t_token *), 1);
	if (!tokens)
	{
		printf("Error: Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	*tokens = NULL;
	while (input[i])
	{
		processing_data(tokens, input, &i, &k);
	}
	return (tokens);
}
