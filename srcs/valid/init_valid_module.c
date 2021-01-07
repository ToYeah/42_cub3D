/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_valid_module.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 18:57:28 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/05 19:45:53 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_validate.h"

void	init_config(t_config *conf)
{
	conf->window_width = -1;
	conf->window_height = -1;
	conf->floor_color = -1;
	conf->ceiling_color = -1;
	conf->map_height = 0;
	conf->map_width = 0;
	conf->start_x = 0;
	conf->start_y = 0;
	conf->sprite_count = 0;
	conf->map_flag = FALSE;
	conf->is_save = FALSE;
	conf->north_texture.img = NULL;
	conf->south_texture.img = NULL;
	conf->west_texture.img = NULL;
	conf->east_texture.img = NULL;
	conf->sprite_texture.img = NULL;
}

void	init_valid_funcs(t_valid_func *funcs)
{
	funcs[(int)RESOLUTION] = valid_resolution;
	funcs[(int)NORTH] = valid_texture_path;
	funcs[(int)SOUTH] = valid_texture_path;
	funcs[(int)WEST] = valid_texture_path;
	funcs[(int)EAST] = valid_texture_path;
	funcs[(int)SPRITE] = valid_texture_path;
	funcs[(int)FLOOR] = valid_color;
	funcs[(int)CEILLING] = valid_color;
	funcs[(int)MAP] = valid_map;
	funcs[(int)NEWLINE] = valid_new_line;
	funcs[(int)UNKNOWN] = valid_unknown;
}
