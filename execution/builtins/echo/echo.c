/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 15:54:46 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/17 15:54:47 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	is_n_option(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-')
		i++;
	else
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	first_non_option(char **args)
{
	int	i;

	i = 1;
	while (args[i] && is_n_option(args[i]))
		i++;
	return (i);
}

int	ft_echo(char **args)
{
	int	i;

	if (!args[1])
	{
		printf("\n");
		return (0);
	}
	i = first_non_option(args);
	while (args && args[i])
	{
		printf("%s", args[i]);
		if (args[++i])
			printf(" ");
	}
	if (!is_n_option(args[1]) || !args[1])
		printf("\n");
	return (0);
}
