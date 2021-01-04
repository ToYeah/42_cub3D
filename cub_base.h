/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_base.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 02:33:18 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/04 08:29:21 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_BASE_H
# define CUB_BASE_H

#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "./minilibx-linux/mlx.h"

#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <sysexits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "my_libft/libft.h"

/////////////////////////////////////////////////
#include <stdio.h>
/////////////////////////////////////////////////

#define DEF_ERR_MSG "Error"
#define MAX_MAP_SIZE 200
#define FLOOR_CHAR '-'
#define ITEM_CHAR '@'
#define WALL_CHAR '1'
#define SPACE_CHAR ' '
#define GRIDSIZE 32
#define MOVE_SPEED 0.5;
#define ROTATE_SPEED (0.5 * (PI / 180))

#define FILE_H_SIZE 14
#define INFO_H_SIZE 40

typedef struct	s_vector
{
	double x;
	double y;
}				t_vector;

typedef struct	s_texture
{
	void 	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}				t_texture;

typedef struct	s_config
{
	int			window_width;
	int			window_height;
	int			floor_color;
	int			ceiling_color;
	t_texture	north_texture;
	t_texture	south_texture;
	t_texture	west_texture;
	t_texture	east_texture;
	t_texture	sprite_texture;
	int			map_height;
	int			map_width;
	t_bool		map_flag;
	int			start_x;
	int			start_y;
	int			sprite_count;
	char		start_rotation;
	t_bool		is_save;
}				t_config;


typedef struct	s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct	s_player
{
	t_vector pos;
	double radius;
	double turnDirection;
	double verticalDirection;
	double horizontalDirection;
	double rotationAngle;
}				t_player;

typedef struct	s_collision
{
	double view_length;
	double length;
}				t_collision;

typedef struct	s_sprite
{
	t_vector pos;
	double distance;
}				t_sprite;


typedef struct	s_game
{
	void	*mlx;
	void	*win;
	t_data	data;
	t_player player;
	t_config config;
	char **map;
	double fov;
	int ray_max;
	t_sprite *sprites;
	double view_plane_distance;
	t_collision *collisions;
}				t_game;


t_bool set_configuration(t_game *game, char *path);

char **malloc_map(size_t x, size_t y);

t_bool valid_runtime_arg(int argc, char **argv, t_bool *is_save);

t_bool put_err_msg(char *err_msg);

t_vector vector_constructor(double x, double y);

t_sprite *malloc_sprite_ary(t_game *game);

t_collision *malloc_collisions(t_game *game);

double	calc_distance_vector(t_vector start, t_vector end);

double	calc_sprite_theta(t_vector player_pos, t_vector sprite_pos);
void exit_cub(t_game *game, int status);
#endif
