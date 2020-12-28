/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 06:26:02 by totaisei          #+#    #+#             */
/*   Updated: 2020/12/28 17:35:49 by totaisei         ###   ########.fr       */
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
#include "cub_base.h"

#define PI 3.14159265358979323846
#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1

#define COLUMUN_WIDTH 1;
#define MINI_MAP_SCALE 0.5

#define FOV				60
#define KEY_ESC			65307
#define KEY_W			119
#define KEY_A			97
#define KEY_S			115
#define KEY_D			100
#define KEY_LEFT		65361
#define KEY_RIGHT		65363

#define X_EVENT_KEY_PRESS		2
#define X_EVENT_KEY_EXIT		17

typedef struct	s_ray
{
	double angle;
	int horizontal_direction;
	int vertical_direction;
	t_vector collision_point;
	t_bool select_horizontal;
}				t_ray;

typedef struct	t_wall
{
	int	win_x;
	int	win_y;
	double offset_x;
	int	wall_height;
}				t_wall;

t_vector vector_constructor(double x, double y)
{
	t_vector result;
	result.x = x;
	result.y = y;
	return result;
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

double	select_start_angle(char c)
{
	if (c == 'S')
		return normalized_angle(PI / 2);
	if (c == 'E')
		return normalized_angle(2 * PI);
	if (c == 'W')
		return normalized_angle(PI);
	return normalized_angle(2 * PI - PI / 2);
}

void	init_player(t_player *player, t_config *conf)
{
	player->pos = vector_constructor(
	conf->start_x * GRIDSIZE + GRIDSIZE / 2,
	conf->start_y * GRIDSIZE + GRIDSIZE / 2);
	player->radius = 3;
	player->turnDirection = 0;
	player->verticalDirection = 0;
	player->horizontalDirection = 0;
	player->rotationAngle = select_start_angle(conf->start_rotation);
	player->moveSpeed = 10;
	player->rotationSpeed = 5 * (PI / 180);
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

unsigned int extract_color(t_texture *texture, int x, int y)
{
	return *(int *)((texture->addr) + (y * texture->line_length) + (x * (texture->bits_per_pixel / 8)));
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
			else
				my_mlx_pixel_put(data, index_x + pos_x, index_y + pos_y, 0x00FFFFFF);
			index_x--;
		}
		index_y--;
	}
}

void	put_map(t_game *game,char **map)
{
	int x;
	int y;

	y = 0;
	while(y < game->config.map_height)
	{
		x = 0;
		while(x < game->config.map_width)
		{
			if (map[y][x] == WALL_CHAR)
				put_square(&(game->data), x, y, TRUE);
			else if (map[y][x] != ' ')
				put_square(&(game->data), x, y, FALSE);
			x++;
		}
		y++;
	}
}

void	put_background(t_game *game)
{
	int i;
	int j;

	i = 0;
	while(i < game->config.window_height / 2)
	{
		j = 0;
		while(j < game->config.window_width)
		{
			my_mlx_pixel_put(&(game->data), j, i, game->config.ceiling_color);
			j++;
		}
		i++;
	}
	while(i < game->config.window_height)
	{
		j = 0;
		while(j < game->config.window_width)
		{
			my_mlx_pixel_put(&(game->data), j, i, game->config.floor_color);
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

void put_dot(t_data *data, t_vector player_pos, int color)
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
			my_mlx_pixel_put(data, (player_pos.x + j) * MINI_MAP_SCALE, (player_pos.y + i) * MINI_MAP_SCALE, color);
			j++;
		}
		i++;
	}
}

double	correct_angle(double angle)
{
	if(3.1415 < angle && angle < 3.1416)
		angle = 3.14;
	if(-0.0001 < angle && angle < 0.0001)
		angle = 0.001;
	return angle;
}

void put_miniray(t_game *game, double angle, double length, int color)
{
	t_vector end_point;
	end_point = vector_constructor(
	game->player.pos.x * MINI_MAP_SCALE + length * cos(angle),
	game->player.pos.y * MINI_MAP_SCALE + length * sin(angle));
	put_line(&(game->data), 
	vector_constructor(game->player.pos.x * MINI_MAP_SCALE,
	game->player.pos.y * MINI_MAP_SCALE),
	end_point, color);
}

void put_minimap(t_game *game)
{
	int i = 0;
	t_vector player_direction;
	put_map(game, game->map);

	
	put_dot(&(game->data), game->player.pos, 0x00FF0000);
	put_miniray(game, game->player.rotationAngle, 30, 0x00FF0000);
	put_miniray(game, normalized_angle(game->player.rotationAngle - (game->fov / 2)),
	30, 0x00FF0000);
	put_miniray(game, normalized_angle(game->player.rotationAngle + (game->fov / 2)),
	30, 0x00FF0000);

	i = 0;
	while (i < game->config.sprite_count)
	{
		//put_dot(&(game->data), game->sprite_pos[i], 0x000000FF);
		i++;
	}
	
}

