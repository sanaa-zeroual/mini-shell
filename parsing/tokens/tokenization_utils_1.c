/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:20:22 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/06 21:29:07 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*heredoc_token(char *input, int l)
{
	char	*full_token;
	int		i;

	i = 0;
	full_token = ft_malloc(sizeof(char), l);
	if (!full_token)
		return (NULL);
	while (i < l)
	{
		full_token[i] = input[i];
		i++;
	}
	return (full_token);
}

char	*read_from_stdin(char *result, char *buffer, int *n)
{
	result = ft_strjoin(result, buffer);
	write(0, "> ", 2);
	buffer = get_next_line(STDIN_FILENO);
	*n = *n + ft_strlen(result);
	return (result);
}

void	handle_heredoc(t_token **tokens, char *input, int *i)
{
	char	*delimiter;
	char	*here;
	int		j;
	int		k;

	j = 0;
	delimiter = NULL;
	while (input[j] != ' ' && input[j] != '\t' && input[j] != '\0')
		j++;
	here = ft_strndup(input, j);
	add_token(tokens, TOKEN_REDIR_HERE_DOC, here, i);
	while (input[j] == ' ' || input[j] == '\t')
	{
		j++;
		(*i)++;
	}
	k = j;
	while (input[j] && (input[j] != '>' && input[j] != '<' && input[j] != '|'
			&& input[j] != ' ' && input[j] != '\t'))
		j++;
	delimiter = ft_strndup(input + k, j - k);
	add_token(tokens, DELIMITER, delimiter, i);
}
