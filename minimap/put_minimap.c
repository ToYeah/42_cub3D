/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_minimap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 08:23:27 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/04 08:30:43 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub_base.h"

void put_line(t_data *data, t_vector start, t_vector end ,int color)
{
	int frac;
	int xNext, yNext, xStep, yStep, xDelta, yDelta;
	int xself, yself, xEnd, yEnd;
	xself = round(start.x);
	yself = round(start.y);
	xEnd = round(end.x);
	yEnd = round(end.y);
	xNext = xself;
	yNext = yself;
	xDelta = xEnd - xself;
	yDelta = yEnd - yself;
	xStep = 1;
	yStep = 1;
	if (xDelta < 0)
		xStep = -1;
	if (yDelta < 0)
		yStep = -1;
	xDelta = xDelta * 2;
	yDelta = yDelta * 2;
	if (xDelta < 0)
		xDelta = -xDelta;
	if (yDelta < 0)
		yDelta = -yDelta;
	my_mlx_pixel_put(data, xNext, yNext, color);
	// --A--
	if (xDelta > yDelta)
	{
		frac = yDelta * 2 - xDelta;
		while (xNext != xEnd)
		{
			if (frac >= 0)
			{
				yNext = yNext + yStep;
				frac = frac - xDelta;
			}
			xNext = xNext + xStep;
			frac = frac + yDelta;
			my_mlx_pixel_put(data, xNext, yNext, color);
		}
		// --B--
	}
	else
	{
		frac = xDelta * 2 - yDelta;
		while (yNext != yEnd)
		{
			if (frac >= 0)
			{
				xNext = xNext + xStep;
				frac = frac - yDelta;
			}
			yNext = yNext + yStep;
			frac = frac + xDelta;
			my_mlx_pixel_put(data, xNext, yNext, color);
		}
	}
}

void put_dot(t_data *data, t_vector player_pos, int color)
{
	int i;
	int j;

	i = 0;
	player_pos.x -= 1;
	player_pos.y -= 1;
	while(i < 10)
	{
		j = 0;
		while(j < 10)
		{
			my_mlx_pixel_put(data, (player_pos.x + j) * MINI_MAP_SCALE, (player_pos.y + i) * MINI_MAP_SCALE, color);
			j++;
		}
		i++;
	}
}

void put_miniray(t_game *game, double angle, double length, int color)
{
	t_vector end_point;
	end_point = vector_constructor(
	game->player.pos.x * MINI_MAP_SCALE + length * cos(angle),
	game->player.pos.y * MINI_MAP_SCALE + length * sin(angle));
	put_line(&(game->data), 
	vector_constructor(game->player.pos.x * MINI_MAP_SCALE,
	game->player.pos.y * MINI_MAP_SCALE),
	end_point, color);
}

void put_minimap(t_game *game)
{
	int i;
	double angle;
	t_vector player_direction;
	put_map(game, game->map);

	angle = normalized_angle(game->player.rotationAngle - (game->fov / 2));
	i = 0;
	while (i < game->ray_max)
	{
		put_miniray(game, angle,
		game->collisions[i].length * MINI_MAP_SCALE, 0x00FF0000);
		angle += game->fov / game->ray_max;;
		i++;
	}

	put_dot(&(game->data), game->player.pos, 0x00000000);
	put_miniray(game, game->player.rotationAngle, 5, 0x00000000);
	put_minisprite(game);
}

void put_minisprite(t_game *game)
{
	int i;
	double sprite_angle;
	double angle_width;
	double distance;

	i = 0;
	while(i < game->config.sprite_count)
	{
		sprite_angle = calc_sprite_theta(game->player.pos, game->sprites[i].pos);
		distance = calc_distance_vector(game->player.pos, game->sprites[i].pos);
		angle_width = normalized_angle(atan2(GRIDSIZE / 2, distance));
		put_line(&(game->data), vector_constructor(
		game->player.pos.x * MINI_MAP_SCALE + distance * cos(sprite_angle + angle_width) * MINI_MAP_SCALE, 
		game->player.pos.y * MINI_MAP_SCALE + distance * sin(sprite_angle + angle_width) * MINI_MAP_SCALE),
		vector_constructor(
		game->player.pos.x * MINI_MAP_SCALE + distance * cos(sprite_angle - angle_width) * MINI_MAP_SCALE, 
		game->player.pos.y * MINI_MAP_SCALE + distance * sin(sprite_angle - angle_width) * MINI_MAP_SCALE),
		0x000000FF);
		i++;
	}
}


void	put_square(t_data *data, int x, int y, int fill)
{
	int pos_x;
	int pos_y;
	int index_x;
	int index_y;
	int map_size = GRIDSIZE * MINI_MAP_SCALE;

	pos_x = x * map_size ;
	pos_y = y * map_size ;
	
	index_y = map_size;
	while(index_y != 0)
	{
		index_x = map_size;
		while (index_x != 0)
		{
			if (index_x == 1 || index_x == map_size || index_y == 1 || index_y == map_size || fill)
				my_mlx_pixel_put(data, index_x + pos_x, index_y + pos_y, 0x00000000);
			else
				my_mlx_pixel_put(data, index_x + pos_x, index_y + pos_y, 0x00FFFFFF);
			index_x--;
		}
		index_y--;
	}
}

void	put_map(t_game *game,char **map)
{
	int x;
	int y;

	y = 0;
	while(y < game->config.map_height)
	{
		x = 0;
		while(x < game->config.map_width)
		{
			if (map[y][x] == WALL_CHAR)
				put_square(&(game->data), x, y, TRUE);
			else if (map[y][x] != ' ')
				put_square(&(game->data), x, y, FALSE);
			x++;
		}
		y++;
	}
}
