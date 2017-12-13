/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/13 11:40:06 by amathias          #+#    #+#             */
/*   Updated: 2017/12/13 15:19:51 by amathias         ###   ########.fr       */
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

void	send_msgs_gamestatus(t_env *env)
{
	t_msg_gamestatus	gs;
	int 				i;

	gs.mtype = 1;
	gs.is_started = 1;
	i = 0;
	sem_wait(env->sem_board);
	while (i < env->shared->player_counter)
	{
		msgsnd(env->msq_gamestatus, &gs, sizeof(t_msg_gamestatus), 0);
		i++;
	}
	sem_post(env->sem_board);
}

void	receive_msg_gamestatus(t_env *env)
{
	t_msg_gamestatus gs;

	if (msgrcv(env->msq_gamestatus, &gs, sizeof(t_msg_gamestatus),
		1, IPC_NOWAIT) != -1)
	{
		printf("game start\n");
		game_loop(env);
	}
}

void	receive_loop(t_env *env)
{
	char buf[1024];

	fcntl(0, F_SETFL, O_NONBLOCK);
	while (42)
	{
		if (read(0, buf, 1024) > 0)
		{
			printf("read something\n");
			send_msgs_gamestatus(env);
		}
		receive_msg_gamestatus(env);
	}
}
