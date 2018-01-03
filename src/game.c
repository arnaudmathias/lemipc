/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 11:31:18 by amathias          #+#    #+#             */
/*   Updated: 2018/01/03 09:43:18 by amathias         ###   ########.fr       */
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
			else if (i == env->pos.y && j == env->pos.x)
				printf("%s%c%s", ANSI_COLOR_RED, env->shared->board[i][j],
						ANSI_COLOR_RESET);
			else
				printf("%c", env->shared->board[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
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
		system("clear");
		game_display(env);
		update_target(env);
		if (is_trapped(env))
		{
			sem_post(env->sem_board);
			sig_handler(0);
		}
		move(env);
		if (has_win(env))
		{
			printf("You win !\n");
			sem_post(env->sem_board);
			sig_handler(0);
		}
		else
			sem_post(env->sem_board);
		sleep(1);
	}
}
