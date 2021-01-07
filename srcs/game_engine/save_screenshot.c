/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_screenshot.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 08:44:06 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/04 16:20:30 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_engine.h"

static void	write_bmp_file(t_game *game, int fd)
{
	unsigned int size;
	unsigned int offset;

	offset = FILE_H_SIZE + INFO_H_SIZE;
	size = offset + (game->config.window_height *
	game->config.window_width * 4);
	write(fd, "BM", 2);
	write(fd, &size, 4);
	write(fd, "\0\0", 2);
	write(fd, "\0\0", 2);
	write(fd, &offset, 4);
}

static void	write_bmp_information(t_game *game, int fd)
{
	unsigned int	info_size;
	unsigned int	plane;
	unsigned int	bpp;
	int				i;

	i = 0;
	info_size = INFO_H_SIZE;
	plane = 1;
	bpp = 32;
	write(fd, &info_size, 4);
	write(fd, &(game->config.window_width), 4);
	write(fd, &(game->config.window_height), 4);
	write(fd, &plane, 2);
	write(fd, &bpp, 2);
	while (i++ < 24)
		write(fd, "\0", 1);
}

static void	write_bmp_pixel(t_game *game, int fd)
{
	int index_x;
	int index_y;
	int *color_p;
	int color;

	index_y = 0;
	while (index_y < game->config.window_height)
	{
		index_x = 0;
		while (index_x < game->config.window_width)
		{
			color_p = (int *)(game->data.addr +
			((game->config.window_height - index_y - 1) *
			(game->data.line_length)) +
			index_x * (game->data.bits_per_pixel / 8));
			color = *color_p;
			write(fd, &color, 4);
			index_x++;
		}
		index_y++;
	}
}

void		save_screenshot(t_game *game)
{
	int fd;

	put_background(game);
	cast_all_ray(game);
	put_sprites(game);
	fd = open("./screenshot.bmp",
	O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd < 0)
	{
		put_err_msg(strerror(errno));
		exit_cub(game, EXIT_FAILURE);
	}
	write_bmp_file(game, fd);
	write_bmp_information(game, fd);
	write_bmp_pixel(game, fd);
	close(fd);
	exit_cub(game, EXIT_SUCCESS);
}
