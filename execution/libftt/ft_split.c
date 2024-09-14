/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 19:09:51 by szeroual          #+#    #+#             */
/*   Updated: 2023/12/08 16:30:23 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	separate_string(char const *s, char c)
{
	int	i;
	int	wc;

	i = 0;
	wc = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			wc++;
		i++;
	}
	return (wc);
}

static int	ft_free(char **ptr, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
	return (0);
}

static void	str_cpy(char *d, const char *src, char c)
{
	int	i;

	i = 0;
	while (src[i] != c && src[i])
	{
		d[i] = src[i];
		i++;
	}
	d[i] = '\0';
}

static int	allocate_substr(char **ar, const char *str, char c)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (str[i])
	{
		if (str[i] == c)
			i++;
		else
		{
			j = 0;
			while (str[i + j] != c && str[i + j])
				j++;
			ar[k] = malloc(j + 1);
			if (!ar[k])
				return (ft_free(ar, k));
			str_cpy(ar[k], str + i, c);
			i += j;
			k++;
		}
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**ar;
	int		wc;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	wc = separate_string(s, c);
	ar = malloc(sizeof(char *) * (wc + 1));
	if (!ar)
		return (NULL);
	if (allocate_substr(ar, s, c) == 0)
		return (NULL);
	ar[wc] = 0;
	return (ar);
}
