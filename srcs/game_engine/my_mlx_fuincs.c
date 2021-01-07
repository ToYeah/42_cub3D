/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_mlx_fuincs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 09:03:55 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/04 14:44:47 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_engine.h"

unsigned int	extract_color(t_texture *texture, int x, int y)
{
	return (*(int *)((texture->addr) +
	(y * texture->line_length) + (x * (texture->bits_per_pixel / 8))));
}

void			my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr +
	(y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}
