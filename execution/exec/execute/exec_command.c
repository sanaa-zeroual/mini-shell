/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-20 10:41:37 by szeroual          #+#    #+#             */
/*   Updated: 2024-11-20 10:41:37 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../minishell.h"

void execs(t_cmd *cmd, int btn, t_mini *box) 
{
    if (btn != -1)
    {
        exec_builtin(btn, cmd, box);
        exit(0);
    }
    if (cmd->cmd_path)
    {
        execve(cmd->cmd_path, cmd->arguments, NULL);
        perror(cmd->cmd_path);
        exit(errno);
    }
}

void execute_arguments(t_cmd *cmd, t_mini *box)
{
    g_var->size = count_commands(cmd);
    g_var->pipe_nb = g_var->size - 1;
    g_var->exit_status = 0;
    g_var->pre_pipe_infd = -1;
    int i;
    i = 0;
    while (cmd && g_var->exit_status == 0) 
    {
        execute_pipes(cmd, i, box);
        cmd = cmd->next;
        i++;
    }
    if (g_var->pre_pipe_infd > 2)
        close(g_var->pre_pipe_infd);
    sig_wait(cmd);
}
