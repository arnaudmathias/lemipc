/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/13 11:40:06 by amathias          #+#    #+#             */
/*   Updated: 2018/01/04 15:01:20 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	init_msqs(t_env *env)
{
	key_t	key;

	if (access("/tmp/lempipc", F_OK) != 0)
	{
		creat("/tmp/lemipc", 0666);
	}
	key = ftok("/tmp/lemipc", 'G');
	env->msq_target = msgget(key, 0666 | IPC_CREAT);
	key = ftok("/tmp/lemipc", 'R');
	env->msq_ready = msgget(key, 0666 | IPC_CREAT);
}

void	delete_msqs(t_env *env)
{
	if (msgctl(env->msq_target, IPC_RMID, NULL))
		perr_exit("msgctl");
	if (msgctl(env->msq_ready, IPC_RMID, NULL))
		perr_exit("msgctl");
}

void	broadcast_ready(t_env *env)
{
	t_msg_ready		msg_ready;
	int				i;
	int				j;

	msg_ready.mtype = 1;
	msg_ready.ready = 1;
	i = 0;
	while (i < BOARD_SIZE)
	{
		j = 0;
		while (j < BOARD_SIZE)
		{
			if (env->shared->board[i][j] != 0)
				msgsnd(env->msq_ready, &msg_ready,
						sizeof(msg_ready.ready), 0);
			j++;
		}
		i++;
	}
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
				msgsnd(env->msq_target, &msg_target,
						sizeof(msg_target.target), 0);
			j++;
		}
		i++;
	}
}
