/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/08 21:48:41 by totaisei          #+#    #+#             */
/*   Updated: 2020/10/08 21:48:44 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*casted_s1;
	unsigned char	*casted_s2;

	casted_s1 = (unsigned char *)s1;
	casted_s2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (casted_s1[i] == casted_s2[i])
			i++;
		else
			return (casted_s1[i] - casted_s2[i]);
	}
	return (0);
}
