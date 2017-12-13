/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ia.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/13 14:32:58 by amathias          #+#    #+#             */
/*   Updated: 2017/12/13 14:48:49 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

float	get_distance(t_pos pos1, t_pos pos2)
{
	return (sqrtf((pos1.x - pos2.x) * (pos1.y - pos2.y)));
}

void	update_target(t_env *env)
{
	t_pos 	tmp;
	float	min_distance;
	float	tmp_distance;
	int i;
	int j;

	i = 0;
	min_distance = BOARD_SIZE * BOARD_SIZE;
	while (i < BOARD_SIZE)
	{
		j = 0;
		while (j < BOARD_SIZE)
		{
			if (env->shared->board[i][j] == 0
					&& env->shared->board[i][j] != env->team_id)
			{
				tmp.x = j;
				tmp.y = i;
				tmp_distance = get_distance(env->pos, tmp);
				if (tmp_distance < min_distance)
				{
					env->target = tmp;
					min_distance = tmp_distance;
				}
			}
			j++;
		}
		i++;
	}
}
