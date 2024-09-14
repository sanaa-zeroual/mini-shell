/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szeroual <szeroual@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 22:12:25 by szeroual          #+#    #+#             */
/*   Updated: 2023/12/08 16:30:14 by szeroual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int nb, int fd)
{
	if (fd < 0)
		return ;
	if (nb == -2147483648)
	{
		write(fd, "-2147483648", 11);
	}
	else if (nb < 0)
	{
		ft_putchar_fd('-', fd);
		nb = -nb;
		ft_putnbr_fd(nb, fd);
	}
	else if (nb >= 0 && nb <= 9)
	{
		ft_putchar_fd(nb + '0', fd);
	}
	else if (nb > 9)
	{
		ft_putnbr_fd(nb / 10, fd);
		ft_putnbr_fd(nb % 10, fd);
	}
}
