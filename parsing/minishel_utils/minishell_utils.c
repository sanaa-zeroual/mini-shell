/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:52:29 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/06 21:33:30 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_operator(t_token *node)
{
	if (node->type == TOKEN_DOUBLE_QUOTED || node->type == TOKEN_SINGLE_QUOTED
		|| node->type == TOKEN_COMMAND || node->type == TOKEN_OPTION
		|| node->type == TOKEN_BUILT_IN || node->type == TOKEN_UNKNOWN
		|| node->type == TOKEN_ARGUMENT)
		return (0);
	return (1);
}

int	is_operand(t_token *node)
{
	if (node->type == TOKEN_UNKNOWN || node->type == TOKEN_DOUBLE_QUOTED
		|| node->type == TOKEN_SINGLE_QUOTED || node->type == TOKEN_COMMAND
		|| node->type == TOKEN_BUILT_IN || node->type == TOKEN_ARGUMENT
		|| node->type == TOKEN_OPTION)
		return (1);
	return (0);
}
