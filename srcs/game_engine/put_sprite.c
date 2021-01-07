/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_sprite.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 08:55:33 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/05 19:55:12 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_engine.h"

static void	draw_sprtite(
	t_game *game, t_vector win_pos, int vwh, double length)
{
	int i;
	int j;
	int color;

	i = win_pos.y < 0 ? -win_pos.y : 0;
	while (i < vwh && (win_pos.y + i) < game->config.window_height)
	{
		j = win_pos.x < 0 ? -win_pos.x : 0;
		while (j < vwh && (win_pos.x + j) < game->config.window_width)
		{
			if (game->collisions[(int)round(win_pos.x + j)]
			.view_length > length)
			{
				color = extract_color(&(game->config.sprite_texture),
				(double)j / vwh * game->config.sprite_texture.width,
				(double)i / vwh * game->config.sprite_texture.height);
				if (color != 0)
					my_mlx_pixel_put(&(game->data),
					(win_pos.x + j), (win_pos.y + i), color);
			}
			j++;
		}
		i++;
	}
}

static void	put_one_sprite(
	t_game *game, t_vector sprite_pos, double angle, double distance)
{
	double		ratio;
	int			vwh;
	double		sprite_view_length;
	t_vector	win_pos;

	ratio = calc_angle_diff(game, angle, TRUE) / game->fov;
	if (ratio > 2)
		ratio -= 2 * PI;
	vwh = (int)round((GRIDSIZE /
	(calc_distance_vector(game->player.pos, sprite_pos)
	* cos(angle - game->player.rotation_angle)) * game->view_plane_distance));
	win_pos = vector_constructor(
	(ratio * game->config.window_width) - vwh / 2,
	game->config.window_height / 2 - vwh / 2);
	sprite_view_length = distance * cos(angle - game->player.rotation_angle);
	draw_sprtite(game, win_pos, vwh, sprite_view_length);
}

static void	calc_sprites_distance(t_game *game)
{
	int i;

	i = 0;
	while (i < game->config.sprite_count)
	{
		game->sprites[i].distance =
		calc_distance_vector(game->player.pos, game->sprites[i].pos);
		i++;
	}
}

static void	sort_sprites(t_sprite *base, int size)
{
	int			i;
	int			j;
	t_sprite	tmp;

	i = size;
	while (i)
	{
		j = 0;
		while (j < size - 1)
		{
			if (base[j].distance < base[j + 1].distance)
			{
				tmp = base[j];
				base[j] = base[j + 1];
				base[j + 1] = tmp;
			}
			j++;
		}
		i--;
	}
}

void		put_sprites(t_game *game)
{
	int		i;
	double	ang;
	double	wid;

	calc_sprites_distance(game);
	sort_sprites(game->sprites, game->config.sprite_count);
	i = 0;
	while (i < game->config.sprite_count)
	{
		ang = calc_sprite_theta(game->player.pos, game->sprites[i].pos);
		wid = normalized_angle(atan2(GRIDSIZE / 2, game->sprites[i].distance));
		if (calc_angle_diff(game, normalized_angle(
			ang), FALSE) <= game->fov ||
		calc_angle_diff(game, normalized_angle(
			ang + wid), FALSE) <= game->fov ||
		calc_angle_diff(game, normalized_angle(
			ang - wid), FALSE) <= game->fov ||
		calc_angle_diff(game, normalized_angle(
			ang + wid / 2), FALSE) <= game->fov ||
		calc_angle_diff(game, normalized_angle(
			ang - wid / 2), FALSE) <= game->fov)
			put_one_sprite(game, game->sprites[i].pos,
			ang, game->sprites[i].distance);
		i++;
	}
}
