/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/13 11:40:06 by amathias          #+#    #+#             */
/*   Updated: 2017/12/14 17:46:20 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	init_msqs(t_env *env)
{
	key_t	key;
	int		msqid;

	if (access("/tmp/lempipc", F_OK) != 0)
		creat("/tmp/lemipc", 0666);
	key = ftok("/tmp/lemipc", 'G');
	msqid = msgget(key, 0666 | IPC_CREAT);
	env->msq_target = msqid;
}

void	delete_msqs(t_env *env)
{
	if (msgctl(env->msq_target, IPC_RMID, NULL))
	{
		perr_exit("msgctl");
	}
}

int		receive_target(t_env *env)
{
	t_msg_target	msg_target;

	if (msgrcv(env->msq_target, &msg_target, sizeof(t_msg_target),
			env->team_id, IPC_NOWAIT) != -1)
	{
		env->target = msg_target.target;
		return (1);
	}
	return (0);
}

void	broadcast_target(t_env *env, t_pos target)
{
	t_msg_target		msg_target;
	int					i;
	int					j;

	msg_target.mtype = env->team_id;
	msg_target.target = target;
	i = 0;
	while (i < BOARD_SIZE)
	{
		j = 0;
		while (j < BOARD_SIZE)
		{
			if (env->shared->board[i][j] == env->team_id
				&& i != env->pos.y && j != env->pos.x)
			{
				msgsnd(env->msq_target, &msg_target, sizeof(t_msg_target), 0);
			}
			j++;
		}
		i++;
	}
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
