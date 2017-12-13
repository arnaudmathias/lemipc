/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 11:31:18 by amathias          #+#    #+#             */
/*   Updated: 2017/12/13 11:32:37 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

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
				if (env->shared->board[y][x] != env->team_id &&
					num_of_player_around(env, env->shared->board[y][x] >= 2))
					return (1);
			}
			x++;
		}
		y++;
	}
	return (0);
}

int		is_pos_free(t_env *env, t_pos pos)
{
	return (env->shared->board[pos.y][pos.x] == 0);
}

void	place_player(t_env *env)
{
	t_pos	pos;
	int		try;

	try = 0;
	while (try < 10)
	{
		pos.x = rand() % (BOARD_SIZE);
		pos.y = rand() % (BOARD_SIZE);
		if (is_pos_free(env, pos))
		{
			env->pos = pos;
			env->shared->board[pos.y][pos.x] = env->team_id;
			return ;
		}
		try++;
	}
	//TODO: find first free pos and place user there
}
