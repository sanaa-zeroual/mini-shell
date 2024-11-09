/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:39:34 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/07 14:36:48 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern t_gc	*g_rt;
t_gc		*g_rt = NULL;

void	*ft_malloc(size_t size, int ele_nbr)
{
	t_gc	*node;
	void	*ptr;

	ptr = malloc(ele_nbr * size);
	node = malloc(sizeof(t_gc));
	node->ptr = ptr;
	node->next = g_rt;
	g_rt = node;
	return (ptr);
}

void	clean_gc(void)
{
	t_gc	*temp;

	while (g_rt)
	{
		if (g_rt->ptr)
			free(g_rt->ptr);
		temp = g_rt;
		g_rt = g_rt->next;
		if (temp)
			free(temp);
	}
}
