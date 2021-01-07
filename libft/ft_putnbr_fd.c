/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/11 03:29:16 by totaisei          #+#    #+#             */
/*   Updated: 2020/10/19 12:35:13 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (0 == n)
		write(fd, "0", 1);
	else if (-2147483648 == n)
		write(fd, "-2147483648", 11);
	else
	{
		if (n < 0)
		{
			n *= -1;
			write(fd, "-", 1);
		}
		if (n / 10 != 0)
		{
			ft_putnbr_fd(n / 10, fd);
		}
		write(fd, &"0123456789"[n % 10], 1);
	}
}

void	ft_putnbr_ui_fd(unsigned int n, int fd)
{
	if (0 == n)
		write(fd, "0", 1);
	else
	{
		if (n / 10 != 0)
		{
			ft_putnbr_fd(n / 10, fd);
		}
		write(fd, &"0123456789"[n % 10], 1);
	}
}
