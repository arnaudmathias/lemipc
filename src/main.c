/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/09 18:32:27 by amathias          #+#    #+#             */
/*   Updated: 2017/12/13 14:14:28 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	connect_player(t_env *env, int team_num)
{
	ft_memset(env, 0, sizeof(t_env));
	init_msqs(env);
	init_shared_memory(env);
	if (team_num > 0 && team_num < (BOARD_SIZE * BOARD_SIZE) / 2)
	{
		env->team_id = team_num;
		place_player(env);
		// Place user on the map
	}
	else
		err_exit("Invalid team id");
	env->shared->player_counter++;
}

void	disconnect_player(t_env *env)
{
	if (sem_wait(env->sem_board) == 0)
	{
		printf("sem_trywait\n");
		env->shared->player_counter--;
		printf("%d\n", env->shared->player_counter);
		if (env->shared->player_counter == 0)
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
	connect_player(&g_env, ft_atoi(argv[1]));
	sigact.sa_handler = sig_handler;
	sigaction(SIGINT, &sigact, NULL);
	game_display(&g_env);
	receive_loop(&g_env);
	//getc(stdin);
	disconnect_player(&g_env);
	return (EXIT_SUCCESS);
}
