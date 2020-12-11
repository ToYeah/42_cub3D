/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 06:26:02 by totaisei          #+#    #+#             */
/*   Updated: 2020/12/11 17:26:38 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <X11/Xlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#include "./minilibx-linux/mlx.h"
#include <stdio.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define PI 3.14159265358979323846

#define ABS(Value) Value < 0 ? Value * -1 : Value
#define WIDTH 500
#define HEIGHT 500

#define GRIDSIZE 32



char map[10][10] = 
{
	{'1','1','1','1','1','1','1','1','1','1'},
	{'1','0','0','0','0','0','0','0','0','1'},
	{'1','0','0','0','0','0','0','0','0','1'},
	{'1','0','0','0','0','0','0','0','0','1'},
	{'1','1','1','1','0','0','0','0','0','1'},
	{'1','0','0','1','0','0','0','0','0','1'},
	{'1','0','0','0','0','0','0','0','0','1'},
	{'1','0','0','0','0','0','0','0','0','1'},
	{'1','0','0','0','0','0','0','0','0','1'},
	{'1','1','1','1','1','1','1','1','1','1'}
};

typedef struct	s_data
{
	void*	img;
	char*	addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct	s_player
{
	int x;
	int y;
	double radius;
	double turnDirection;
	double walkDirection;
	double rotationAngle;
	double moveSpeed;
	double rotationSpeed;
}				t_player;

typedef struct	s_line
{
	int start_x;
	int start_y;
	int end_x;
	int end_y;
}				t_line;

void	init_player(t_player *player, double x, double y)
{
	player->x = x;
	player->y = y;
	player->radius = 3;
	player->turnDirection = 0;
	player->walkDirection = 0;
	player->rotationAngle = PI / 2;
	player->moveSpeed = 3;
	player->rotationSpeed = 3 * (PI / 180);

}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	put_square(t_data *data, int x, int y, int fill)
{
	int pos_x;
	int pos_y;
	int index_x;
	int index_y;

	pos_x = x * GRIDSIZE ;//+ x *5;
	pos_y = y * GRIDSIZE ;//+ y *5;
	
	index_y = GRIDSIZE;
	while(index_y != 0)
	{
		index_x = GRIDSIZE;
		while (index_x != 0)
		{
			if (index_x == 1 || index_x == GRIDSIZE || index_y == 1 || index_y == GRIDSIZE || fill)
				my_mlx_pixel_put(data, index_x + pos_x, index_y + pos_y, 0x00000000);
			index_x--;
		}
		index_y--;
	}
}

void	put_map(t_data *data,char map[10][10], int x_len, int y_len)
{
	int x;
	int y;

	y = 0;

	while(y < y_len)
	{
		x = 0;
		while(x < x_len)
		{
			if (map[y][x] != '0')
				put_square(data, x, y, TRUE);
			else
				put_square(data, x, y, FALSE);
			x++;
		}
		y++;
	}
}

void	img_fill(t_data *data, int x, int y)
{
	if (!(y < HEIGHT))
		return;
	else if (!(x < WIDTH))
		img_fill(data, 0, y + 1);
	else
	{
		my_mlx_pixel_put(data, x, y, 0x00FFFFFF);
		img_fill(data, x + 1, y);
	}
}

void	put_square_deb(t_data *data, int x, int y, int fill)
{
	int pos_x;
	int pos_y;
	int index_x;
	int index_y;

	pos_x = x * GRIDSIZE ;//+ x *5;
	pos_y = y * GRIDSIZE ;//+ y *5;
	
	index_y = GRIDSIZE;
	while(index_y != 0)
	{
		index_x = GRIDSIZE;
		while (index_x != 0)
		{
			if (index_x == 1 || index_x == GRIDSIZE || index_y == 1 || index_y == GRIDSIZE || fill)
				my_mlx_pixel_put(data, index_x + pos_x, index_y + pos_y, 0x00FF0000);
			index_x--;
		}
		index_y--;
	}
}

void put_line_positive(t_line line, t_data *data, int posi, int nega)
{
	int d;
	int tmp;
	int dx;
	int dy;

	d = nega - (posi << 1);
	tmp = posi;
	posi = ((nega - posi) << 1);
	nega = -(tmp << 1);
	dx = line.end_x < line.start_x ? -1 : 1;
	dy = line.end_y < line.start_y ? -1 : 1;
	while(line.start_x != line.end_x)
	{
		line.start_x += dx;
		if(d < 0)
		{
			line.start_y += dy;
			d += posi;
		}
		else
			d += nega;
		if(line.start_x < 0 || line.start_y < 0)
			break;
		my_mlx_pixel_put(data, line.start_x, line.start_y, 0x00FF0000);
	}
}

void put_line_negative(t_line line, t_data *data, int posi, int nega)
{
	int d;
	int tmp;
	int dx;
	int dy;

	d = nega - (posi << 1);
	tmp = posi;
	posi = ((nega - posi) << 1);
	nega = -(tmp << 1);
	dx = line.end_x < line.start_x ? -1 : 1;
	dy = line.end_y < line.start_y ? -1 : 1;
	while(line.start_y != line.end_y)
	{
		line.start_y += dy;
		if(d < 0)
		{
			line.start_x += dx;
			d += posi;
		}
		else
			d += nega;
		if(line.start_x < 0 || line.start_y < 0)
			break;
		my_mlx_pixel_put(data, line.start_x, line.start_y, 0x00FF0000);
	}
}

void put_line(t_line line, t_data *data) 
{
	int positive_inc;
	int negative_inc;
	int dx;
	int dy;
	
	positive_inc = ABS(line.end_x - line.start_x);
	negative_inc = ABS(line.end_y - line.start_y);
	dx = 1;
	if (line.end_x < line.start_x)
		dx = -1;
	dy = 1;
	if (line.end_y < line.start_y)
		dy = -1;
	if (positive_inc > negative_inc)
	{
		positive_inc = ABS(line.end_y - line.start_y);
		negative_inc = ABS(line.end_x - line.start_x);
		put_line_negative(line, data, positive_inc, negative_inc);
	}
	else
	{
		put_line_positive(line, data, positive_inc, negative_inc);
	}
}

void put_player(t_data *data, t_player player)
{
	int i;
	int j;

	i = 0;
	player.x -= 5;
	player.y -= 5;
	while(i < 10)
	{
		j = 0;
		while(j < 10)
		{
			my_mlx_pixel_put(data, player.x + j, player.y + i, 0x00FF0000);
			j++;
		}
		i++;
	}
}

int main()
{
	void *mlx;
	void *mlx_win;
	t_data img;
	t_line centor;
	t_player player;


	init_player(&player, 100,100);

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, WIDTH, HEIGHT, "mlx");
	img.img = mlx_new_image(mlx, WIDTH, HEIGHT);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	img_fill(&img, 0, 0);


	put_map(&img, map, 10, 10);
	centor.start_x = player.x;
	centor.start_y = player.y;
	centor.end_x =   cos(player.rotationAngle) * 500;
	centor.end_y =   sin(player.rotationAngle) * 500;
	put_line(centor,&img);
	put_player(&img, player);


	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);

	return (1);
}