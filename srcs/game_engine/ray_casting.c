/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 09:05:33 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/08 08:00:26 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_engine.h"

static t_bool	calc_horizontal_intersection(
	t_game *game, t_vector player_pos, t_ray *ray, t_vector *res)
{
	t_vector step;

	res->y = floor(player_pos.y / GRIDSIZE) * GRIDSIZE;
	if (ray->vertical_direction == DOWN)
		res->y += GRIDSIZE;
	res->x = player_pos.x + (res->y - player_pos.y) / tan(ray->angle);
	step.y = GRIDSIZE * ray->vertical_direction;
	step.x = GRIDSIZE / tan(ray->angle);
	if ((step.x < 0 && ray->horizontal_direction == RIGHT)
	|| (step.x > 0 && ray->horizontal_direction == LEFT))
		step.x *= -1;
	while (res->x >= 0 && res->x <= MAX_MAP_SIZE * GRIDSIZE
	&& res->y >= 0 && res->y <= MAX_MAP_SIZE * GRIDSIZE)
	{
		if (valid_collision_dir(game, *res, ray->vertical_direction, TRUE))
			*res = vector_constructor(res->x + step.x, res->y + step.y);
		else
			return (TRUE);
	}
	return (FALSE);
}

static t_bool	calc_vertical_intersection(
	t_game *game, t_vector player_pos, t_ray *ray, t_vector *res)
{
	t_vector	step;

	res->x = floor(player_pos.x / GRIDSIZE) * GRIDSIZE;
	if (ray->horizontal_direction == RIGHT)
		res->x += GRIDSIZE;
	res->y = player_pos.y + (res->x - player_pos.x) * tan(ray->angle);
	step.x = GRIDSIZE * ray->horizontal_direction;
	step.y = GRIDSIZE * tan(ray->angle);
	if ((step.y < 0 && ray->vertical_direction == DOWN)
	|| (step.y > 0 && ray->vertical_direction == UP))
		step.y *= -1;
	while (res->x >= 0 && res->x <= MAX_MAP_SIZE * GRIDSIZE
	&& res->y >= 0 && res->y <= MAX_MAP_SIZE * GRIDSIZE)
	{
		if (valid_collision_dir(game, *res, ray->horizontal_direction, FALSE))
			*res = vector_constructor(res->x + step.x, res->y + step.y);
		else
			return (TRUE);
	}
	return (FALSE);
}

static void		set_ray_direction(t_ray *ray)
{
	ray->vertical_direction = UP;
	if (ray->angle > 0 && ray->angle < PI)
		ray->vertical_direction = DOWN;
	ray->horizontal_direction = LEFT;
	if (ray->angle < (PI / 2) || ray->angle > (PI / 2 + PI))
		ray->horizontal_direction = RIGHT;
}

static void		cast_single_ray(t_game *game, t_ray *ray)
{
	t_vector	vertical_intersection;
	t_bool		horizontal_flag;

	ray->angle = correct_angle(normalized_angle(ray->angle));
	set_ray_direction(ray);
	ray->select_horizontal = TRUE;
	horizontal_flag = calc_horizontal_intersection(
		game, game->player.pos, ray, &(ray->collision_point));
	calc_vertical_intersection(
		game, game->player.pos, ray, &vertical_intersection);
	if ((!horizontal_flag ||
	calc_distance_vector(game->player.pos, ray->collision_point) >=
	calc_distance_vector(game->player.pos, vertical_intersection)))
	{
		ray->collision_point = vertical_intersection;
		ray->select_horizontal = FALSE;
	}
}

void			cast_all_ray(t_game *game)
{
	int			count;
	t_ray		ray;

	count = 0;
	ray.angle = game->player.rotation_angle - (game->fov / 2);
	while (count < game->ray_max)
	{
		cast_single_ray(game, &ray);
		game->collisions[count].length =
		calc_distance_vector(game->player.pos, ray.collision_point);
		game->collisions[count].view_length =
		game->collisions[count].length *
		cos(normalized_angle(ray.angle - game->player.rotation_angle));
		put_one_colmun(game, count, &ray, calc_view_wall_height(
			game, game->collisions[count].view_length));
		count++;
		ray.angle = calc_angle_step(count, game);
	}
}
