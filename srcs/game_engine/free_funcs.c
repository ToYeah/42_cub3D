/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 08:36:49 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/04 14:26:50 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_engine.h"

static void	map_free(char **map)
{
	int i;

	i = 0;
	while (i < MAX_MAP_SIZE)
	{
		free(map[i]);
		map[i] = NULL;
		i++;
	}
	free(map);
	map = NULL;
}

static void	free_mlx_conf(t_game *game)
{
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->data.img)
		mlx_destroy_image(game->mlx, game->data.img);
	if (game->config.north_texture.img)
		mlx_destroy_image(game->mlx, game->config.north_texture.img);
	if (game->config.east_texture.img)
		mlx_destroy_image(game->mlx, game->config.east_texture.img);
	if (game->config.west_texture.img)
		mlx_destroy_image(game->mlx, game->config.west_texture.img);
	if (game->config.south_texture.img)
		mlx_destroy_image(game->mlx, game->config.south_texture.img);
	if (game->config.sprite_texture.img)
		mlx_destroy_image(game->mlx, game->config.sprite_texture.img);
}

static void	free_all_cub(t_game *game)
{
	if (game)
	{
		if (game->map)
			map_free(game->map);
		if (game->collisions)
			free(game->collisions);
		if (game->sprites)
			free(game->sprites);
		if (game->mlx)
		{
			mlx_loop_end(game->mlx);
			free_mlx_conf(game);
			mlx_destroy_display(game->mlx);
			free(game->mlx);
		}
		free(game);
	}
}

void		exit_cub(t_game *game, int status)
{
	free_all_cub(game);
	exit(status);
}
