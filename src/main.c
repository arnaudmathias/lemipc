/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/09 18:32:27 by amathias          #+#    #+#             */
/*   Updated: 2018/01/03 18:27:59 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	connect_player(t_env *env, char *team_num)
{
	if (team_num == NULL || ft_strlen(team_num) != 1
			|| ft_isprint(team_num[0]) == 0 || team_num[0] == '.')
		err_exit("Invalid argument"
			": team_num must be any printable ASCII character (except '.')");
	ft_memset(env, 0, sizeof(t_env));
	init_msqs(env);
	init_shared_memory(env);
	if (team_num[0] > 0)
	{
		env->team_id = team_num[0];
		sem_wait(env->sem_board);
		if (place_player(env) == 0)
			err_exit("Error: cannot place player, map is full");
		sem_post(env->sem_board);
	}
	else
		err_exit("Invalid team id");
	env->shared->player_counter++;
}

void	disconnect_player(t_env *env)
{
	if (sem_wait(env->sem_board) == 0)
	{
		env->shared->player_counter--;
		if (env->is_ready)
			env->shared->player_ready--;
		env->shared->board[env->pos.y][env->pos.x] = 0;
		if (env->shared->player_counter <= 0)
		{
			sem_post(env->sem_board);
			delete_shared_memory(env);
			delete_msqs(env);
		}
		else
			sem_post(env->sem_board);
	}
	else
		perr_exit("sem_trywait");
	if (munmap(env->shared, sizeof(t_shared)) == -1)
		perr_exit("munmap");
	exit(0);
}

void	disconnect_waiting_player(t_env *env)
{
	env->shared->player_counter--;
	if (env->is_ready)
		env->shared->player_ready--;
	env->shared->board[env->pos.y][env->pos.x] = 0;
	if (env->shared->player_counter <= 0)
	{
		sem_post(env->sem_board);
		delete_shared_memory(env);
		delete_msqs(env);
	}
	else
		sem_post(env->sem_board);
	if (munmap(env->shared, sizeof(t_shared)) == -1)
		perr_exit("munmap");
	exit(0);
}

void	sig_handler(int signum)
{
	(void)signum;
	disconnect_player(&g_env);
	exit(0);
}

int		main(int argc, char **argv)
{
	struct sigaction	sigact;

	if (argc != 2)
	{
		ft_putstr("Invalid usage: ./lemipc <team_num>\n");
		exit(EXIT_FAILURE);
	}
	srand(time(NULL));
	connect_player(&g_env, argv[1]);
	sigact.sa_handler = sig_handler;
	sigaction(SIGINT, &sigact, NULL);
	receive_loop(&g_env);
	disconnect_player(&g_env);
	return (EXIT_SUCCESS);
}
