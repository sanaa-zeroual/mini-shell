/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 21:39:01 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/09 13:26:04 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	fill_up_node(t_cmd **node, t_token **tokens, t_file **head)
{
	(*node)->file = ft_malloc(sizeof(t_file), 1);
	(*node)->file->type = get_red_type(*tokens);
	if ((*tokens)->next)
	{
		*tokens = (*tokens)->next;
		(*node)->file->filename = ft_strdup((*tokens)->expanded_value[0]);
		(*node)->file->next = NULL;
		push_t_file(head, (*node)->file);
		*tokens = (*tokens)->next;
	}
}

char	*get_inside_quote(char *tmp, int *i, int *j)
{
	char	c;

	c = tmp[(*i)++];
	while (tmp[*i] && tmp[*i] != c)
		(*i)++;
	if (tmp[*i] == c)
		(*i)++;
	return (ft_strndup(tmp + *j + 1, (*i) - (*j) - 2));
}

void	go_to_next(t_token **tokens)
{
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
		(*tokens) = (*tokens)->next;
}

int	is_quoted(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

char	*parse_line(char *input)
{
	char	*result;

	result = expand_non_operator(input);
	return (result);
}
