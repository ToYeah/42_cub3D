/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 09:10:13 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/05 19:54:41 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_engine.h"

int		press_key(int key_code, t_game *game)
{
	if (key_code == KEY_ESC)
		exit_cub(game, EXIT_SUCCESS);
	else if (key_code == KEY_W)
		game->player.vertical_direction = 1;
	else if (key_code == KEY_S)
		game->player.vertical_direction = -1;
	else if (key_code == KEY_D)
		game->player.horizontal_direction = 1;
	else if (key_code == KEY_A)
		game->player.horizontal_direction = -1;
	else if (key_code == KEY_LEFT)
		game->player.turn_direction = -1;
	else if (key_code == KEY_RIGHT)
		game->player.turn_direction = 1;
	return (0);
}

int		release_key(int key_code, t_game *game)
{
	if (key_code == KEY_ESC)
		exit_cub(game, EXIT_SUCCESS);
	else if (key_code == KEY_W)
		game->player.vertical_direction = 0;
	else if (key_code == KEY_S)
		game->player.vertical_direction = 0;
	else if (key_code == KEY_D)
		game->player.horizontal_direction = 0;
	else if (key_code == KEY_A)
		game->player.horizontal_direction = 0;
	else if (key_code == KEY_LEFT)
		game->player.turn_direction = 0;
	else if (key_code == KEY_RIGHT)
		game->player.turn_direction = 0;
	return (0);
}

int		press_red_cross(t_game *game)
{
	exit_cub(game, EXIT_SUCCESS);
	return (0);
}
