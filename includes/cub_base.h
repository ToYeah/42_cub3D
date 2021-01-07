/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_base.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 02:33:18 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/07 08:52:19 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_BASE_H
# define CUB_BASE_H

# include <limits.h>
# include <stdlib.h>
# include <math.h>
# include <errno.h>
# include <string.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "mlx.h"
# include "libft.h"

# define DEF_ERR_MSG "Error"
# define MAX_MAP_SIZE 200
# define FLOOR_CHAR '-'
# define ITEM_CHAR '@'
# define WALL_CHAR '1'
# define SPACE_CHAR ' '
# define GRIDSIZE 32
# define FILE_H_SIZE 14
# define INFO_H_SIZE 40

typedef struct	s_vector
{
	double		x;
	double		y;
}				t_vector;

typedef struct	s_texture
{
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			width;
	int			height;
}				t_texture;

typedef struct	s_config
{
	int			window_width;
	int			window_height;
	int			floor_color;
	int			ceiling_color;
	int			map_height;
	int			map_width;
	int			start_x;
	int			start_y;
	int			sprite_count;
	char		start_rotation;
	t_bool		map_flag;
	t_bool		is_save;
	t_texture	north_texture;
	t_texture	south_texture;
	t_texture	west_texture;
	t_texture	east_texture;
	t_texture	sprite_texture;
}				t_config;

typedef struct	s_data
{
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}				t_data;

typedef struct	s_player
{
	double		radius;
	double		turn_direction;
	double		vertical_direction;
	double		horizontal_direction;
	double		rotation_angle;
	double		rotate_speed;
	double		move_speed;
	t_vector	pos;
}				t_player;

typedef struct	s_collision
{
	double		view_length;
	double		length;
}				t_collision;

typedef struct	s_sprite
{
	double		distance;
	t_vector	pos;
}				t_sprite;

typedef struct	s_game
{
	void		*mlx;
	void		*win;
	char		**map;
	double		fov;
	int			ray_max;
	double		view_plane_distance;
	t_data		data;
	t_player	player;
	t_sprite	*sprites;
	t_config	config;
	t_collision	*collisions;
}				t_game;

t_bool			put_err_msg(char *err_msg);

#endif
