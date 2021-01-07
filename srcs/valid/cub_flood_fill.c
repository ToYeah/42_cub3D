/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_flood_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 14:02:15 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/04 16:26:59 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_validate.h"

t_bool	cub_flood_fill(char **map, int x, int y)
{
	if (map[y][x] == '1' || map[y][x] == FLOOR_CHAR || map[y][x] == ITEM_CHAR)
		return (TRUE);
	if (map[y][x] == ' ')
	{
		map[y][x] = 'x';
		return (FALSE);
	}
	if (map[y][x] == '0')
		map[y][x] = FLOOR_CHAR;
	if (map[y][x] == '2')
		map[y][x] = ITEM_CHAR;
	if (cub_flood_fill(map, x + 1, y) && cub_flood_fill(map, x - 1, y) &&
	cub_flood_fill(map, x, y + 1) && cub_flood_fill(map, x, y - 1))
		return (TRUE);
	return (FALSE);
}
