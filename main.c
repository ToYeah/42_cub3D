/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 06:26:02 by totaisei          #+#    #+#             */
/*   Updated: 2020/12/16 05:04:10 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <X11/Xlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#include "./minilibx-linux/mlx.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0
#define PI 3.14159265358979323846
#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1

#define WIDTH 3
#define HEIGHT 400
#define MAP_WIDTH  10
#define MAP_HEIGHT 10

#define COLUMUN_WIDTH 1;
#define GRIDSIZE 32
#define MINI_MAP_SCALE 0.5
#define RAY_LENGTH 150;

#define FOV 60

#define KEY_ESC			65307
#define KEY_W			119
#define KEY_A			97
#define KEY_S			115
#define KEY_D			100
#define KEY_LEFT		65361
#define KEY_RIGHT		65363

#define X_EVENT_KEY_PRESS		2
#define X_EVENT_KEY_EXIT		17

int g_update = 1;
int g_raycount = WIDTH / COLUMUN_WIDTH;
double g_fov = FOV * (PI / 180);

char map[10][10] = 
{
	{'1','1','1','1','1','1','1','1','1','1'},
	{'1','0','0','0','0','0','0','0','0','1'},
	{'1','0','0','1','0','0','1','0','0','1'},
	{'1','1','1','0','0','0','1','1','0','1'},
	{'1','0','0','0','0','0','0','1','1','1'},
	{'1','0','1','0','0','0','0','0','0','1'},
	{'1','0','1','0','0','1','1','1','0','1'},
	{'1','0','1','0','0','0','0','0','0','1'},
	{'1','0','1','1','1','0','0','0','0','1'},
	{'1','1','1','1','1','1','1','1','1','1'}
};

typedef struct	s_vector
{
	double x;
	double y;
}				t_vector;

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
}				t_game;

typedef struct	s_ray
{
	double angle;
	int horizontal_direction;
	int vertical_direction;
	t_vector collision_point;
}				t_ray;

typedef struct	s_line
{
	int start_x;
	int start_y;
	int end_x;
	int end_y;
}				t_line;

void	init_player(t_player *player, double x, double y)
{
	player->pos.x = x;
	player->pos.y = y;
	player->radius = 3;
	player->turnDirection = 0;
	player->verticalDirection = 0;
	player->horizontalDirection = 0;
	player->rotationAngle = PI;
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
	int map_size = GRIDSIZE * MINI_MAP_SCALE;

	pos_x = x * map_size ;//+ x *5;
	pos_y = y * map_size ;//+ y *5;
	
	index_y = map_size;
	while(index_y != 0)
	{
		index_x = map_size;
		while (index_x != 0)
		{
			if (index_x == 1 || index_x == map_size || index_y == 1 || index_y == map_size || fill)
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
	int i;
	int j;

	i = 0;
	while(i < HEIGHT)
	{
		j = 0;
		while(j < WIDTH)
		{
			my_mlx_pixel_put(data, j, i, 0x00999999);
			j++;
		}
		i++;
	}
}

void put_line(t_data *data, t_vector start, t_vector end ,int color)
{
	int frac;
	int xNext, yNext, xStep, yStep, xDelta, yDelta;
	int xself, yself, xEnd, yEnd;
	xself = round(start.x);
	yself = round(start.y);
	xEnd = round(end.x);
	yEnd = round(end.y);
	xNext = xself;
	yNext = yself;
	xDelta = xEnd - xself;
	yDelta = yEnd - yself;
	xStep = 1;
	yStep = 1;
	if (xDelta < 0)
		xStep = -1;
	if (yDelta < 0)
		yStep = -1;
	xDelta = xDelta * 2;
	yDelta = yDelta * 2;
	if (xDelta < 0)
		xDelta = -xDelta;
	if (yDelta < 0)
		yDelta = -yDelta;
	my_mlx_pixel_put(data, xNext, yNext, color);
	// --A--
	if (xDelta > yDelta)
	{
		frac = yDelta * 2 - xDelta;
		while (xNext != xEnd)
		{
			if (frac >= 0)
			{
				yNext = yNext + yStep;
				frac = frac - xDelta;
			}
			xNext = xNext + xStep;
			frac = frac + yDelta;
			my_mlx_pixel_put(data, xNext, yNext, color);
		}
		// --B--
	}
	else
	{
		frac = xDelta * 2 - yDelta;
		while (yNext != yEnd)
		{
			if (frac >= 0)
			{
				xNext = xNext + xStep;
				frac = frac - yDelta;
			}
			yNext = yNext + yStep;
			frac = frac + xDelta;
			my_mlx_pixel_put(data, xNext, yNext, color);
		}
	}
}

void put_player(t_data *data, t_vector player_pos)
{
	int i;
	int j;

	i = 0;
	player_pos.x -= 1;
	player_pos.y -= 1;
	while(i < 10)
	{
		j = 0;
		while(j < 10)
		{
			my_mlx_pixel_put(data, (player_pos.x + j) * MINI_MAP_SCALE, (player_pos.y + i) * MINI_MAP_SCALE, 0x00FF0000);
			j++;
		}
		i++;
	}
}

void put_minimap(t_game *game, t_vector *rays)
{
	int i = 0;
	t_vector mini_player;
	t_vector mini_ray;

	put_map(game->data, map, 10, 10);
	while(i < g_raycount)
	{
		mini_player = game->player->pos;
		mini_player.x *= MINI_MAP_SCALE;
		mini_player.y *= MINI_MAP_SCALE;
		mini_ray = rays[i];
		mini_ray.x *= MINI_MAP_SCALE;
		mini_ray.y *= MINI_MAP_SCALE;
		put_line(game->data, mini_player, mini_ray, 0x00FF0000);
		i++;
	}
	put_player(game->data, game->player->pos);
}



int validate_collision(t_vector pos)// char **map;
{
	int x;
	int y;

	x = (floor(pos.x / GRIDSIZE));
	y = (floor(pos.y / GRIDSIZE));
	if(0 <= x && x < MAP_WIDTH && 0 <= y && y < MAP_HEIGHT && map[y][x] != '1')
		return TRUE;
	return FALSE;
}

int validate_collision_direction(t_vector pos, int direction, int horizontal)
{
	if (direction == UP && horizontal == TRUE)
		pos.y--;
	if (direction == LEFT && horizontal == False)
		pos.x--;
	return validate_collision(pos);
}

double	normalized_angle(double angle)
{
	int div;
	if(angle < 0 || angle > PI * 2)
	{
		div = (angle / (PI * 2));
		angle -= (div * PI * 2);

		if (angle < 0)
			angle += PI * 2;
	}

	return angle;
}

void 	update_player_pos(t_player *player)
{
	t_vector new_pos;

	player->rotationAngle += player->rotationSpeed * player->turnDirection;
	player->rotationAngle = normalized_angle(player->rotationAngle);
	new_pos.x = player->pos.x;
	new_pos.y = player->pos.y;
	new_pos.x += cos(player->rotationAngle) * player->verticalDirection * player->moveSpeed;
	new_pos.y += sin(player->rotationAngle) * player->verticalDirection * player->moveSpeed;
	new_pos.x += (cos(player->rotationAngle + PI/2)) * player->horizontalDirection * player->moveSpeed;
	new_pos.y += (sin(player->rotationAngle + PI/2)) * player->horizontalDirection * player->moveSpeed;
	if(validate_collision(new_pos))
		player->pos = new_pos;
}

double	calc_distance_vector(t_vector start, t_vector end)
{
	return (sqrt((start.x - end.x) *(start.x - end.x) + (start.y - end.y) *(start.y - end.y)));
}

int		calc_horizontal_intersection(t_vector player_pos, t_ray format, t_vector *result)
{
	t_vector intersection;
	t_vector collision_point;
	t_vector step;

	intersection.y = floor(player_pos.y / GRIDSIZE) * GRIDSIZE;
	if (format.vertical_direction == DOWN)
		intersection.y += GRIDSIZE;
	intersection.x = player_pos.x + (intersection.y - player_pos.y) / tan(format.angle);
	step.y = GRIDSIZE * format.vertical_direction;
	step.x = GRIDSIZE / tan(format.angle);
	if((step.x < 0 && format.horizontal_direction == RIGHT) || (step.x > 0 && format.horizontal_direction == LEFT))
		step.x *= -1;
	collision_point = intersection;
	while (collision_point.x >= 0 && collision_point.y >= 0 && collision_point.x <= WIDTH && collision_point.y <= HEIGHT)
	{
		if (validate_collision_direction(collision_point,format.vertical_direction, TRUE))
		{
			collision_point.x += step.x;
			collision_point.y += step.y;
		}
		else
		{
			*result = collision_point;
			return TRUE;
		}
	}
	*result = collision_point;
	return FALSE;
}

int	calc_vertical_intersection(t_vector player_pos, t_ray format, t_vector *result)
{
	t_vector intersection;
	t_vector collision_point;
	t_vector step;
	int flag;

	intersection.x = floor(player_pos.x / GRIDSIZE) * GRIDSIZE;
	if (format.horizontal_direction == RIGHT)
		intersection.x += GRIDSIZE;
	intersection.y = player_pos.y + (intersection.x - player_pos.x) * tan(format.angle);
	step.x = GRIDSIZE * format.horizontal_direction;
	step.y = GRIDSIZE * tan(format.angle);
	if((step.y < 0 && format.vertical_direction == DOWN) || (step.y > 0 && format.vertical_direction == UP))
		step.y *= -1;
	collision_point = intersection;
	while (collision_point.x >= 0 && collision_point.y >= 0 && collision_point.x <= WIDTH && collision_point.y <= HEIGHT)
	{
		if (validate_collision_direction(collision_point, format.horizontal_direction, FALSE))
		{
			collision_point.x += step.x;
			collision_point.y += step.y;
		}
		else
		{
			*result = collision_point;
			return TRUE;
		}
	}
	*result = collision_point;
	return FALSE;
}

t_vector	cast_single_ray(t_player player, double angle, t_game *game)//, char **map)
{
	t_vector horizontal_intersection;
	t_vector vertical_intersection;
	t_ray format;
	int horizontal_flag;
	int vertical_flag;

	format.angle = normalized_angle(angle);
	if(3.1415 < format.angle && format.angle < 3.1416)
		format.angle = 3.14;
	if(-0.0001 < format.angle && format.angle < 0.0001)
		format.angle = 0.001;
	format.vertical_direction = UP;
	if (format.angle > 0 && format.angle < PI)
		format.vertical_direction = DOWN;
	format.horizontal_direction = LEFT;
	if (format.angle < (PI / 2) || format.angle > (PI / 2 + PI))
		format.horizontal_direction = RIGHT;
	horizontal_flag = calc_horizontal_intersection(player.pos, format, &horizontal_intersection);
	vertical_flag = calc_vertical_intersection(player.pos, format, &vertical_intersection);

	if (horizontal_flag && !vertical_flag)
		return horizontal_intersection;
	if (!horizontal_flag && vertical_flag)
		return vertical_intersection;
	if (calc_distance_vector(player.pos, horizontal_intersection) >= calc_distance_vector(player.pos ,vertical_intersection))
		return vertical_intersection;
	else
		return horizontal_intersection;
	
}

void	cast_all_ray(t_game *game, t_vector *rays)
{
	int count;
	double angle;
	t_vector collision_point;

	count = 0;
	angle = game->player->rotationAngle - (g_fov / 2);
	while(count < g_raycount)
	{
		//collision_point = cast_single_ray(*(game->player), angle, game);
		//put_line(game->data, game->player->pos, collision_point, 0x000000FF);
		rays[count] = cast_single_ray(*(game->player), angle, game);
		angle += g_fov  / g_raycount;
		count++;
	}
	
}

void put_3D_wall(t_game *game, t_vector *rays)
{
	double view_plane_distance;
	double view_wall_height;
	t_vector wall_top;
	t_vector wall_bottom;

	int i = 0;

	view_plane_distance = (WIDTH / 2) / normalized_angle(tan(g_fov/2));
	while(i < g_raycount)
	{
		//view_wall_height = (GRIDSIZE / calc_distance_vector(game->player->pos, rays[i]) * view_plane_distance);
		view_wall_height = (GRIDSIZE / calc_distance_vector(game->player->pos, rays[i]) * view_plane_distance);
		
		wall_top.x = i;
		wall_bottom.x = i;
		wall_bottom.y = (HEIGHT/2) - (view_wall_height/2);
		wall_top.y = (HEIGHT/2) + (view_wall_height/2);
		if (!(calc_distance_vector(wall_bottom, wall_top) <= HEIGHT))
		{
			wall_top.y = 0;
			wall_bottom.y = HEIGHT;
		}
		put_line(game->data, wall_bottom, wall_top, (int)0xFFFFFFFF);
		i++;
	}
}

int		deal_key(int key_code, t_game *game)
{
	if (key_code == KEY_ESC)
		exit(0);
	else if (key_code == KEY_W)
		game->player->verticalDirection = +1;
	else if (key_code == KEY_S)
		game->player->verticalDirection = -1;
	else if (key_code == KEY_D)
		game->player->horizontalDirection = +1;
	else if (key_code == KEY_A)
		game->player->horizontalDirection = -1;
	else if (key_code == KEY_LEFT)
		game->player->turnDirection = -1;
	else if (key_code == KEY_RIGHT)
		game->player->turnDirection = +1;
	g_update = 1;
	return (0);
}

int	main_loop(t_game *game)
{
	t_vector centor;
	t_vector end;
	t_vector rays[g_raycount];
	int i = 0;

	if (g_update)
	{
		update_player_pos(game->player);
		img_fill(game->data, 0, 0);
		cast_all_ray(game, rays);
		put_3D_wall(game, rays);
		put_minimap(game, rays);
		mlx_put_image_to_window(game->mlx, game->win, game->data->img, 0, 0);
	}
	game->player->verticalDirection = 0;
	game->player->horizontalDirection = 0;
	game->player->turnDirection = 0;
	g_update = 0;
	return (0);
}

int main()
{
	t_data data;
	t_player player;
	t_game game;

	game.mlx = mlx_init();
	game.win = mlx_new_window(game.mlx, WIDTH, HEIGHT, "mlx");
	data.img = mlx_new_image(game.mlx, WIDTH, HEIGHT);
	data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel, &data.line_length, &data.endian);
	game.data = &data;
	init_player(&player, 50, 50);
	game.player = &player;

	mlx_hook(game.win, X_EVENT_KEY_PRESS, 1, &deal_key, &game);

	mlx_loop_hook(game.mlx, &main_loop, &game);
	mlx_loop(game.mlx);
}