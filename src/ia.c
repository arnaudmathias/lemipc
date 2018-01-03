/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ia.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/13 14:32:58 by amathias          #+#    #+#             */
/*   Updated: 2018/01/03 15:35:52 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

float	get_total_distance(t_env *env, t_pos target_pos)
{
	float	sum;
	int		i;
	int		j;

	i = 0;
	sum = 0.0f;
	while (i < BOARD_SIZE)
	{
		j = 0;
		while (j < BOARD_SIZE)
		{
			if (env->shared->board[i][j] == env->team_id)
			{
				sum += get_distance(target_pos, get_pos(j, i));
			}
			j++;
		}
		i++;
	}
	return (sum);
}

void	update_distance(t_env *env, t_pos pos, float *min_distance,
			t_pos *target)
{
	float	tmp_distance;

	if (env->shared->board[pos.y][pos.x] != 0
			&& env->shared->board[pos.y][pos.x] != env->team_id)
	{
		tmp_distance = get_total_distance(env, pos);
		if (tmp_distance < *min_distance)
		{
			*target = pos;
			*min_distance = tmp_distance;
		}
	}
}

t_pos	get_new_target(t_env *env)
{
	float	min_distance;
	t_pos	pos;
	t_pos	target;

	pos.y = 0;
	target = get_pos(0, 0);
	min_distance = BOARD_SIZE * BOARD_SIZE;
	while (pos.y < BOARD_SIZE)
	{
		pos.x = 0;
		while (pos.x < BOARD_SIZE)
		{
			update_distance(env, pos, &min_distance, &target);
			pos.x++;
		}
		pos.y++;
	}
	return (target);
}

void	update_target(t_env *env)
{
	t_pos			new_target;
	t_msg_target	msg_target;

	if (msgrcv(env->msq_target, &msg_target, sizeof(t_msg_target),
			env->team_id, IPC_NOWAIT) != -1)
	{
		env->target = msg_target.target;
	}
	else
	{
		new_target = get_new_target(env);
		broadcast_target(env, new_target);
		env->target = new_target;
	}
}
