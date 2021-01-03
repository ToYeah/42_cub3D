/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_cub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 03:40:39 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/03 18:58:54 by totaisei         ###   ########.fr       */
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
	conf->map_flag = FALSE;
	conf->map_height = 0;
	conf->start_x = 0;
	conf->start_y = 0;
	conf->sprite_count = 0;
	conf->map_width = 0;

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

t_bool is_map_line(char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		if (!(ft_strchr(" 012NEWS", str[i])))
			return FALSE;
		i++;
	}
	return TRUE;
}

t_identifier select_identifier(char *line)
{
	if (!line)
		return UNKNOWN;
	if(*line == '\0')
		return NEWLINE;
	if (is_map_line(line))
		return MAP;
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
	int max_width;
	int max_height;
	if (*(input.status) != NOT_ENTERED || game->config.map_flag)
		return FAILURE;
	input.line += 1;
	input.line = ft_through_space(input.line);
	if (is_resolution_format(input.line))
	{
		mlx_get_screen_size(game->mlx, &max_width, &max_height);
		if (atoi_range(input.line, max_width, MIN_RESOLUTION_SIZE, &(game->config.window_width)))
		{
			input.line = ft_through_digit(input.line);
			input.line = ft_through_space(input.line);
			if (atoi_range(input.line, max_height, MIN_RESOLUTION_SIZE, &(game->config.window_height)))
				return ENTERED;
		}
	}
	return FAILURE;
}

t_status open_texture(t_game *game, t_texture *texture, t_ident_line input)
{
	input.line = ft_through_space(input.line);
	texture->img = mlx_xpm_file_to_image(game->mlx, input.line, &(texture->width), &(texture->height));
	if (texture->img != NULL)
	{
		texture->addr = mlx_get_data_addr(texture->img, &(texture->bits_per_pixel),
		&(texture->line_length), &(texture->endian));
		return ENTERED;
	}
	return FAILURE;
}

