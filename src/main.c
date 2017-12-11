/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/09 18:32:27 by amathias          #+#    #+#             */
/*   Updated: 2017/12/11 19:01:15 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	perr_exit(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	init_semaphores(t_env *env)
{
	int		need_init;

	need_init = 0;
	if ((env->sem_board = sem_open(SEM_BOARD, O_CREAT, 0777, 0)) != SEM_FAILED)
		need_init = 1;
	else if ((env->sem_board = sem_open(SEM_BOARD, 0)) == SEM_FAILED)
		perr_exit("sem_open");
}

void	init_shared_memory(t_env *env)
{
	int			shm_fd;
	t_shared	*shared;
	int			need_init;

	init_semaphores(env);
	need_init = 0;
	if ((shm_fd = shm_open(SHARED_BOARD, O_RDWR | O_CREAT, 777)) != -1)
		need_init = 1;
	else if ((shm_fd = shm_open(SHARED_BOARD, 0)) == -1)
		perr_exit("shm_open");
	if (need_init)
	{
		if (need_init && ftruncate(shm_fd, sizeof(t_shared)) == -1)
			perr_exit("ftruncate");
		if ((shared = mmap(NULL, sizeof(t_shared), PROT_READ | PROT_WRITE,
				MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
			perr_exit("mmap");
		ft_memset(shared, 0, sizeof(t_shared));
	}
}

void	delete_shared_memory(t_env *env)
{
	if (munmap(env->board, sizeof(t_shared)) == -1)
		perr_exit("munmap");
	sem_unlink(SEM_BOARD);
	shm_unlink(SHARED_BOARD);
	printf("delete shared memory\n");
}

int		main(void)
{
	t_env	env;

	ft_memset(&env, 0, sizeof(t_env));
	init_shared_memory(&env);
	if (sem_wait(env.sem_board) == 0) {
		env.board->player_counter--;
		if (env.board->player_counter == 0) {
			sem_post(env.sem_board);
			delete_shared_memory(&env);
		} else {
			sem_post(env.sem_board);
		}
	}
	return (EXIT_SUCCESS);
}
