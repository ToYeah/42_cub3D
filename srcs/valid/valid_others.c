/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_others.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 13:41:40 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/05 19:06:44 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_validate.h"

t_status	valid_unknown(t_game *game, t_ident_line input)
{
	(void)game;
	(void)input;
	return (FAILURE);
}

t_status	valid_new_line(t_game *game, t_ident_line input)
{
	(void)input;
	if (game->config.map_flag == FALSE)
		return (NOT_ENTERED);
	return (FAILURE);
}
