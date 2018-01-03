/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_graph.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/03 13:46:59 by amathias          #+#    #+#             */
/*   Updated: 2018/01/03 16:09:15 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	sig_handler(int signum)
{
	(void)signum;
	if (munmap(g_env.shared, sizeof(t_shared)) == -1)
		perr_exit("munmap");
	exit(0);
}

void	wait_start(t_env *env)
{
	char 			buf[256];
	struct pollfd	fd[1];
	int				ret;

	fd[0].fd = 0;
	fd[0].events = POLLIN;
	while (1)
	{
		sem_wait(env->sem_board);
		system("clear");
		game_display(env);
		sem_post(env->sem_board);
		printf("Press any key to start the game\n");
		ret = poll(fd, 1, 100);
		if (ret != 0)
			break ;
	}
	read(0, buf, 255);
	sem_wait(env->sem_board);
	broadcast_ready(env);
	sem_post(env->sem_board);
}

void	game_loop(t_env *env)
{
	wait_start(env);
	while (42)
	{
		sem_wait(env->sem_board);
		system("clear");
		game_display(env);
		if (is_empty(env))
		{
			sem_post(env->sem_board);
			reset_graph(env);
		}
		sem_post(env->sem_board);
		sleep(1);
	}
}

int		main(void)
{
	struct sigaction	sigact;

	srand(time(NULL));
	ft_memset(&g_env, 0, sizeof(t_env));
	init_shared_memory_graph(&g_env);
	init_msqs_graph(&g_env);
	sigact.sa_handler = sig_handler;
	sigaction(SIGINT, &sigact, NULL);
	game_loop(&g_env);
	return (EXIT_SUCCESS);
}

void	reset_graph(t_env *env)
{
	if (munmap(env->shared, sizeof(t_shared)) == -1)
		perr_exit("munmap");
	main();
}
