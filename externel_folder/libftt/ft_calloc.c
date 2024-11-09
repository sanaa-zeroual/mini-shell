/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 12:15:32 by szeroual          #+#    #+#             */
/*   Updated: 2023/12/08 16:31:38 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	total_size;
	void	*result;

	if (size != 0 && count > SIZE_MAX / size)
		return (NULL);
	total_size = count * size;
	result = malloc(total_size);
	if (result != NULL)
	{
		ft_memset(result, 0, total_size);
	}
	return (result);
}
