/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_pos_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 08:59:40 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/05 19:50:47 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_engine.h"

int	validate_new_position(t_game *game, t_vector pos)
{
	int new_x;
	int new_y;
	int now_x;
	int now_y;

	new_x = (floor(pos.x / GRIDSIZE));
	new_y = (floor(pos.y / GRIDSIZE));
	now_x = (floor(game->player.pos.x / GRIDSIZE));
	now_y = (floor(game->player.pos.y / GRIDSIZE));
	if (0 <= new_x && new_x < MAX_MAP_SIZE && 0 <= new_y && new_y < MAX_MAP_SIZE
	&& game->map[new_y][new_x] != WALL_CHAR
	&& game->map[new_y][new_x] != SPACE_CHAR
	&& (game->map[now_y][new_x] != WALL_CHAR
	&& game->map[now_y][new_x] != SPACE_CHAR
	&& game->map[new_y][now_x] != WALL_CHAR
	&& game->map[new_y][now_x] != SPACE_CHAR))
		return (TRUE);
	return (FALSE);
}

int	valid_collision_dir(t_game *game, t_vector pos, int dir, int horizontal)
{
	int x;
	int y;

	if (dir == UP && horizontal == TRUE)
		pos.y--;
	if (dir == LEFT && horizontal == FALSE)
		pos.x--;
	x = (floor(pos.x / GRIDSIZE));
	y = (floor(pos.y / GRIDSIZE));
	if (0 <= x && x < MAX_MAP_SIZE && 0 <= y && y < MAX_MAP_SIZE
	&& game->map[y][x] != WALL_CHAR)
		return (TRUE);
	return (FALSE);
}
