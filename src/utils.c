/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 11:27:33 by amathias          #+#    #+#             */
/*   Updated: 2017/12/12 14:08:13 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	perr_exit(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	err_exit(char *msg)
{
	ft_putendl_fd(msg, 2);
	exit(EXIT_FAILURE);
}
