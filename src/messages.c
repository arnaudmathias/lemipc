/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/13 11:40:06 by amathias          #+#    #+#             */
/*   Updated: 2017/12/13 19:23:11 by amathias         ###   ########.fr       */
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
	env->msq_target = get_msq_id('G');
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

void	broadcast_target(t_env *env)
{
	t_msg_target		msg_target;
	int 				i;
	int					j;

	msg_target.mtype = env->team_id;
	msg_target.target = env->target;
	i = 0;
	while (i < BOARD_SIZE)
	{
		j = 0;
		while (j < BOARD_SIZE)
		{
			if (env->shared->board[i][j] == env->team_id)
				msgsnd(env->msq_target, &msg_target, sizeof(t_msg_target), 0);
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
