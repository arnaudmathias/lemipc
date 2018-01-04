/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 11:27:33 by amathias          #+#    #+#             */
/*   Updated: 2018/01/04 15:00:29 by amathias         ###   ########.fr       */
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

void	game_display(t_env *env)
{
	int i;
	int j;

	i = 0;
	while (i < BOARD_SIZE)
	{
		j = 0;
		while (j < BOARD_SIZE)
		{
			if (env->shared->board[i][j] == 0)
				ft_putchar('.');
			else
				ft_putchar(env->shared->board[i][j]);
			j++;
		}
		ft_putchar('\n');
		i++;
	}
}

int		is_empty(t_env *env)
{
	int i;
	int j;

	i = 0;
	while (i < BOARD_SIZE)
	{
		j = 0;
		while (j < BOARD_SIZE)
		{
			if (env->shared->board[i][j] != 0)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int		is_full(t_env *env)
{
	int i;
	int j;

	i = 0;
	while (i < BOARD_SIZE)
	{
		j = 0;
		while (j < BOARD_SIZE)
		{
			if (env->shared->board[i][j] == 0)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}
