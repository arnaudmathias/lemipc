/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/13 11:40:06 by amathias          #+#    #+#             */
/*   Updated: 2017/12/13 17:42:20 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

int		get_msq_id(char c)
{
	key_t key;
	int msqid;

	if (access("/tmp/lempipc", F_OK) != 0)
		creat("/tmp/lemipc", 0666);
	key = ftok("/tmp/lemipc", c);
	msqid = msgget(key, 0666 | IPC_CREAT);
	return (msqid);
}

void	init_msqs(t_env *env)
{
	env->msq_gamestatus = get_msq_id('G');
}

void	delete_msqs(t_env *env)
{
	msgctl(env->msq_gamestatus, IPC_RMID, NULL);
}

void	receive_loop(t_env *env)
{
	char buf[2];

	printf("Press enter when you're ready\n");
	if (read(0, buf, 2) > 0)
	{
		sem_wait(env->sem_board);
		env->shared->player_ready++;
		env->is_ready = 1;
		sem_post(env->sem_board);
		while (42)
		{
			sem_wait(env->sem_board);
			if (env->shared->player_ready == env->shared->player_counter)
			{
				sem_post(env->sem_board);
				game_loop(env);
			}
			else
				sem_post(env->sem_board);
		}
	}
}
