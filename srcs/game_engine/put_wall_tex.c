/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_wall_tex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 09:00:23 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/04 15:58:05 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_engine.h"

static void	set_wall_pos(t_ray *ray, double wall_height, int x, t_wall *wall)
{
	double tmp;

	wall->wall_height = round(wall_height);
	wall->win_x = x;
	wall->win_y = 0;
	if (ray->select_horizontal == TRUE)
		tmp = fmod(ray->collision_point.x, GRIDSIZE);
	else
		tmp = fmod(ray->collision_point.y, GRIDSIZE);
	wall->offset_x = (tmp / 32);
}

static void	put_wall_texture(
	t_game *game, t_wall *wall, t_texture *texture, t_bool rev)
{
	t_vector	tex;
	double		offset;
	int			wall_y;
	int			color;

	offset = (game->config.window_height / 2) - (wall->wall_height / 2);
	tex = vector_constructor(wall->offset_x * texture->width, 0);
	if (rev)
		tex.x = texture->width - tex.x;
	wall_y = 0;
	if (offset < 0)
		wall_y = -offset;
	wall->win_y = offset + wall_y;
	while (wall_y < wall->wall_height
	&& wall->win_y < game->config.window_height)
	{
		if (0 <= wall->win_y && wall->win_y <= game->config.window_height)
		{
			tex.y = (((double)wall_y / (double)wall->wall_height) *
			texture->height);
			color = extract_color(texture, (int)tex.x, (int)tex.y);
			my_mlx_pixel_put(&(game->data), wall->win_x, wall->win_y, color);
		}
		wall->win_y = offset + ++wall_y;
	}
}

void		put_one_colmun(t_game *game, int i, t_ray *ray, double wall_height)
{
	t_wall wall;

	set_wall_pos(ray, wall_height, i, &wall);
	if (ray->vertical_direction == UP && ray->select_horizontal == TRUE)
		put_wall_texture(game, &wall, &(game->config.south_texture), FALSE);
	else if (ray->vertical_direction == DOWN && ray->select_horizontal == TRUE)
		put_wall_texture(game, &wall, &(game->config.north_texture), TRUE);
	else if (ray->horizontal_direction == LEFT
	&& ray->select_horizontal == FALSE)
		put_wall_texture(game, &wall, &(game->config.east_texture), TRUE);
	else
		put_wall_texture(game, &wall, &(game->config.west_texture), FALSE);
}
