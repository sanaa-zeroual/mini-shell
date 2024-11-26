/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 23:04:40 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/18 23:12:14 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	use_while_env(t_envi *new_env)
{
	int		is_swap;
	t_envi	*curr;

	while (1)
	{
		is_swap = 0;
		curr = new_env;
		while (curr->next != NULL)
		{
			if (ft_strncmp(curr->name, curr->next->name,
					ft_strlen(curr->name)) > 0)
			{
				swap_nodes(curr, curr->next);
				is_swap = 1;
			}
			curr = curr->next;
		}
		if (!is_swap)
			break ;
	}
}

t_envi	*sort_env(t_envi *env)
{
	t_envi	*new_env;

	if (!env)
	{
		return (NULL);
	}
	new_env = cpy_list(env);
	if (!new_env)
	{
		perror("Failed to copy environment");
		return (NULL);
	}
	use_while_env(new_env);
	return (new_env);
}

t_envi	*create_env_node(char *name, char *value)
{
	t_envi	*new_node;

	if (!name)
		return (NULL);
	new_node = malloc(sizeof(t_envi));
	if (!new_node)
	{
		perror("error");
		exit(0);
	}
	new_node->name = ft_strdup(name);
	new_node->vale = ft_strdup(value);
	new_node->next = NULL;
	return (new_node);
}

t_envi	*find_last_node(t_envi *env)
{
	t_envi	*curr;

	curr = env;
	while (curr->next)
		curr = curr->next;
	return (curr);
}

void	add_env_variable(t_envi **env, char *name, char *value)
{
	t_envi	*new_node;
	t_envi	*last;

	new_node = create_env_node(name, value);
	if (!*env)
	{
		*env = new_node;
		return ;
	}
	last = find_last_node(*env);
	last->next = new_node;
	new_node->prv = last;
}
