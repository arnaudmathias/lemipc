/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 17:21:15 by amathias          #+#    #+#             */
/*   Updated: 2018/01/04 16:29:16 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

t_pos	get_pos(int x, int y)
{
	t_pos pos;

	pos.x = x;
	pos.y = y;
	return (pos);
}

float	get_distance(t_pos pos1, t_pos pos2)
{
	return (sqrtf(fabs((float)(pos1.x - pos2.x)) + fabs((float)(pos1.y - pos2.y))));
}

int		pos_equal(t_pos pos1, t_pos pos2)
{
	return (pos1.x == pos2.x && pos1.y == pos2.y);
}
