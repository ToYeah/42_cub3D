/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_engine.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 09:18:55 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/08 08:00:19 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_ENGINE_H
# define GAME_ENGINE_H

# include "cub_base.h"

# define PI 3.14159265358979323846
# define UP -1
# define DOWN 1
# define LEFT -1
# define RIGHT 1

# define COLUMUN_WIDTH 1
# define MINI_MAP_SCALE 0.5

# define FOV			60

# define KEY_ESC		65307
# define KEY_W			119
# define KEY_A			97
# define KEY_S			115
# define KEY_D			100
# define KEY_LEFT		65361
# define KEY_RIGHT		65363

# define X_EVENT_KEY_PRESS		2
# define X_EVENT_KEY_EXIT		17
# define X_EVENT_KEY_RELEASE		3
# define X_EVENT_CLIENT_MESSAGE	33

typedef struct	s_ray
{
	double		angle;
	int			horizontal_direction;
	int			vertical_direction;
	t_bool		select_horizontal;
	t_vector	collision_point;
}				t_ray;

typedef struct	s_wall
{
	int			win_x;
	int			win_y;
	int			wall_height;
	double		offset_x;
}				t_wall;

void			exit_cub(t_game *game, int status);
unsigned int	extract_color(t_texture *texture, int x, int y);
double			calc_sprite_theta(t_vector player_pos, t_vector sprite_pos);
double			calc_angle_diff(t_game *game, double angle, t_bool minus);
double			correct_angle(double angle);
double			normalized_angle(double angle);
double			calc_distance_vector(t_vector start, t_vector end);
double			calc_view_wall_height(t_game *game, double view_length);
double			calc_angle_step(int count, t_game *game);
int				press_key(int key_code, t_game *game);
int				release_key(int key_code, t_game *game);
int				press_red_cross(t_game *game);
int				validate_new_position(t_game *game, t_vector pos);
int				valid_collision_dir(t_game *g, t_vector p, int d, int h);
int				main_loop(t_game *game);
void			put_background(t_game *game);
void			put_sprites(t_game *game);
void			my_mlx_pixel_put(t_data *data, int x, int y, int color);
void			save_screenshot(t_game *game);
void			put_one_colmun(t_game *game, int i, t_ray *ray, double w);
void			cast_all_ray(t_game *game);
t_bool			load_configuration(t_game *game, t_bool is_save);
t_vector		vector_constructor(double x, double y);
t_collision		*malloc_collisions(t_game *game);
t_sprite		*malloc_sprite_ary(t_game *game);
char			**malloc_map(int x, int y);

#endif
