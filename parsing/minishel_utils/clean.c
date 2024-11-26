/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:39:34 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/13 15:23:53 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	*ft_malloc(size_t size, int ele_nbr)
{
	t_gc	*node;
	void	*ptr;

	ptr = malloc(ele_nbr * size);
	node = malloc(sizeof(t_gc));
	node->ptr = ptr;
	node->next = g_var->head;
	g_var->head = node;
	return (ptr);
}

void	clean_gc(void)
{
	t_gc	*temp;

	while (g_var->head)
	{
		if (g_var->head)
			free(g_var->head->ptr);
		temp = g_var->head;
		g_var->head = g_var->head->next;
		if (temp)
			free(temp);
	}
	free(g_var->head);
	free(g_var);
}
