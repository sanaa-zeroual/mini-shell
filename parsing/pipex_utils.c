#include "../minishell.h"

char	*find_command_path(char **dir, char *command) // work
{
	char	*full_path;
	int		i;

	i = 0;
	while (dir[i])
	{
		full_path = malloc(strlen(dir[i]) + strlen(command) + 2);
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
		free(full_path);
		i++;
	}
	return (NULL);
}

void check_single_quote(char **str)
{
    if (*str[0] == 39 && *str[strlen(*str) - 1] == 39)
        *str = ft_strtrim(*str, "'");
}

void clean_arr(char **arr)
{
	int i;

	i = 0;
	if (arr)
	{
		while(arr[i])
		{
			free(arr[i]);
			i++;
		}
		free(arr);
	}
}

char	*get_executable(char *command)
{
	char	*command_path;
	char	**dir;
	char	**arr;
    char    *path;

    path = getenv("PATH");
	arr = ft_split(command, ' ');
    check_single_quote(&arr[0]);
	if (access(arr[0], X_OK) == 0)
	{
		command_path = strdup(arr[0]);
		clean_arr(arr);
		return (command_path);
	}
	dir = ft_split(path, ':');
	command_path = find_command_path(dir, command);
	clean_arr(dir);
	clean_arr(arr);
	return (command_path);
}
// int main(int argc, char **argv)
// {
//     (void)argc;
// 	char *path = get_executable(argv[1]);
//     printf("executable of %s is: %s\n", argv[1], path);
// 	free(path);
// }