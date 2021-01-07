/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   angle_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 08:52:36 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/05 19:55:13 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_engine.h"

double	calc_sprite_theta(t_vector player_pos, t_vector sprite_pos)
{
	t_vector	diff;
	double		result;

	diff.x = player_pos.x - sprite_pos.x;
	diff.y = player_pos.y - sprite_pos.y;
	result = atan2(diff.y, diff.x) + PI;
	return (result);
}

double	calc_angle_diff(t_game *game, double angle, t_bool minus)
{
	double raise;
	double start;

	start = normalized_angle(game->player.rotation_angle - (game->fov / 2));
	raise = 2 * PI - start;
	if (angle >= start)
		angle = angle - start;
	else
		angle = angle + raise;
	if (minus && angle > PI)
		angle = angle - (2 * PI);
	return (angle);
}

double	normalized_angle(double angle)
{
	int div;

	if (angle < 0 || angle > PI * 2)
	{
		div = (angle / (PI * 2));
		angle -= (div * PI * 2);
		if (angle < 0)
			angle += PI * 2;
	}
	return (angle);
}

double	correct_angle(double angle)
{
	if (3.1415 < angle && angle < 3.1416)
		angle = 3.14;
	if (-0.0001 < angle && angle < 0.0001)
		angle = 0.001;
	return (angle);
}
