/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 20:25:49 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/06 21:33:09 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_consecutive_operator(t_token *tokens)
{
	t_token	*current;
	t_token	*prev;

	current = tokens->next;
	prev = tokens;
	while (current)
	{
		if ((is_red(prev) && current->type == TOKEN_PIPE) || (is_red(prev)
				&& is_red(current)) || (prev->type == TOKEN_PIPE
				&& current->type == TOKEN_PIPE))
		{
			printf("Syntax_here Error.\n");
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

int	lst_size(t_token *stack)
{
	int	size;

	size = 0;
	while (stack)
	{
		size++;
		stack = stack->next;
	}
	return (size);
}

int	handle_operators_bg_en(t_token *tokens)
{
	t_token	*last_node;

	last_node = get_last_token(tokens);
	if ((is_operator(tokens) && lst_size(tokens) == 1
			&& tokens->type != TOKEN_REDIR_HERE_DOC) || (is_operator(last_node)
			&& last_node->type != TOKEN_REDIR_HERE_DOC
			&& last_node->type != DELIMITER) || tokens->type == TOKEN_PIPE)
	{
		printf("Syntax Error.\n");
		return (1);
	}
	return (0);
}

int	check_token(char *str, char c)
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
	copied = ft_malloc(sizeof(char), j);
	if (copied == NULL)
		return (0);
	ft_strncpy(copied, str + 1, j);
	copied[j] = '\0';
	if (get_token_type(copied, 0) == TOKEN_COMMAND || get_token_type(copied,
			0) == TOKEN_ARGUMENT || get_token_type(copied, 0) == TOKEN_UNKNOWN
		|| get_token_type(copied, 0) == TOKEN_OPTION)
		result = 0;
	else
		result = 1;
	return (result);
}

char	quote_type(const char *str)
{
	char	c;
	int		i;

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
