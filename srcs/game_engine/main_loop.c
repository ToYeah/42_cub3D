/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 09:13:06 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/06 17:20:45 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_engine.h"

static void		update_player_pos(t_game *game, t_player *player)
{
	t_vector new_pos;

	player->rotation_angle += player->rotate_speed * (player->turn_direction);
	player->rotation_angle = normalized_angle(player->rotation_angle);
	new_pos.x = player->pos.x;
	new_pos.y = player->pos.y;
	new_pos.x += cos(player->rotation_angle) *
	player->vertical_direction * player->move_speed;
	new_pos.y += sin(player->rotation_angle) *
	player->vertical_direction * player->move_speed;
	new_pos.x += (cos(player->rotation_angle + PI / 2)) *
	player->horizontal_direction * player->move_speed;
	new_pos.y += (sin(player->rotation_angle + PI / 2)) *
	player->horizontal_direction * player->move_speed;
	if (validate_new_position(game, new_pos))
		player->pos = new_pos;
}

void			put_background(t_game *game)
{
	int i;
	int j;

	i = 0;
	while (i < game->config.window_height / 2)
	{
		j = 0;
		while (j < game->config.window_width)
		{
			my_mlx_pixel_put(&(game->data), j, i, game->config.ceiling_color);
			j++;
		}
		i++;
	}
	while (i < game->config.window_height)
	{
		j = 0;
		while (j < game->config.window_width)
		{
			my_mlx_pixel_put(&(game->data), j, i, game->config.floor_color);
			j++;
		}
		i++;
	}
}

int				main_loop(t_game *game)
{
	update_player_pos(game, &(game->player));
	put_background(game);
	cast_all_ray(game);
	put_sprites(game);
	mlx_put_image_to_window(game->mlx, game->win, game->data.img, 0, 0);
	return (0);
}