t_status valid_texture_path(t_game *game, t_ident_line input)
{
	if (*(input.status) != NOT_ENTERED || game->config.map_flag || ft_strrncmp(input.line, ".xpm", 4))
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
	if (*(input.status) != NOT_ENTERED || game->config.map_flag || !(is_color_format(input.line)))
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

t_status unknown_valid(t_game *game, t_ident_line input)
{
	return FAILURE;
}

t_status valid_new_line(t_game *game, t_ident_line input)
{
	if (game->config.map_flag == FALSE)
		return NOT_ENTERED;
	return FAILURE;
}

t_status valid_map(t_game *game, t_ident_line input)
{
	int i;

	i = 0;
	game->config.map_flag = TRUE;//NANKAI MO YOBARERU NO IYA YAWA
	if (*input.status == FAILURE || ft_strlen(input.line) > MAX_MAP_SIZE)
		return FAILURE;
	while (input.line[i])
	{
		game->map[game->config.map_height][i] = input.line[i];
		if (input.line[i] == 'N' || input.line[i] == 'E' || input.line[i] == 'W' || input.line[i] == 'S')
		{
			if (!(game->config.start_x == 0 && game->config.start_y == 0))
				return FAILURE;
			game->config.start_x = i;
			game->config.start_y = game->config.map_height;
			game->config.start_rotation = input.line[i];
		}
		if (input.line[i] == '2')
			game->config.sprite_count++;
		i++;
	}
	if (game->config.map_width < i)
		game->config.map_width = i;
	game->config.map_height++;
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
	valid_funcs[(int)MAP] = valid_map;
	valid_funcs[(int)NEWLINE] = valid_new_line;
	valid_funcs[(int)UNKNOWN] = unknown_valid;
}

t_bool cub_flood_fill(char **map, int x, int y)
{
	if (map[y][x] == '1' || map[y][x] == FLOOR_CHAR || map[y][x] == ITEM_CHAR)
		return TRUE;
	if (map[y][x] == ' ')
	{
		map[y][x] = 'x';
		return FALSE;
	}
	if (map[y][x] == '0')
		map[y][x] = FLOOR_CHAR;
	if (map[y][x] == '2')
		map[y][x] = ITEM_CHAR;
	if (cub_flood_fill(map, x + 1, y) && cub_flood_fill(map, x - 1, y) &&
	cub_flood_fill(map, x, y + 1) && cub_flood_fill(map, x, y - 1))
		return TRUE;
	return FALSE;
}

char *select_conf_err_msg(int ident)
{
	if (ident == (int)RESOLUTION)
		return "Invalid Resolution.";
	else if (ident == (int)NORTH)
		return "Invalid North texture.";
	else if (ident == (int)SOUTH)
		return "Invalid South texture.";
	else if (ident == (int)WEST)
		return "Invalid West texture.";
	else if (ident == (int)EAST)
		return "Invalid East texture.";
	else if (ident == (int)SPRITE)
		return "Invalid Sprite texture.";
	else if (ident == (int)FLOOR)
		return "Invalid Floor color.";
	else if (ident == (int)CEILLING)
		return "Invalid Ceiling texture.";
	else if (ident == (int)MAP)
		return "Invalid Map.";
	else if (ident == (int)NEWLINE)
		return "Invalid new line.";
	else
		return "Contains invalid line.";
}

t_bool put_err_msg(char *err_msg)
{
	ft_putendl_fd(DEF_ERR_MSG, 1);
	ft_putendl_fd(err_msg, 1);
	return FALSE;
}

t_bool valid_statuses(t_status *statuses)
{
	int i;

	i = 0;
	while(i < (int)UNKNOWN + 1)
	{
		if (statuses[i] == FAILURE)
			return put_err_msg(select_conf_err_msg(i));
		if (i < (int)NEWLINE && statuses[i] == NOT_ENTERED)
			return put_err_msg(select_conf_err_msg(i));
		i++;
	}
	return TRUE;
}

t_bool valid_cub(t_game *game, int fd)
{
	int				gnl_result;
	t_status		statuses[(int)UNKNOWN + 1];
	t_ident_line	ident_line;
	t_status		(*valid_funcs[(int)UNKNOWN + 1])(t_game *, t_ident_line);

	int i = 0;
	while(i < (int)UNKNOWN + 1)
		statuses[i++] = NOT_ENTERED;
	gnl_result = 1;
	init_config(&(game->config));
	init_valid_funcs(valid_funcs);
	while(gnl_result == 1)
	{
		gnl_result = ft_get_next_line(fd, &(ident_line.line));
		ident_line.ident = select_identifier(ident_line.line);
		ident_line.status = &(statuses[(int)ident_line.ident]);
		*(ident_line.status) = valid_funcs[(int)ident_line.ident](game, ident_line);
		ft_safe_free_char(&(ident_line.line));
	}
	if (gnl_result != 0)
		return put_err_msg(strerror(errno));
	return valid_statuses(statuses);
}

char **malloc_map(size_t x, size_t y)
{
	char **result;
	int i;

	i = 0;
	if (!(result = malloc(sizeof(char *) * y)))
		return NULL;
	while(i < y)
	{
		if (!(result[i] = malloc(x)))
		{
			i--;
			while(i >= 0)
			{
				ft_safe_free_char(&result[i]);
				i--;
			}
			free(result);
			result = NULL;
			return NULL;
		}
		ft_memset(result[i], ' ', x);
		i++;
	}
	return result;
}

void map_print(t_game *game)
{
	int i;

	i = 0;
	while(i < game->config.map_height)
	{
		fprintf(stderr, "%.*s\n", game->config.map_width,game->map[i]);
		i++;
	}
}

t_sprite *malloc_sprite_ary(t_game *game)
{
	int index_x;
	int index_y;
	int ary_index;
	t_sprite *result;

	index_y = 0;
	ary_index = 0;
	if(!(result = malloc(sizeof(t_sprite) * game->config.sprite_count)))
		return NULL;
	while(index_y < game->config.map_height)
	{
		index_x = 0;
		while(index_x < game->config.map_width)
		{
			if(game->map[index_y][index_x] == ITEM_CHAR)
			{
				result[ary_index].pos = vector_constructor
				(index_x * GRIDSIZE + GRIDSIZE / 2,index_y * GRIDSIZE + GRIDSIZE / 2);
				result[ary_index].distance = 0;
				ary_index++;
			}
			index_x++;
		}
		index_y++;
	}
	return result;
}

t_collision *malloc_collisions(t_game *game)
{
	t_collision *result;

	if(!(result = malloc(sizeof(t_collision) * game->config.window_width)))
		return (NULL);
	return result;
}

t_bool set_configuration(t_game *game, char *path)
{
	int fd;
	if((fd = open(path, O_RDONLY)) < 0)
		return put_err_msg(strerror(errno));
	if (!valid_cub(game, fd))
		return FALSE;
	if (game->config.start_x == 0 && game->config.start_y == 0)
		return put_err_msg("No starting point.");
	if (!cub_flood_fill(game->map, game->config.start_x, game->config.start_y))
		return put_err_msg("Unclosed map.");
	return TRUE;
}

t_bool valid_runtime_arg(int argc, char **argv, t_bool *is_save)
{
	if (argc == 1)
		return put_err_msg("too few arguments to cub3D.");
	if (argc > 3)
		return put_err_msg("Too many arguments to cub3D");
	if (argc > 1 && ft_strrncmp(argv[1], ".cub", 4))
		return put_err_msg("The second argument is incorrect");
	if (argc == 3 && ft_strncmp(argv[2], "--save", 7))
		return put_err_msg("The third argument is incorrect");
	else if (argc == 3)
		*is_save = TRUE;
	return TRUE;
}