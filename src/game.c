/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 11:31:18 by amathias          #+#    #+#             */
/*   Updated: 2017/12/13 17:15:13 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	game_display(t_env *env)
{
	int i;
	int j;

	i = 0;
	printf("\n");
	while (i < BOARD_SIZE)
	{
		j = 0;
		while (j < BOARD_SIZE)
		{
			if (env->shared->board[i][j] == 0)
				printf("%c", 'o');
			else
				printf("%d", env->shared->board[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

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
	printf("countd: %d\n", count);
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

int 	is_inbound(int x, int y)
{
	return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE);
}

int		is_pos_free(t_env *env, int x, int y)
{
	return (env->shared->board[y][x] == 0);
}

void	place_player(t_env *env)
{
	t_pos	pos;
	int		try;

	try = 0;
	while (try < BOARD_SIZE * BOARD_SIZE)
	{
		pos.x = rand() % (BOARD_SIZE);
		pos.y = rand() % (BOARD_SIZE);
		if (is_pos_free(env, pos.x, pos.y))
		{
			env->pos = pos;
			env->shared->board[pos.y][pos.x] = env->team_id;
			return ;
		}
		try++;
	}
	assert(0); //TODO: find first free pos and place user there
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
			if (env->shared->board[i][j] != env->team_id)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

void	move(t_env *env)
{
	t_pos	oldpos;

	oldpos = env->pos;
	if (env->target.x < env->pos.x && is_inbound(env->pos.x - 1, env->pos.y)
			&& is_pos_free(env, env->pos.x - 1, env->pos.y))
		env->pos.x--;
	else if (env->target.x > env->pos.x && is_inbound(env->pos.x + 1, env->pos.y)
			&& is_pos_free(env, env->pos.x + 1, env->pos.y))
		env->pos.x++;
	else if (env->target.y < env->pos.y && is_inbound(env->pos.x, env->pos.y - 1)
			&& is_pos_free(env, env->pos.x, env->pos.y - 1))
		env->pos.y--;
	else if (env->target.y > env->pos.y && is_inbound(env->pos.x, env->pos.y + 1)
			&& is_pos_free(env, env->pos.x, env->pos.y + 1))
		env->pos.y++;
	else
		return ;
	env->shared->board[oldpos.y][oldpos.x] = 0;
	env->shared->board[env->pos.y][env->pos.x] = env->team_id;
}

void	game_loop(t_env *env)
{
	while (42)
	{
		sem_wait(env->sem_board);
		system("clear");
		game_display(env);
		update_target(env);
		if (is_trapped(env))
		{
			sem_post(env->sem_board);
			sig_handler(0);
		}
		move(env);
		sem_post(env->sem_board);
		sleep(1);
	}
}
