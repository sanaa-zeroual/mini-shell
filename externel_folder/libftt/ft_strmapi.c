/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 21:02:27 by szeroual          #+#    #+#             */
/*   Updated: 2023/12/08 16:30:57 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	s_len;
	char	*result;
	size_t	i;

	i = 0;
	if (s == NULL || f == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	result = (char *)malloc(s_len + 1);
	if (result != NULL)
	{
		while (i < s_len)
		{
			result[i] = f(i, s[i]);
			i++;
		}
		result[s_len] = '\0';
	}
	return (result);
}
