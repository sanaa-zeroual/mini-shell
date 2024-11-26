/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check-cmd_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-20 10:36:17 by szeroual          #+#    #+#             */
/*   Updated: 2024-11-20 10:36:17 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	check_cmd_path(t_cmd *cmd)
{
	struct stat	statbuf;

	if (stat(cmd->arguments[0], &statbuf) == 0)
	{
		if (S_ISREG(statbuf.st_mode) && (statbuf.st_mode & S_IXUSR))
		{
			cmd->cmd_path = cmd->arguments[0];
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->arguments[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			g_var->exit_status = 1;
		}
	}
	else
	{
	    ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->arguments[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		g_var->exit_status = 1;
	}
}

int	check_path(char *path, int builtin)
{
	struct stat	statbuf;
	int			i;
	char		*folders;
	int			status;

	if (builtin)
		return (1);
	if (!ft_strchr(path, '/'))
		return (1);
	i = ft_strlen(path);
	while (i != 0 && path[i] != '/')
		i--;
	folders = allocate_folders(path, i);
	status = (stat(folders, &statbuf) != -1);
	free(folders);
	return (status);
}
