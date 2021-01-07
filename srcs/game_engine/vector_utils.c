/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 08:51:01 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/04 16:22:47 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_engine.h"

t_vector	vector_constructor(double x, double y)
{
	t_vector result;

	result.x = x;
	result.y = y;
	return (result);
}

double		calc_distance_vector(t_vector start, t_vector end)
{
	return (sqrt((start.x - end.x) * (start.x - end.x) +
	(start.y - end.y) * (start.y - end.y)));
}
