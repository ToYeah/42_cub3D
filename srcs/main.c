/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 06:26:02 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/06 19:03:18 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_engine.h"
#include "config_validate.h"

static t_bool	init_game(t_game **game)
{
	t_game *result;

	if (!(result = malloc(sizeof(t_game))))
		return (put_err_msg(strerror(errno)));
	ft_memset(result, 0, sizeof(t_game));
	if ((!(result->mlx = mlx_init())))
		return (put_err_msg(strerror(errno)));
	if (!(result->map = malloc_map(MAX_MAP_SIZE, MAX_MAP_SIZE)))
		return (put_err_msg(strerror(errno)));
	*game = result;
	return (TRUE);
}

int				main(int argc, char **argv)
{
	t_game *game;
	t_bool is_save;

	game = NULL;
	is_save = FALSE;
	if (!valid_runtime_arg(argc, argv, &is_save))
		exit_cub(game, EXIT_FAILURE);
	if (!init_game(&game))
		exit_cub(game, EXIT_FAILURE);
	if (!set_configuration(game, argv[1]))
		exit_cub(game, EXIT_FAILURE);
	if (!load_configuration(game, is_save))
		exit_cub(game, EXIT_FAILURE);
	if (is_save)
		save_screenshot(game);
	mlx_hook(game->win,
	X_EVENT_KEY_PRESS, (1L << 0), &press_key, game);
	mlx_hook(game->win,
	X_EVENT_KEY_RELEASE, (1L << 1), &release_key, game);
	mlx_hook(game->win,
	X_EVENT_CLIENT_MESSAGE, (1L << 17), &press_red_cross, game);
	mlx_loop_hook(game->mlx, &main_loop, game);
	mlx_loop(game->mlx);
}
