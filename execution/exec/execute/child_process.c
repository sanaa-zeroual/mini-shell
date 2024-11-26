/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanaa <sanaa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:40:47 by szeroual          #+#    #+#             */
/*   Updated: 2024/11/26 15:04:41 by sanaa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	child_process(t_cmd *token, int pipe_nb, int btn, t_mini *env)
{
	(void)pipe_nb;
	g_var->last_child_id = fork();
	if (g_var->last_child_id == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	if (g_var->pre_pipe_infd != -1)
		dup2(g_var->pre_pipe_infd, STDIN_FILENO);
	if (token->pipe_fd[1] > 2)
		dup2(token->pipe_fd[1], STDOUT_FILENO);
	if (g_var->pre_pipe_infd > 2)
		close(g_var->pre_pipe_infd);
	if (token->pipe_fd[0] > 2)
		close(token->pipe_fd[0]);
	if (token->pipe_fd[1] > 2)
		close(token->pipe_fd[1]);
	handle_file_redirections(token, btn);
	execs(token, btn, env);
		exit(0);
	}
}

void child(t_cmd *cmd, int pipe_nb,int btn,  t_mini *box)
{
    if(g_var->last_child_id == 0)
    {
        if(g_var->pre_pipe_infd != - 1)
            dup2(g_var->pre_pipe_infd, STDIN_FILENO);
        if(pipe_nb < g_var->size - 1 && cmd->pipe_fd[1] > 2)
            dup2(cmd->pipe_fd[1], STDOUT_FILENO);
        handle_file_redirections(cmd, btn);
        execs(cmd, btn, box);
        exit(0);        
    }
}

void sig_wait(t_cmd *cmd)
{
    int status;
    while (cmd)
    {
        waitpid(cmd->pid, &status, 0);
        if (WIFSIGNALED(status))
            g_var->exit_status = 128 + WTERMSIG(status);
        if (WIFEXITED(status))
            g_var->exit_status = WEXITSTATUS(status);
        cmd = cmd->next;
    }
}
