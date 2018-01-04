/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/13 17:44:26 by amathias          #+#    #+#             */
/*   Updated: 2018/01/04 15:39:54 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

int		num_of_player_around(t_env *env, int team_num)
{
	int x;
	int y;
	int count;

	y = env->pos.y - 1;
	count = 0;
	while (y <= env->pos.y + 1)
	{
		x = env->pos.x - 1;
		while (x <= env->pos.x + 1)
		{
			if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
			{
				if (env->shared->board[y][x] == team_num)
					count++;
			}
			x++;
		}
		y++;
	}
	return (count);
}

int		is_trapped(t_env *env)
{
	int x;
	int y;

	y = env->pos.y - 1;
	while (y <= env->pos.y + 1)
	{
		x = env->pos.x - 1;
		while (x <= env->pos.x + 1)
		{
			if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
			{
				if (env->shared->board[y][x] != env->team_id
					&& env->shared->board[y][x] != 0
					&& num_of_player_around(env, env->shared->board[y][x]) >= 2)
					return (1);
			}
			x++;
		}
		y++;
	}
	return (0);
}

int		inbound(int x, int y)
{
	return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE);
}

int		is_pos_free(t_env *env, int x, int y)
{
	if (inbound(x, y) == 0)
		return (0);
	return (env->shared->board[y][x] == 0);
}

void	receive_loop(t_env *env)
{
	t_msg_ready	msg_ready;
	int			already_started;

	sem_wait(env->sem_board);
	already_started = env->shared->started;
	sem_post(env->sem_board);
	if (already_started
		|| msgrcv(env->msq_ready, &msg_ready, sizeof(msg_ready.ready), 46, 0)
			!= (ssize_t) - 1)
	{
		sem_wait(env->sem_board);
		if (env->shared->started == 0)
			env->shared->started = 1;
		sem_post(env->sem_board);
		game_loop(env);
	}
	else
		perr_exit("msgrcv");
}
