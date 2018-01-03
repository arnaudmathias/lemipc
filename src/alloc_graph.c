/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_graph.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/03 14:00:21 by amathias          #+#    #+#             */
/*   Updated: 2018/01/03 14:23:06 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	init_semaphores_graph(t_env *env)
{
	while ((env->sem_board = sem_open(SEM_BOARD, 0)) == SEM_FAILED)
	{
		system("clear");
		printf("Semaphore unavailable\n");
		sleep(1);
	}
}

void	init_shared_memory_graph(t_env *env)
{
	int			shm_fd;
	t_shared	*shared;

	while ((shm_fd = shm_open(SHARED_BOARD, O_RDWR)) == -1)
	{
		system("clear");
		printf("Shared memory unavailable\n");
		sleep(1);
	}
	printf("shm_fd: %d\n", shm_fd);
	ftruncate(shm_fd, sizeof(t_shared));
	if ((shared = mmap(NULL, sizeof(t_shared), PROT_READ | PROT_WRITE,
			MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
		perr_exit("mmap");
	env->shared = shared;
	init_semaphores_graph(env);
}

void	delete_shared_memory(t_env *env)
{
	if (munmap(env->shared, sizeof(t_shared)) == -1)
		perr_exit("munmap");
	sem_unlink(SEM_BOARD);
	shm_unlink(SHARED_BOARD);
	printf("delete shared memory\n");
}
