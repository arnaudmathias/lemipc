/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_graph.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/03 13:46:59 by amathias          #+#    #+#             */
/*   Updated: 2018/01/03 14:12:06 by amathias         ###   ########.fr       */
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
				printf("%c", env->shared->board[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

void	connect_player(t_env *env)
{
	ft_memset(env, 0, sizeof(t_env));
	init_shared_memory_graph(env);
}


void	sig_handler(int signum)
{
	(void)signum;
	if (munmap(g_env.shared, sizeof(t_shared)) == -1)
		perr_exit("munmap");
	exit(0);
}

void	game_loop(t_env *env)
{
	while (42)
	{
		sem_wait(env->sem_board);
		system("clear");
		game_display(env);
		sem_post(env->sem_board);
		sleep(1);
	}
}

int		main()
{
	struct sigaction	sigact;

	srand(time(NULL));
	connect_player(&g_env);
	sigact.sa_handler = sig_handler;
	sigaction(SIGINT, &sigact, NULL);
	game_loop(&g_env);
	return (EXIT_SUCCESS);
}
