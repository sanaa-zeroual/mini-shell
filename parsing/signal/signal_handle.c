/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 20:40:55 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/06 21:21:56 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_ctrl_c(void)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_ctrl_d(void)
{
	clean_gc();
	exit(127);
}

void	handle_signal(void)
{
	signal(SIGINT, (__sighandler_t)handle_ctrl_c);
	signal(SIGTERM, (__sighandler_t)handle_ctrl_d);
}
