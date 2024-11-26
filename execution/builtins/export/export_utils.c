/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 23:02:01 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/18 23:12:41 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	swap_nodes(t_envi *a, t_envi *b)
{
	char	*tmp_name;
	char	*tmp_vale;

	tmp_name = a->name;
	tmp_vale = a->vale;
	a->name = b->name;
	a->vale = b->vale;
	b->name = tmp_name;
	b->vale = tmp_vale;
}

int	ft_utils(char *ptr)
{
	int	i;

	i = 0;
	if (!isalpha(ptr[0]) && ptr[0] != '_')
		return (0);
	while (ptr[++i] && ptr[i] != '=')
	{
		if (ptr[i] == '+')
			break ;
		else if (!isalnum(ptr[i]) && ptr[i] != '_')
			return (0);
	}
	if (ptr[i] && ptr[i] == '+' && ptr[i + 1] != '=')
		return (0);
	return (1);
}

void	add_back(t_envi **node, t_envi *new)
{
	t_envi	*last;

	if (!*node)
	{
		*node = new;
		return ;
	}
	last = *node;
	while (last->next)
		last = last->next;
	last->next = new;
	new->prv = last;
}

t_envi	*add_new_var(char *name, char *vale)
{
	t_envi	*lst;

	lst = malloc(sizeof(t_envi));
	if (!lst)
	{
		perror("allocation error");
		return (NULL);
	}
	lst->name = ft_strdup(name);
	lst->vale = ft_strdup(vale);
	lst->next = NULL;
	lst->prv = NULL;
	return (lst);
}

t_envi	*cpy_list(t_envi *env)
{
	t_envi	*new_list;

	new_list = NULL;
	while (env)
	{
		add_back(&new_list, add_new_var(env->name, env->vale));
		env = env->next;
	}
	return (new_list);
}
