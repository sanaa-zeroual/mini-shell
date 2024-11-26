/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd_name.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-20 10:37:38 by szeroual          #+#    #+#             */
/*   Updated: 2024-11-20 10:37:38 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"


char *construct_full_path(char *dir, char *cmd)
{
    char *full_path = malloc(strlen(dir) + strlen(cmd) + 2);
    if (!full_path)
    {
        perror("malloc failed");
        return NULL;
    }
    strcpy(full_path, dir);
    strcat(full_path, "/");
    strcat(full_path, cmd);
    return full_path;
}

int is_executable(char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) == 0 && S_ISREG(statbuf.st_mode) && (statbuf.st_mode & S_IXUSR))
    {
        return 1;
    }
    return 0;
}

void free_path_dirs(char **path_dirs)
{
    int i = 0;
    while (path_dirs[i])
    {
        free(path_dirs[i]);
        i++;
    }
    free(path_dirs);
}

void handle_command_not_found(t_cmd *cmd)
{
    ft_putstr_fd("minishell: command not found: ", 2);
    ft_putstr_fd(cmd->arguments[0], 2);
    ft_putstr_fd("\n", 2);
    g_var->exit_status = 127;
}

void search_command_in_paths(t_cmd *cmd, char **path_dirs)
{
    char *full_path;
    int i = 0;

    while (path_dirs[i])
    {
        full_path = construct_full_path(path_dirs[i], cmd->arguments[0]);
        if (!full_path)
        {
            free_path_dirs(path_dirs);
            return;
        }
        if (is_executable(full_path))
        {
            cmd->cmd_path = full_path;
            free_path_dirs(path_dirs);
            return;
        }
        free(full_path);
        i++;
    }
    handle_command_not_found(cmd);
    free_path_dirs(path_dirs);
}

void check_command_name(t_cmd *cmd)
{
    char *path_env = getenv("PATH");
    if (!path_env)
    {
        handle_command_not_found(cmd);
        return;
    }

    char **path_dirs = ft_split(path_env, ':');
    if (!path_dirs)
    {
        perror("ft_split failed");
        return;
    }

    search_command_in_paths(cmd, path_dirs);
}

