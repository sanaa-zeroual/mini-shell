/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanaa <sanaa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 13:07:29 by szeroual          #+#    #+#             */
/*   Updated: 2024/11/26 15:04:38 by sanaa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void error_pipe()
{
	perror("pipe");
	exit(1);
}

void close_files(t_cmd *token)
{
	if (token->pipe_fd[1] > 2)
		close(token->pipe_fd[1]);
	if (g_var->pre_pipe_infd > 2)
		close(g_var->pre_pipe_infd);
}

void red_builtin(t_cmd *token, int btn, t_mini *box)
{
	files_redirections(token, 1);
	exec_builtin(btn, token, box);
}

void	execute_pipes(t_cmd *token, int pipe_nb, t_mini *env)
{
	int btn;
	int original_stdin;

	original_stdin = dup(STDIN_FILENO);
	btn = check_builtin(token);
	if (g_var->size == 1 && btn != -1)
		red_builtin(token, btn, env);
	else
	{
		if (g_var->size != pipe_nb + 1)
		{
			if (pipe(token->pipe_fd) == -1)
				error_pipe();
		}
		child_process(token, pipe_nb, btn, env);
		close_files(token);
		g_var->pre_pipe_infd = token->pipe_fd[0];
		if (g_var->last_child_id > 0)
			waitpid(g_var->last_child_id, NULL, 0);
	}
	close(original_stdin);
}

