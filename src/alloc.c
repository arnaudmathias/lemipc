/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 11:28:49 by amathias          #+#    #+#             */
/*   Updated: 2017/12/12 11:40:01 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	init_semaphores(t_env *env)
{
	int		need_init;

	need_init = 0;
	if ((env->sem_board = sem_open(SEM_BOARD, O_CREAT, 0666, 1)) != SEM_FAILED)
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
	if ((shm_fd = shm_open(SHARED_BOARD, O_RDWR | O_CREAT | O_EXCL, 0666)) != -1)
		need_init = 1;
	else if ((shm_fd = shm_open(SHARED_BOARD, O_RDWR | O_CREAT)) == -1)
		perr_exit("shm_open");
	printf("shm_fd: %d\n", shm_fd);
	ftruncate(shm_fd, sizeof(t_shared));
	if ((shared = mmap(NULL, sizeof(t_shared), PROT_READ | PROT_WRITE,
			MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
		perr_exit("mmap");
	if (need_init)
		ft_memset(shared, 0, sizeof(t_shared));
	env->shared = shared;
}

void	delete_shared_memory(t_env *env)
{
	if (munmap(env->shared, sizeof(t_shared)) == -1)
		perr_exit("munmap");
	sem_unlink(SEM_BOARD);
	shm_unlink(SHARED_BOARD);
	printf("delete shared memory\n");
}
