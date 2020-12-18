/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_cub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 03:40:39 by totaisei          #+#    #+#             */
/*   Updated: 2020/12/18 16:39:51 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub_base.h"

#define MAX_RESOLUTION_SIZE 100000
#define MIN_RESOLUTION_SIZE 100

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
	char 			*line;
}				t_ident_line;


void init_config(t_config *conf)
{
	conf->window_width = -1;
	conf->window_height = -1;
	conf->north_texture.img = NULL;
	conf->south_texture.img = NULL;
	conf->west_texture.img = NULL;
	conf->east_texture.img = NULL;
	conf->sprite_texture.img = NULL;
	conf->floor_color = -1;
	conf->ceiling_color = -1;
}


char	*ft_through_space(char *str)
{
	if (!str)
		return NULL;
	while(*str && *str == ' ')
		str++;
	return str;
}

char	*ft_through_digit(char *str)
{
	if (!str)
		return NULL;
	while(*str && ft_isdigit(*str))
		str++;
	return str;
}

t_identifier find_identifier(char *line)
{
	if (!line)
		return UNKNOWN;
	if(*line == '\0')
		return NEWLINE;
	line = ft_through_space(line);
	if(ft_strncmp(line, "R ", 2) == 0)
		return RESOLUTION;
	if(ft_strncmp(line, "NO ", 3) == 0)
		return NORTH;
	if(ft_strncmp(line, "SO ", 3) == 0)
		return SOUTH;
	if(ft_strncmp(line, "WE ", 3) == 0)
		return WEST;
	if(ft_strncmp(line, "EA ", 3) == 0)
		return EAST;
	if(ft_strncmp(line, "S ", 2) == 0)
		return SPRITE;
	if(ft_strncmp(line, "F ", 2) == 0)
		return FLOOR;
	if(ft_strncmp(line, "C ", 2) == 0)
		return CEILLING;
	return UNKNOWN;
}

char	*one_char_compare(char *str,unsigned char c)
{
	if (!str)
		return NULL;
	if ((unsigned char)*str == c)
		return (str + 1);
	return NULL;
}

t_bool is_resolution_format(char *str)
{
	str = ft_through_space(str);
	if (!ft_isdigit(*str))
		return FALSE;
	str = ft_through_digit(str);
	if (!(str = one_char_compare(str, ' ')))
		return FALSE;
	str = ft_through_space(str);
	if (!ft_isdigit(*str))
		return FALSE;
	str = ft_through_digit(str);
	if (*str != '\0')
		return FALSE;
	return TRUE;
}

t_bool atoi_range(char *str, int max, int min, int *result)
{
	*result = 0;
	while (ft_isdigit(*str) && *result < max)
	{
		*result *= 10;
		*result += (*str - '0');
		str++;
	}
	if (*result < min)
	{
		*result = -1;
		return FALSE;
	}
	if (!(*result < max))
	{
		*result = max;
		return TRUE;
	}
	return TRUE;
}

t_status valid_resolution(t_game *game, t_ident_line input)
{
	if (*(input.status) != NOT_ENTERED)
		return FAILURE;
	input.line += 1;
	input.line = ft_through_space(input.line);
	if (is_resolution_format(input.line))
	{
		if (atoi_range(input.line, game->config.max_width, MIN_RESOLUTION_SIZE, &(game->config.window_width)))
		{
			input.line = ft_through_digit(input.line);
			input.line = ft_through_space(input.line);
			if (atoi_range(input.line, game->config.max_height, MIN_RESOLUTION_SIZE, &(game->config.window_height)))
				return ENTERED;
		}
	}
	return FAILURE;
}

t_status open_texture(t_game *game, t_texture *texture, t_ident_line input)
{
	input.line = ft_through_space(input.line);
	texture->img = mlx_xpm_file_to_image(game->mlx, input.line, &(texture->img_width), &(texture->img_height));
	if (texture->img != NULL)
		return ENTERED;
	return FAILURE;
}

t_status valid_texture_path(t_game *game, t_ident_line input)
{
	if (*(input.status) != NOT_ENTERED)
		return FAILURE;
	while(*input.line != ' ')
		input.line++;
	if (input.ident == NORTH)
		return open_texture(game, &(game->config.north_texture), input);
	if (input.ident == SOUTH) 
		return open_texture(game, &(game->config.south_texture), input);
	if (input.ident == WEST)
		return open_texture(game, &(game->config.west_texture), input);
	if (input.ident == EAST)
		return open_texture(game, &(game->config.east_texture), input);
	if (input.ident == SPRITE)
		return open_texture(game, &(game->config.sprite_texture), input);
	return FAILURE;
}

char *is_three_nums(char *str)
{
	int count;

	count = 0;
	while (*str && ft_isdigit(*str))
	{
		str++;
		count++;
	}
	if (count == 1 || count == 2 || count == 3)
		return str;
	return NULL;
}

int int_to_color(int a, int r, int g, int b)
{
	int result;

	result = 0 | ((unsigned char)a << 24) | ((unsigned char)r << 16) | ((unsigned char)g << 8) | ((unsigned char)b);
	return result; 
}

t_bool is_color_format(char *str)
{
	if (!(str = is_three_nums(str)) || !(str = one_char_compare(str, ',')))
			return FALSE;
	if (!(str = is_three_nums(str)) || !(str = one_char_compare(str, ',')))
			return FALSE;
	if (!(str = is_three_nums(str)) || *str != '\0')
			return FALSE;
	return TRUE;
}

t_status valid_color(t_game *game, t_ident_line input)
{
	int parameter[3];

	input.line += 1;
	input.line = ft_through_space(input.line);
	if (*(input.status) != NOT_ENTERED || !(is_color_format(input.line)))
		return FAILURE;
	parameter[0] = ft_atoi(input.line);
	input.line = ft_through_digit(input.line);
	parameter[1] = ft_atoi(++input.line);
	input.line = ft_through_digit(input.line);
	parameter[2] = ft_atoi(++input.line);
	if(!(parameter[0] <= 0xFF && parameter[1] <= 0xFF && parameter[2] <= 0xFF))
		return FAILURE;
	if(input.ident == FLOOR)
		game->config.floor_color = int_to_color(0, parameter[0], parameter[1], parameter[2]);
	else
		game->config.ceiling_color = int_to_color(0, parameter[0], parameter[1], parameter[2]);
	return ENTERED;
}

t_status tmp_valid(t_game *game, t_ident_line input)
{
	return ENTERED;
}

void init_valid_funcs(t_status	(**valid_funcs)(t_game *, t_ident_line))
{
	valid_funcs[(int)RESOLUTION] = valid_resolution;
	valid_funcs[(int)NORTH] = valid_texture_path;
	valid_funcs[(int)SOUTH] = valid_texture_path;
	valid_funcs[(int)WEST] = valid_texture_path;
	valid_funcs[(int)EAST] = valid_texture_path;
	valid_funcs[(int)SPRITE]  = valid_texture_path;
	valid_funcs[(int)FLOOR] = valid_color;
	valid_funcs[(int)CEILLING] = valid_color;
	valid_funcs[(int)MAP] = tmp_valid;
	valid_funcs[(int)NEWLINE] = tmp_valid;
	valid_funcs[(int)UNKNOWN] = tmp_valid;
}

t_bool valid_cub(t_game *game, char *path)
{
	int gnl_result;
	int fd;
	t_status statuses[(int)UNKNOWN + 1];
	t_ident_line ident_line;
	t_status	(*valid_funcs[(int)UNKNOWN + 1])(t_game *, t_ident_line);


	int i = 0;
	while(i < (int)UNKNOWN + 1)
		statuses[i++] = NOT_ENTERED;

	gnl_result = 1;
	init_config(&(game->config));

	if((fd = open(path, O_RDONLY)) < 0)
		return 1;//error
	init_valid_funcs(valid_funcs);

	while(gnl_result == 1) // TODO if gnl_res == -1
	{
		gnl_result = ft_get_next_line(fd, &(ident_line.line));
		ident_line.ident = find_identifier(ident_line.line);
		ident_line.status = &(statuses[(int)ident_line.ident]);
		*(ident_line.status) = valid_funcs[(int)ident_line.ident](game, ident_line);
		if(ident_line.ident != UNKNOWN)
			fprintf(stderr,"%3d | %d | %s\n",ident_line.ident, *ident_line.status, ident_line.line);
		ft_safe_free(&(ident_line.line));
	}
	i = 0;
	while(i < (int)UNKNOWN + 1)
	{
		if (statuses[i++] != ENTERED)
			return FALSE;
	}
	return TRUE;
}

int main(int argc, char **argv)
{
	t_data data;
	t_game game;
	char map[200][200];

	game.mlx = mlx_init();
	//game.win = mlx_new_window(game.mlx, 500, 500, "mlx");
	//data.img = mlx_new_image(game.mlx, 500, 500);
	//data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel, &data.line_length, &data.endian);
	//game.data = &data;
	//if(argc != 2)
	//	return 1;//error
	mlx_get_screen_size(game.mlx, &(game.config.max_width), &(game.config.max_height));
	t_bool a = valid_cub(&game, argv[1]);
	fprintf(stderr, "%d\n", a);
	fprintf(stderr, "width:%d height:%d\n", game.config.window_width, game.config.window_height);

	mlx_loop(game.mlx);
}