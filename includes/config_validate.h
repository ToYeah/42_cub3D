/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_validate.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 13:07:24 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/05 19:52:19 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_VALIDATE_H
# define CONFIG_VALIDATE_H

# include "cub_base.h"

# define MAX_RESOLUTION_SIZE 100000
# define MIN_RESOLUTION_SIZE 100

typedef enum	e_identifier
{
	RESOLUTION,
	NORTH,
	SOUTH,
	WEST,
	EAST,
	SPRITE,
	FLOOR,
	CEILLING,
	MAP,
	NEWLINE,
	UNKNOWN
}				t_identifier;

typedef enum	e_status
{
	FAILURE,
	NOT_ENTERED,
	ENTERED
}				t_status;

typedef struct	s_ident_line
{
	t_identifier	ident;
	t_status		*status;
	char			*line;
}				t_ident_line;

typedef t_status	(*t_valid_func)(t_game *, t_ident_line);

t_status		valid_resolution(t_game *game, t_ident_line input);
t_status		valid_texture_path(t_game *game, t_ident_line input);
t_status		valid_color(t_game *game, t_ident_line input);
t_status		valid_map(t_game *game, t_ident_line input);
t_status		valid_unknown(t_game *game, t_ident_line input);
t_status		valid_new_line(t_game *game, t_ident_line input);
t_bool			valid_statuses(t_status *statuses);
t_bool			cub_flood_fill(char **map, int x, int y);
t_bool			valid_cub(t_game *game, int fd);
t_bool			set_configuration(t_game *game, char *path);
t_bool			valid_runtime_arg(int argc, char **argv, t_bool *is_save);
char			*one_char_compare(char *str, unsigned char c);
void			init_config(t_config *conf);
void			init_valid_funcs(t_valid_func *f);

#endif
