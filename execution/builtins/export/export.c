/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 21:41:35 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/18 23:36:50 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	check_each_element(char *str)
{
	int	i;

	i = 0;
	if (!ft_strlen(str))
	{
		printf("minishell: export: `%s': not a valid identifier\n", str);
		return (1);
	}
	while (str[i])
	{
		if ((is_special(str[i]) && str[i] != '=') || str[0] == '=')
		{
			printf("minishell: export: `%s': not a valid identifier\n", str);
			return (1);
		}
		i++;
	}
	return (0);
}

void	process_existing_env(t_envi **env, char *arr[2])
{
	t_envi	*tmp;

	tmp = (*env);
	while (tmp)
	{
		if (!strcmp(tmp->name, arr[0]))
			tmp->vale = strdup(arr[1]);
		tmp = tmp->next;
	}
}

int	process_single_env(char *ptr_i, t_envi **env)
{
	char	*arr[2];
	t_envi	*new;
	int		status;
	char	*tmp;

	status = 0;
	tmp = strdup(ptr_i);
	arr[0] = strtok(ptr_i, "=");
	arr[1] = strtok(NULL, "=");
	if (!arr[1] && tmp[strlen(tmp) - 1] != '=')
		return (status);
	else if (!arr[1] && tmp[strlen(tmp) - 1] == '=')
		arr[1] = strdup("");
	if (ft_utils(arr[0]))
	{
		new = search_env(*env, arr[0]);
		if (new)
			process_existing_env(env, arr);
		else
			add_env_variable(env, arr[0], arr[1]);
	}
	return (status);
}

int	add_one(char **ptr, t_envi **env)
{
	int	i;
	int	status;
	int	result;

	i = 1;
	status = 0;
	while (ptr[i])
	{
		if (check_each_element(ptr[i]))
			status = 1;
		else
		{
			result = process_single_env(ptr[i], env);
			if (result)
				status = result;
		}
		i++;
	}
	return (status);
}

int	ft_export(char **ptr, t_envi **env)
{
	t_envi	*newenv;
	int		status;

	status = 0;
	if (!ptr[1])
	{
		newenv = sort_env(*env);
		if (!newenv)
		{
			perror("Failed to sort environment");
			return (1);
		}
		while (newenv)
		{
			printf("declare -x %s", newenv->name);
			if (newenv->vale)
				printf("=\"%s\"", newenv->vale);
			printf("\n");
			newenv = newenv->next;
		}
	}
	else
		status = add_one(ptr, env);
	return (status);
}
