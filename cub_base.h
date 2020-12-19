/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_base.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 02:33:18 by totaisei          #+#    #+#             */
/*   Updated: 2020/12/19 13:40:58 by totaisei         ###   ########.fr       */
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
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "my_libft/libft.h"

/////////////////////////////////////////////////
#include <stdio.h>
/////////////////////////////////////////////////

#define DEF_ERR_MSG "Error"

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
	t_bool		map_flag;
	int			start_x;
	int			start_y;
	int			item_count;
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
	double moveSpeed;
	double rotationSpeed;
}				t_player;


typedef struct	s_game
{
	void	*mlx;
	void	*win;
	t_data	*data;
	t_player *player;
	t_config config;
	char **map;
}				t_game;


#endif
