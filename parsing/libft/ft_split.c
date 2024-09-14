/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 13:03:44 by shebaz            #+#    #+#             */
/*   Updated: 2024/08/10 12:18:47 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_len(char const *s, char c, int *i)
{
	int	count;

	count = 0;
	while (s[*i] == c)
		*i = *i + 1;
	while (s[*i])
	{
		if (s[*i] != c)
			count ++;
		if (s[*i] == c)
			return (count);
		*i = *i + 1;
	}
	return (count);
}

int	ft_counter(char *str, char c)
{
	int	i;
	int	word_counter;

	i = 0;
	word_counter = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			word_counter++;
			while (str[i + 1] == c)
			{
				i++;
			}
		}
		i++;
	}
	if (str[0] != c && str[strlen(str) - 1] != c)
		word_counter++;
	if (str[0] == c && str[strlen(str) - 1] == c)
		word_counter--;
	return (word_counter);
}

static char	**ft_allocation(char *s, char c)
{
	int		i;
	int		k;
	char	**strings;

	i = 0;
	k = 0;
	strings = (char **)malloc((ft_counter(s, c) + 1) * sizeof(char *));
	if (!strings)
		return (NULL);
	while (k < ft_counter(s, c))
	{
		strings[k] = malloc((count_len(s, c, &i) + 1) * sizeof(char));
		if (!strings[k])
		{
			while (k > 0)
				free(strings[--k]);
			free(strings);
			return (NULL);
		}
		k++;
	}
	return (strings);
}

static char	**ft_copy(char **strings, char *s, char c)
{
	int	k;
	int	i;
	int	j;

	k = 0;
	i = 0;
	while (s[i] && k < ft_counter(s, c))
	{
		j = 0;
		while (s[i] == c)
			i++;
		while (s[i] != c && s[i])
		{
			strings[k][j] = s[i];
			i++;
			j++;
		}
		strings[k][j] = '\0';
		k++;
	}
	strings[k] = NULL;
	return (strings);
}

char	**ft_split(char *s, char c)
{
	char	**strings;

	if (s == NULL)
		return (NULL);
	if (strlen(s) == 0)
	{
		strings = malloc(1 * sizeof(char *));
		if (!strings)
			return (NULL);
		strings[0] = NULL;
		return (strings);
	}
	strings = ft_allocation(s, c);
	if (!strings)
		return (NULL);
	strings = ft_copy(strings, s, c);
	return (strings);
}