int validate_collision(t_game *game, t_vector pos)// char **map;
{
	int x;
	int y;

	x = (floor(pos.x / GRIDSIZE));
	y = (floor(pos.y / GRIDSIZE));
	if(0 <= x && x < MAX_MAP_SIZE && 0 <= y && y < MAX_MAP_SIZE && game->map[y][x] != WALL_CHAR)
		return TRUE;
	return FALSE;
}

int validate_collision_direction(t_game *game, t_vector pos, int dir, int horizontal)
{
	if (dir == UP && horizontal == TRUE)
		pos.y--;
	if (dir == LEFT && horizontal == False)
		pos.x--;
	return validate_collision(game, pos);
}

void 	update_player_pos(t_game *game, t_player *player)
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
	if(validate_collision(game, new_pos))
		player->pos = new_pos;
}

double	calc_distance_vector(t_vector start, t_vector end)
{
	return (sqrt((start.x - end.x) *(start.x - end.x) + (start.y - end.y) *(start.y - end.y)));
}

t_bool	calc_horizontal_intersection(t_game *game, t_vector player_pos, t_ray *ray, t_vector *result)
{
	t_vector intersection;
	t_vector collision_point;
	t_vector step;

	intersection.y = floor(player_pos.y / GRIDSIZE) * GRIDSIZE;
	if (ray->vertical_direction == DOWN)
		intersection.y += GRIDSIZE;
	intersection.x = player_pos.x + (intersection.y - player_pos.y) / tan(ray->angle);
	step.y = GRIDSIZE * ray->vertical_direction;
	step.x = GRIDSIZE / tan(ray->angle);
	if((step.x < 0 && ray->horizontal_direction == RIGHT) || (step.x > 0 && ray->horizontal_direction == LEFT))
		step.x *= -1;
	collision_point = intersection;
	while (collision_point.x >= 0 && collision_point.y >= 0 && collision_point.x <= MAX_MAP_SIZE * GRIDSIZE && collision_point.y <= MAX_MAP_SIZE * GRIDSIZE)
	{
		if (validate_collision_direction(game, collision_point,ray->vertical_direction, TRUE))
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

t_bool	calc_vertical_intersection(t_game *game, t_vector player_pos, t_ray *ray, t_vector *result)
{
	t_vector intersection;
	t_vector collision_point;
	t_vector step;
	int flag;

	intersection.x = floor(player_pos.x / GRIDSIZE) * GRIDSIZE;
	if (ray->horizontal_direction == RIGHT)
		intersection.x += GRIDSIZE;
	intersection.y = player_pos.y + (intersection.x - player_pos.x) * tan(ray->angle);
	step.x = GRIDSIZE * ray->horizontal_direction;
	step.y = GRIDSIZE * tan(ray->angle);
	if((step.y < 0 && ray->vertical_direction == DOWN) || (step.y > 0 && ray->vertical_direction == UP))
		step.y *= -1;
	collision_point = intersection;
	while (collision_point.x >= 0 && collision_point.y >= 0 && collision_point.x <= MAX_MAP_SIZE * GRIDSIZE && collision_point.y <= MAX_MAP_SIZE * GRIDSIZE)
	{
		if (validate_collision_direction(game, collision_point, ray->horizontal_direction, FALSE))
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

void	set_ray_direction(t_ray *ray)
{
	ray->vertical_direction = UP;
	if (ray->angle > 0 && ray->angle < PI)
		ray->vertical_direction = DOWN;
	ray->horizontal_direction = LEFT;
	if (ray->angle < (PI / 2) || ray->angle > (PI / 2 + PI))
		ray->horizontal_direction = RIGHT;
}

void	cast_single_ray(t_game *game, t_ray *ray)//, char **map)
{
	t_vector vertical_intersection;
	t_bool horizontal_flag;

	ray->angle = correct_angle(normalized_angle(ray->angle));
	set_ray_direction(ray);
	ray->select_horizontal = TRUE;
	horizontal_flag =
	calc_horizontal_intersection(game, game->player.pos, ray, &(ray->collision_point));
	calc_vertical_intersection(game, game->player.pos, ray, &vertical_intersection);
	if ((!horizontal_flag ||
	calc_distance_vector(game->player.pos, ray->collision_point) >= 
	calc_distance_vector(game->player.pos ,vertical_intersection)))
	{
		ray->collision_point = vertical_intersection;
		ray->select_horizontal = FALSE;
	}
}

void	set_wall_pos(t_ray *ray, double wall_height, int x, t_wall *wall)
{
	double tmp;

	wall->wall_height = round(wall_height);
	wall->win_x = x;
	wall->win_y = 0;
	if (ray->select_horizontal == TRUE)
		tmp = fmod(ray->collision_point.x, GRIDSIZE);
	else
		tmp = fmod(ray->collision_point.y, GRIDSIZE);
	wall->offset_x = (tmp / 32);
}

void put_wall_texture(t_game *game, t_wall *wall, t_texture *texture)
{
	t_vector tex;
	double offset;
	int wall_y;
	int color;

	offset = (game->config.window_height / 2) - (wall->wall_height / 2);
	tex.x = wall->offset_x * texture->width;
	tex.y = 0;
	wall_y = 0;
	wall->win_y = offset + wall_y;
	while(wall_y < wall->wall_height && wall->win_y < game->config.window_height)
	{
		if (0 <= wall->win_y && wall->win_y <= game->config.window_height)
		{
			tex.y = (((double)wall_y / (double)wall->wall_height) * texture->height);
			color = extract_color(texture, (int)tex.x, (int)tex.y);
			my_mlx_pixel_put(&(game->data), wall->win_x, wall->win_y, color);
		}
		wall_y++;
		wall->win_y = offset + wall_y;
	}
}

void put_one_colmun(t_game *game, int i, double wall_height, t_ray *ray)
{
	t_wall wall;

	set_wall_pos(ray, wall_height, i, &wall);
	if(ray->vertical_direction == UP && ray->select_horizontal == TRUE)
		put_wall_texture(game, &wall, &(game->config.south_texture));
	else if(ray->vertical_direction == DOWN && ray->select_horizontal == TRUE)
		put_wall_texture(game, &wall, &(game->config.north_texture));
	else if(ray->horizontal_direction == LEFT && ray->select_horizontal == FALSE)
		put_wall_texture(game, &wall, &(game->config.east_texture));
	else
		put_wall_texture(game, &wall, &(game->config.west_texture));
}

void	cast_all_ray(t_game *game)
{
	int count;
	t_vector collision_point;
	t_ray ray;
	double view_plane_distance;
	double view_wall_height;

	count = 0;
	ray.angle = game->player.rotationAngle - (game->fov / 2);
	view_plane_distance = (game->config.window_width / 2) / normalized_angle(tan(game->fov/2));
	while(count < game->ray_max)
	{
		cast_single_ray(game, &ray);
		view_wall_height = (GRIDSIZE / (calc_distance_vector(game->player.pos, ray.collision_point) *
		cos(ray.angle - game->player.rotationAngle)) * view_plane_distance);
		
		//put_miniray(game, ray.angle, calc_distance_vector(game->player.pos, ray.collision_point) * MINI_MAP_SCALE,
		//0x00FF0000);
		
		put_one_colmun(game, count, view_wall_height, &ray);
		ray.angle += game->fov / game->ray_max;
		count++;
	}
	
}

double	calc_sprite_theta(t_vector *player_pos, t_vector *sprite_pos)
{
	t_vector diff;
	double result;

	diff.x = player_pos->x - sprite_pos->x;
	diff.y = player_pos->y - sprite_pos->y;

	result = atan2(diff.y , diff.x) + PI;
	return result;
}

t_bool	is_in_sight_sprite(t_game *game, double angle, double width)
{
	double l_limit;
	double r_limit;
	double sprite_left;
	double sprite_right;

	l_limit = normalized_angle(game->player.rotationAngle - (game->fov / 2));
	r_limit = l_limit + game->fov;
	sprite_left = angle + width;
	sprite_right = angle - width;
	if(l_limit <= sprite_right && sprite_right <= r_limit)
		return TRUE;
	if(l_limit <= sprite_left && sprite_left <= r_limit)
		return TRUE;
	return FALSE;
}

void	put_sprites(t_game *game)
{
	int i;
	double sprite_angle;
	double angle_width;

	i = 0;
	while(i < game->config.sprite_count)
	{
		sprite_angle = calc_sprite_theta(&(game->player.pos), &(game->sprite_pos[i]));
		angle_width = normalized_angle(atan2(GRIDSIZE / 2 ,
		calc_distance_vector(game->player.pos, game->sprite_pos[i])));

		fprintf(stderr, "ply:%lf\n", game->player.rotationAngle);
		if(is_in_sight_sprite(game, sprite_angle, angle_width))
		{
			put_dot(&(game->data), game->sprite_pos[i], 0x000000FF);
		//put_miniray(game, normalized_angle(sprite_angle + angle_width),
		//calc_distance_vector(game->player.pos, game->sprite_pos[i]) * MINI_MAP_SCALE
		//, 0x000000FF);
		//put_miniray(game, normalized_angle(sprite_angle - angle_width),
		//calc_distance_vector(game->player.pos, game->sprite_pos[i]) * MINI_MAP_SCALE
		//, 0x000000FF);
		}
		i++;
	}
}

int		deal_key(int key_code, t_game *game)
{
	if (key_code == KEY_ESC)
		exit(0);
	else if (key_code == KEY_W)
		game->player.verticalDirection = 1;
	else if (key_code == KEY_S)
		game->player.verticalDirection = -1;
	else if (key_code == KEY_D)
		game->player.horizontalDirection = 1;
	else if (key_code == KEY_A)
		game->player.horizontalDirection = -1;
	else if (key_code == KEY_LEFT)
		game->player.turnDirection = -1;
	else if (key_code == KEY_RIGHT)
		game->player.turnDirection = 1;
	game->update = TRUE;
	return (0);
}

int	main_loop(t_game *game)
{
	t_vector centor;
	t_vector end;
	int i = 0;

	if (game->update)
	{
		update_player_pos(game, &(game->player));
		put_background(game);
		cast_all_ray(game);
		put_minimap(game);
		put_sprites(game);
		mlx_put_image_to_window(game->mlx, game->win, game->data.img, 0, 0);
	}
	game->player.verticalDirection = 0;
	game->player.horizontalDirection = 0;
	game->player.turnDirection = 0;
	game->update = 0;
	return (0);
}

void map_free(char **map)
{
	int i;

	i = 0;
	while(i < MAX_MAP_SIZE)
	{
		free(map[i]);
		map[i] = NULL;
		i++;
	}
	free(map);
	map = NULL;
}

t_bool init_game(t_game **game)
{
	t_game *result;
	
	if (!(result = malloc(sizeof(t_game))))
		return FALSE;
	ft_memset(result, 0, sizeof(t_game));
	if ((!(result->mlx = mlx_init())))
	{
		free(result);
		return FALSE;
	}
	if (!(result->map = malloc_map(MAX_MAP_SIZE, MAX_MAP_SIZE)))
	{
		mlx_destroy_display(result->mlx);
		free(result);
		return FALSE;
	}
	*game = result;
	return TRUE;
}

void	free_mlx_conf(t_game *game)
{
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->data.img)
		mlx_destroy_image(game->mlx, game->data.img);
	if (game->config.north_texture.img)
		mlx_destroy_image(game->mlx, game->config.north_texture.img);
	if (game->config.east_texture.img)
		mlx_destroy_image(game->mlx, game->config.east_texture.img);
	if (game->config.west_texture.img)
		mlx_destroy_image(game->mlx, game->config.west_texture.img);
	if (game->config.south_texture.img)
		mlx_destroy_image(game->mlx, game->config.south_texture.img);
	if (game->config.sprite_texture.img)
		mlx_destroy_image(game->mlx, game->config.sprite_texture.img);
}

int free_all_cub(t_game *game)
{
	if (game)
	{
		if (game->map)
		{
			if (game->mlx)
			{
				free_mlx_conf(game);
				mlx_destroy_display(game->mlx);
				free(game->mlx);
			}
			map_free(game->map);
		}
		free(game);
	}
	return 1;
}

t_bool load_configuration(t_game *game)
{
	game->win = mlx_new_window(game->mlx, game->config.window_width, game->config.window_height, "mlx");// error
	game->data.img = mlx_new_image(game->mlx, game->config.window_width, game->config.window_height);// error
	game->sprite_pos = malloc_sprite_ary(game);

	
	game->data.addr = mlx_get_data_addr(game->data.img, &(game->data.bits_per_pixel), &(game->data.line_length), &(game->data.endian));
	game->fov = FOV * (PI / 180);
	game->ray_max = game->config.window_width / COLUMUN_WIDTH;
	game->update = TRUE;
	init_player(&(game->player), &(game->config));
	return TRUE;
}

void exit_cub(t_game *game, int status)
{
	free_all_cub(game);
	exit(status);
}

int main(int argc, char **argv)
{
	t_game *game;

	game = NULL;
	if (!valid_runtime_arg(argc, argv))
		exit_cub(game, EX_USAGE);
	if (!init_game(&game))
		return free_all_cub(game);
	if (!set_configuration(game, argv[1]))
		return free_all_cub(game);
	if (!load_configuration(game))
		fprintf(stderr,"a");
	
	mlx_hook(game->win, X_EVENT_KEY_PRESS, (1L<<0), &deal_key, game);
	mlx_loop_hook(game->mlx, &main_loop, game);
	mlx_loop(game->mlx);
}
