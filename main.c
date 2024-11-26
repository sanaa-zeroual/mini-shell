/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mnsh.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:53:32 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/19 10:04:23 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_globalvar *g_var = NULL;

void	initiale_global(t_envi *env)
{
	g_var = malloc(sizeof(t_globalvar));
	g_var->exit_status = 0;
	g_var->head = NULL;
	g_var->pre_pipe_infd = -1;
	g_var->envp = env;
	g_var->last_child_id = 0;
	g_var->out_fd = -1;
	g_var->red_error = 0;
	g_var->size = 0;
	g_var->pipe_nb = 0;
}

void error_malloc()
{
    perror("malloc");
    exit(EXIT_FAILURE);
}

t_envi *init_env(char **envp)
{
    t_envi *env_list = NULL;
    int i = 0;

    while (envp[i])
    {
        char *env_entry = strdup(envp[i]);
        if (!env_entry)
            error_strdup();
        process_env_entry(env_entry, &env_list);
        i++;
    }
    return env_list;
}

int main(int argc, char **argv, char **envp)
{
    t_mini *box;

    (void)argc;
    (void)argv;

    box = malloc(sizeof(t_mini));
    if (!box)
        error_malloc();
    initialisation(box, envp);
    shell_loop(box);
    return 0;
}

