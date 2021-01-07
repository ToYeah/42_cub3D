/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_err_msg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totaisei <totaisei@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 13:28:19 by totaisei          #+#    #+#             */
/*   Updated: 2021/01/05 18:51:39 by totaisei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub_base.h"

t_bool	put_err_msg(char *err_msg)
{
	ft_putendl_fd(DEF_ERR_MSG, 1);
	ft_putendl_fd(err_msg, 1);
	return (FALSE);
}
