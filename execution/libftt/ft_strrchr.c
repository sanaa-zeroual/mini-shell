/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 17:38:41 by szeroual          #+#    #+#             */
/*   Updated: 2023/12/08 16:31:10 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*last_c;

	last_c = NULL;
	while (*s)
	{
		if (*s == (char)c)
		{
			last_c = s;
		}
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return ((char *)last_c);
}
