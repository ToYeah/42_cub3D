/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 13:11:13 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/05 19:08:26 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_engine.h"

char		**malloc_map(int x, int y)
{
	char	**result;
	int		i;

	i = 0;
	if (!(result = malloc(sizeof(char *) * y)))
		return (NULL);
	while (i < y)
	{
		if (!(result[i] = malloc(x)))
		{
			i--;
			while (i >= 0)
			{
				ft_safe_free_char(&result[i]);
				i--;
			}
			free(result);
			result = NULL;
			return (NULL);
		}
		ft_memset(result[i], ' ', x);
		i++;
	}
	return (result);
}

t_sprite	*malloc_sprite_ary(t_game *game)
{
	int			index_x;
	int			index_y;
	int			ary_index;
	t_sprite	*result;

	index_y = -1;
	ary_index = 0;
	if (!(result = malloc(sizeof(t_sprite) * game->config.sprite_count)))
		return (NULL);
	while (++index_y < game->config.map_height)
	{
		index_x = 0;
		while (index_x < game->config.map_width)
		{
			if (game->map[index_y][index_x] == ITEM_CHAR)
			{
				result[ary_index].pos = vector_constructor(index_x *
				GRIDSIZE + GRIDSIZE / 2, index_y * GRIDSIZE + GRIDSIZE / 2);
				result[ary_index].distance = 0;
				ary_index++;
			}
			index_x++;
		}
	}
	return (result);
}

t_collision	*malloc_collisions(t_game *game)
{
	t_collision *result;

	if (!(result = malloc(sizeof(t_collision) * game->config.window_width)))
		return (NULL);
	return (result);
}
