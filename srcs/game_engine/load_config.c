/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 09:06:55 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/07 08:33:15 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_engine.h"

static double	select_start_angle(char c)
{
	if (c == 'S')
		return (normalized_angle(PI / 2));
	if (c == 'E')
		return (normalized_angle(2 * PI));
	if (c == 'W')
		return (normalized_angle(PI));
	return (normalized_angle(2 * PI - PI / 2));
}

static void		init_player(t_player *player, t_config *conf)
{
	player->pos = vector_constructor(
	(conf->start_x * GRIDSIZE + GRIDSIZE / 2) + 0.1,
	(conf->start_y * GRIDSIZE + GRIDSIZE / 2) + 0.1);
	player->radius = 3;
	player->turn_direction = 0;
	player->vertical_direction = 0;
	player->horizontal_direction = 0;
	player->rotation_angle = select_start_angle(conf->start_rotation);
	player->rotate_speed = (double)conf->window_width / 500 * PI / 180;
	player->move_speed = (double)conf->window_width / 500;
}

double			calc_view_wall_height(t_game *game, double view_length)
{
	if (view_length == 0)
		view_length = 0.3;
	return (GRIDSIZE / view_length * game->view_plane_distance);
}

t_bool			load_configuration(t_game *game, t_bool is_save)
{
	if (!is_save)
	{
		game->win = mlx_new_window(game->mlx,
		game->config.window_width, game->config.window_height, "mlx");
	}
	game->data.img = mlx_new_image(game->mlx,
	game->config.window_width, game->config.window_height);
	game->sprites = malloc_sprite_ary(game);
	game->collisions = malloc_collisions(game);
	if (!game->data.img || !game->sprites
	|| !game->collisions || (!is_save && !game->win))
		return (put_err_msg(strerror(errno)));
	game->data.addr = mlx_get_data_addr(
	game->data.img, &(game->data.bits_per_pixel),
	&(game->data.line_length), &(game->data.endian));
	game->fov = FOV * (PI / 180);
	game->ray_max = game->config.window_width / COLUMUN_WIDTH;
	game->view_plane_distance =
	(game->config.window_width / 2) / tan(game->fov / 2);
	init_player(&(game->player), &(game->config));
	return (TRUE);
}
