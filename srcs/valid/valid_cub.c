/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_cub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 03:40:39 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/11 08:51:45 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_validate.h"

static t_bool		is_map_line(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (!(ft_strchr(" 012NEWS", str[i])))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

static t_identifier	select_identifier(char *line, t_game *game)
{
	if (!line)
		return (UNKNOWN);
	if (*line == '\0')
		return (NEWLINE);
	if (is_map_line(line) && game->config.finish_map_flag == FALSE)
		return (MAP);
	if (ft_strncmp(line, "R ", 2) == 0)
		return (RESOLUTION);
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (NORTH);
	if (ft_strncmp(line, "SO ", 3) == 0)
		return (SOUTH);
	if (ft_strncmp(line, "WE ", 3) == 0)
		return (WEST);
	if (ft_strncmp(line, "EA ", 3) == 0)
		return (EAST);
	if (ft_strncmp(line, "S ", 2) == 0)
		return (SPRITE);
	if (ft_strncmp(line, "F ", 2) == 0)
		return (FLOOR);
	if (ft_strncmp(line, "C ", 2) == 0)
		return (CEILLING);
	return (UNKNOWN);
}

t_bool				valid_cub(t_game *game, int fd)
{
	int				gnl_result;
	int				i;
	t_status		statuses[(int)UNKNOWN + 1];
	t_ident_line	ident_line;
	t_valid_func	valid_funcs[(int)UNKNOWN + 1];

	i = 0;
	while (i < (int)UNKNOWN + 1)
		statuses[i++] = NOT_ENTERED;
	gnl_result = 1;
	init_config(&(game->config));
	init_valid_funcs(valid_funcs);
	while (gnl_result == 1)
	{
		gnl_result = ft_get_next_line(fd, &(ident_line.line));
		ident_line.ident = select_identifier(ident_line.line, game);
		ident_line.status = &(statuses[(int)ident_line.ident]);
		*(ident_line.status) =
		valid_funcs[(int)ident_line.ident](game, ident_line);
		ft_safe_free_char(&(ident_line.line));
	}
	if (gnl_result != 0)
		return (put_err_msg(strerror(errno)));
	return (valid_statuses(statuses));
}

t_bool				set_configuration(t_game *game, char *path)
{
	int fd;

	if ((fd = open(path, O_RDONLY)) < 0)
		return (put_err_msg(strerror(errno)));
	if (!valid_cub(game, fd))
		return (FALSE);
	if (game->config.start_x == 0 && game->config.start_y == 0)
		return (put_err_msg("No starting point."));
	if (!cub_flood_fill(game->map, game->config.start_x, game->config.start_y))
		return (put_err_msg("Unclosed map."));
	return (TRUE);
}

t_bool				valid_runtime_arg(int argc, char **argv, t_bool *is_save)
{
	if (argc == 1)
		return (put_err_msg("too few arguments to cub3D."));
	if (argc > 3)
		return (put_err_msg("Too many arguments to cub3D"));
	if (argc > 1 && (ft_strlen(argv[1]) < 5 || ft_strrncmp(argv[1], ".cub", 4)
	|| argv[1][(int)ft_strlen(argv[1]) - 5] == '/'))
		return (put_err_msg("The second argument is incorrect"));
	if (argc == 3 && ft_strncmp(argv[2], "--save", 7))
		return (put_err_msg("The third argument is incorrect"));
	else if (argc == 3)
		*is_save = (TRUE);
	return (TRUE);
}
