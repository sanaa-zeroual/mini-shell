/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:36:28 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/06 16:06:07 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	built_in_checker(const char *str)
{
	if (!ft_strcmp(str, "export") || !ft_strcmp(str, "cd") || !ft_strcmp(str,
			"pwd") || !ft_strcmp(str, "echo") || !ft_strcmp(str, "unset")
		|| !ft_strcmp(str, "env") || !ft_strcmp(str, "exit"))
		return (1);
	return (0);
}

int	get_token_type(const char *token, char c)
{
	char	*path;

	path = get_executable((char *)token);
	if (c)
	{
		if (c == '"')
			return (TOKEN_DOUBLE_QUOTED);
		return (TOKEN_SINGLE_QUOTED);
	}
	if (built_in_checker(token))
		return (TOKEN_BUILT_IN);
	if (path)
		return (TOKEN_COMMAND);
	if (!ft_strcmp(token, "<<"))
		return (TOKEN_REDIR_HERE_DOC);
	if (!ft_strcmp(token, ">"))
		return (TOKEN_REDIR_OUT);
	if (!ft_strcmp(token, "<"))
		return (TOKEN_REDIR_IN);
	if (!ft_strcmp(token, ">>"))
		return (TOKEN_REDIR_APPEND);
	if (!ft_strcmp(token, "|"))
		return (TOKEN_PIPE);
	return (TOKEN_UNKNOWN);
}

char	*char_to_string(char c, char c2)
{
	char	*string;

	if (!c2)
	{
		string = ft_malloc(sizeof(char), 2);
		if (!string)
		{
			printf("Error: memory allocation failed\n");
			exit(EXIT_FAILURE);
		}
		string[0] = c;
		string[1] = '\0';
		return (string);
	}
	string = ft_malloc(sizeof(char), 3);
	if (!string)
	{
		printf("Error: memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	string[0] = c;
	string[1] = c;
	string[2] = '\0';
	return (string);
}

int	ft_strschr(char *string, char *delimiteur, int *l)
{
	int		i;
	char	**splited;

	splited = ft_split(string, '\n');
	i = 0;
	if (!splited)
	{
		printf("Error\n");
		return (0);
	}
	while (splited[i])
	{
		*l += ft_strlen(splited[i]);
		if (!ft_strcmp(splited[i], delimiteur))
			return (1);
		i++;
	}
	return (0);
}

char	*handle_dollar(char *str)
{
	int		i;
	char	*word;

	i = 0;
	while (str[i] && !isspace(str[i]))
		i++;
	word = ft_malloc(sizeof(char), i + 1);
	if (!word)
		return (NULL);
	word = strncpy(word, str, i);
	return (word);
}
// int	find_delimiter_in_lines(char *string, char *delimiter, int *l)
// {
// 	int		i;
// 	char	**splitted;

// 	splitted = ft_split(string, '\n');
// 	if (!splitted)
// 	{
// 		printf("Allocation Failed\n");
// 		exit(0);
// 	}
// 	i = 0;
// 	while (splitted[i])
// 	{
// 		*l = *l + strlen(splitted[i]);
// 		if (!ft_strcmp(splitted[i], delimiter))
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }