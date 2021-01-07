/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_statuses.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 13:53:38 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/04 17:07:48 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_validate.h"

static char	*select_conf_err_msg(int ident)
{
	if (ident == (int)RESOLUTION)
		return ("Invalid Resolution.");
	else if (ident == (int)NORTH)
		return ("Invalid North texture.");
	else if (ident == (int)SOUTH)
		return ("Invalid South texture.");
	else if (ident == (int)WEST)
		return ("Invalid West texture.");
	else if (ident == (int)EAST)
		return ("Invalid East texture.");
	else if (ident == (int)SPRITE)
		return ("Invalid Sprite texture.");
	else if (ident == (int)FLOOR)
		return ("Invalid Floor color.");
	else if (ident == (int)CEILLING)
		return ("Invalid Ceiling texture.");
	else if (ident == (int)MAP)
		return ("Invalid Map.");
	else if (ident == (int)NEWLINE)
		return ("Invalid new line.");
	else
		return ("Contains invalid line.");
}

t_bool		valid_statuses(t_status *statuses)
{
	int i;

	i = 0;
	while (i < (int)UNKNOWN + 1)
	{
		if (statuses[i] == FAILURE)
			return (put_err_msg(select_conf_err_msg(i)));
		if (i < (int)NEWLINE && statuses[i] == NOT_ENTERED)
			return (put_err_msg(select_conf_err_msg(i)));
		i++;
	}
	return (TRUE);
}
