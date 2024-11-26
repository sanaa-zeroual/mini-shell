/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:03:34 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/06 21:11:08 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*find_command_path(char **dir, char *command)
{
	char	*full_path;
	int		i;

	i = 0;
	while (dir[i])
	{
		full_path = ft_malloc(sizeof(char), ft_strlen(dir[i])
				+ ft_strlen(command) + 2);
		if (!full_path)
		{
			perror("malloc");
			exit(1);
		}
		strcpy(full_path, dir[i]);
		strcat(full_path, "/");
		strcat(full_path, command);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

char	*get_executable(char *command)
{
	char	*command_path;
	char	**dir;
	char	**arr;
	char	*path;

	path = getenv("PATH");
	arr = ft_split(command, ' ');
	if (access(arr[0], X_OK) == 0)
	{
		command_path = ft_strdup(arr[0]);
		return (command_path);
	}
	dir = ft_split(path, ':');
	command_path = find_command_path(dir, command);
	return (command_path);
}
