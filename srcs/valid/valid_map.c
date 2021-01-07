/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 13:34:31 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/07 11:40:22 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_validate.h"

static t_bool	is_dir_char(char c)
{
	if (c == 'N' || c == 'E' || c == 'W' || c == 'S')
		return (TRUE);
	return (FALSE);
}

t_status		valid_map(t_game *game, t_ident_line input)
{
	int i;

	i = -1;
	game->config.map_flag = TRUE;
	if (*input.status == FAILURE || !(ft_strlen(input.line) < MAX_MAP_SIZE)
	|| !(game->config.map_height < MAX_MAP_SIZE))
		return (FAILURE);
	while (input.line[++i])
	{
		game->map[game->config.map_height][i] = input.line[i];
		if (is_dir_char(input.line[i]))
		{
			if (!(game->config.start_x == 0 && game->config.start_y == 0))
				return (FAILURE);
			game->config.start_x = i;
			game->config.start_y = game->config.map_height;
			game->config.start_rotation = input.line[i];
		}
		if (input.line[i] == '2')
			game->config.sprite_count++;
	}
	if (game->config.map_width < i)
		game->config.map_width = i;
	game->config.map_height++;
	return (ENTERED);
}
