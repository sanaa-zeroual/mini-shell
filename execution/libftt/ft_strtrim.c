/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 13:20:58 by szeroual          #+#    #+#             */
/*   Updated: 2023/12/08 16:31:17 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_check(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*p;
	int		start;
	int		end;

	if (!s1)
		return (NULL);
	if (!set)
		return ((char *)s1);
	start = 0;
	end = ft_strlen(s1) - 1;
	while (ft_check(set, s1[start]))
		start++;
	while ((end > start) && ft_check(set, s1[end]))
		end--;
	p = malloc(end - start + 2);
	if (!p)
		return (NULL);
	ft_strlcpy(p, s1 + start, end - start + 2);
	return (p);
}
