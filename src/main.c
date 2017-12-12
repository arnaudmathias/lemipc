/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/09 18:32:27 by amathias          #+#    #+#             */
/*   Updated: 2017/12/12 11:56:37 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	connect_player(t_env *env)
{
	ft_memset(env, 0, sizeof(t_env));
	init_shared_memory(env);
	env->shared->player_counter++;
}

void	disconnect_player(t_env *env)
{
	if (sem_wait(env->sem_board) == 0) {
		printf("sem_trywait\n");
		env->shared->player_counter--;
		printf("%d\n", env->shared->player_counter);
		if (env->shared->player_counter == 0) {
			sem_post(env->sem_board);
			delete_shared_memory(env);
		} else {
			sem_post(env->sem_board);
		}
	} else {
		perr_exit("sem_trywait");
	}
	if (munmap(env->shared, sizeof(t_shared)) == -1)
		perr_exit("munmap");
}

void	sig_handler(int signum)
{
	(void)signum;
	disconnect_player(&g_env);
	exit(0);
}

int		main(void)
{
	struct sigaction	sigact;

	connect_player(&g_env);
	sigact.sa_handler = sig_handler;
	sigaction(SIGINT, &sigact, NULL);
	game_display(&g_env);
	getc(stdin);
	disconnect_player(&g_env);
	return (EXIT_SUCCESS);
}
