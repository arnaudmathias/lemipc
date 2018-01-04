/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 11:31:18 by amathias          #+#    #+#             */
/*   Updated: 2018/01/04 14:57:30 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

t_pos	find_first_available(t_env *env)
{
	int y;
	int x;

	y = 0;
	while (y < BOARD_SIZE)
	{
		x = 0;
		while (x < BOARD_SIZE)
		{
			if (env->shared->board[y][x] == 0)
				return (get_pos(x, y));
			x++;
		}
		y++;
	}
	return (get_pos(0, 0));
}

int		place_player(t_env *env)
{
	t_pos	pos;
	int		try;

	try = 0;
	if (is_full(env))
		return (0);
	while (try < 2)
	{
		pos.x = rand() % (BOARD_SIZE);
		pos.y = rand() % (BOARD_SIZE);
		if (is_pos_free(env, pos.x, pos.y))
		{
			env->pos = pos;
			env->shared->board[pos.y][pos.x] = env->team_id;
			return (1);
		}
		try++;
	}
	env->pos = find_first_available(env);
	env->shared->board[env->pos.y][env->pos.x] = env->team_id;
	return (1);
}

int		has_win(t_env *env)
{
	int i;
	int j;

	i = 0;
	while (i < BOARD_SIZE)
	{
		j = 0;
		while (j < BOARD_SIZE)
		{
			if (env->shared->board[i][j] != 0
				&& env->shared->board[i][j] != env->team_id)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

void	move(t_env *env)
{
	t_pos	new_pos;

	new_pos = find_path(env, env->pos, env->target);
	if (is_pos_free(env, new_pos.x, new_pos.y))
	{
		env->shared->board[env->pos.y][env->pos.x] = 0;
		env->shared->board[new_pos.y][new_pos.x] = env->team_id;
		env->pos = new_pos;
	}
}

void	game_loop(t_env *env)
{
	while (42)
	{
		sem_wait(env->sem_board);
		update_target(env);
		if (is_trapped(env))
			disconnect_waiting_player(env);
		move(env);
		if (has_win(env))
		{
			ft_putendl("You win !");
			disconnect_waiting_player(env);
		}
		else
			sem_post(env->sem_board);
		sleep(1);
	}
}
