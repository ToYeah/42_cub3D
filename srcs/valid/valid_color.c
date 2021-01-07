/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 13:25:11 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/07 10:38:05 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_validate.h"

char			*one_char_compare(char *str, unsigned char c)
{
	if (!str)
		return (NULL);
	if ((unsigned char)*str == c)
		return (str + 1);
	return (NULL);
}

static char		*is_three_nums(char *str)
{
	int		count;
	char	*start;

	count = 0;
	start = str;
	while (*str && ft_isdigit(*str))
	{
		str++;
		count++;
	}
	if ((*start != '0' && (count == 1 || count == 2 || count == 3))
	|| (*start == '0' && count == 1))
		return (str);
	return (NULL);
}

static int		int_to_color(int a, int r, int g, int b)
{
	int result;

	result = 0 | ((unsigned char)a << 24) |
	((unsigned char)r << 16) | ((unsigned char)g << 8) | ((unsigned char)b);
	return (result);
}

static t_bool	is_color_format(char *str)
{
	if (!(str = is_three_nums(str))
	|| !(str = one_char_compare(str, ',')))
		return (FALSE);
	if (!(str = is_three_nums(str))
	|| !(str = one_char_compare(str, ',')))
		return (FALSE);
	if (!(str = is_three_nums(str)) || *str != '\0')
		return (FALSE);
	return (TRUE);
}

t_status		valid_color(t_game *game, t_ident_line input)
{
	int parameter[3];

	input.line += 1;
	input.line = ft_through_space(input.line);
	if (*(input.status) != NOT_ENTERED
	|| game->config.map_flag || !(is_color_format(input.line)))
		return (FAILURE);
	parameter[0] = ft_atoi(input.line);
	input.line = ft_through_digit(input.line);
	parameter[1] = ft_atoi(++input.line);
	input.line = ft_through_digit(input.line);
	parameter[2] = ft_atoi(++input.line);
	if (!(parameter[0] <= 0xFF && parameter[1] <= 0xFF && parameter[2] <= 0xFF))
		return (FAILURE);
	if (input.ident == FLOOR)
		game->config.floor_color =
		int_to_color(0, parameter[0], parameter[1], parameter[2]);
	else
		game->config.ceiling_color =
		int_to_color(0, parameter[0], parameter[1], parameter[2]);
	return (ENTERED);
}
