/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 13:23:37 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/04 17:09:41 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_validate.h"

static t_status	open_texture(
	t_game *game, t_texture *texture, t_ident_line input)
{
	input.line = ft_through_space(input.line);
	texture->img = mlx_xpm_file_to_image(
		game->mlx, input.line, &(texture->width), &(texture->height));
	if (texture->img != NULL)
	{
		texture->addr = mlx_get_data_addr(
			texture->img, &(texture->bits_per_pixel),
		&(texture->line_length), &(texture->endian));
		return (ENTERED);
	}
	return (FAILURE);
}

t_status		valid_texture_path(t_game *game, t_ident_line input)
{
	if (*(input.status) != NOT_ENTERED
	|| game->config.map_flag || ft_strrncmp(input.line, ".xpm", 4))
		return (FAILURE);
	while (*input.line != ' ')
		input.line++;
	if (input.ident == NORTH)
		return (open_texture(game, &(game->config.north_texture), input));
	if (input.ident == SOUTH)
		return (open_texture(game, &(game->config.south_texture), input));
	if (input.ident == WEST)
		return (open_texture(game, &(game->config.west_texture), input));
	if (input.ident == EAST)
		return (open_texture(game, &(game->config.east_texture), input));
	if (input.ident == SPRITE)
		return (open_texture(game, &(game->config.sprite_texture), input));
	return (FAILURE);
}
