/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file_errors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-20 10:38:44 by szeroual          #+#    #+#             */
/*   Updated: 2024-11-20 10:38:44 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	check_file_errors(char *path, int builtin)
{
	if (path && (path[0] == '$' || (path[0] == '"' && path[1] == '$')))
	{
		g_var->red_error = 1;
		g_var->exit_status = 1;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(" ambiguous redirect\n", 2);
		if (builtin)
			return (1);
		else
			exit(1);
	}
	return (0);
}