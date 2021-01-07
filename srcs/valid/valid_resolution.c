/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_resolution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 13:20:13 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/07 10:51:50 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_validate.h"

static t_bool	is_resolution_format(char *str)
{
	str = ft_through_space(str);
	if (!ft_isdigit(*str))
		return (FALSE);
	str = ft_through_digit(str);
	if (!(str = one_char_compare(str, ' ')))
		return (FALSE);
	str = ft_through_space(str);
	if (!ft_isdigit(*str))
		return (FALSE);
	str = ft_through_digit(str);
	if (*str != '\0')
		return (FALSE);
	return (TRUE);
}

static t_bool	atoi_range(char *str, int max, int min, int *result)
{
	if (*str == '0')
		return (FALSE);
	*result = 0;
	while (ft_isdigit(*str) && *result < max)
	{
		*result *= 10;
		*result += (*str - '0');
		str++;
	}
	if (*result < min)
	{
		*result = -1;
		return (FALSE);
	}
	if (!(*result < max))
	{
		*result = max;
		return (TRUE);
	}
	return (TRUE);
}

t_status		valid_resolution(t_game *game, t_ident_line input)
{
	int max_width;
	int max_height;

	if (*(input.status) != NOT_ENTERED || game->config.map_flag)
		return (FAILURE);
	input.line += 1;
	input.line = ft_through_space(input.line);
	if (is_resolution_format(input.line))
	{
		mlx_get_screen_size(game->mlx, &max_width, &max_height);
		if (atoi_range(input.line, max_width,
		MIN_RESOLUTION_SIZE, &(game->config.window_width)))
		{
			input.line = ft_through_digit(input.line);
			input.line = ft_through_space(input.line);
			if (atoi_range(input.line, max_height,
			MIN_RESOLUTION_SIZE, &(game->config.window_height)))
				return (ENTERED);
		}
	}
	return (FAILURE);
}
