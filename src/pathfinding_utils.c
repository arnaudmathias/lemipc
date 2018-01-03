/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinding_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 16:22:49 by amathias          #+#    #+#             */
/*   Updated: 2018/01/03 12:37:58 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

int		are_adjacent(t_pos pos1, t_pos pos2)
{
	if (pos_equal(get_pos(pos1.x + 1, pos1.y), get_pos(pos2.x, pos2.y)))
		return (1);
	else if (pos_equal(get_pos(pos1.x - 1, pos1.y), get_pos(pos2.x, pos2.y)))
		return (1);
	else if (pos_equal(get_pos(pos1.x, pos1.y + 1), get_pos(pos2.x, pos2.y)))
		return (1);
	else if (pos_equal(get_pos(pos1.x, pos1.y - 1), get_pos(pos2.x, pos2.y)))
		return (1);
	else
		return (0);
}

void	print_lpos(t_lpos *lpos)
{
	while (lpos)
	{
		printf("%d|%d, %d\n", lpos->pos.x, lpos->pos.y, lpos->counter);
		lpos = lpos->next;
	}
	printf("\n");
}

void	push_back_remaining(t_lpos **main_list, t_lpos **adja_list)
{
	t_lpos *tmp;

	tmp = *adja_list;
	while (tmp)
	{
		push_back_pos(main_list, new_pos(tmp->pos, tmp->counter));
		tmp = tmp->next;
	}
	clear_poslist(adja_list);
}

t_lpos	*get_adjacent_pos_list(t_env *env, t_pos pos, int counter)
{
	t_lpos	*adjacent_list;
	t_lpos	*backup_next;
	t_lpos	*tmp;

	adjacent_list = new_pos(get_pos(pos.x + 1, pos.y), counter + 1);
	push_back_pos(&adjacent_list,
			new_pos(get_pos(pos.x - 1, pos.y), counter + 1));
	push_back_pos(&adjacent_list,
			new_pos(get_pos(pos.x, pos.y + 1), counter + 1));
	push_back_pos(&adjacent_list,
			new_pos(get_pos(pos.x, pos.y - 1), counter + 1));
	tmp = adjacent_list;
	while (tmp)
	{
		backup_next = tmp->next;
		if (!is_pos_free(env, tmp->pos.x, tmp->pos.y)
			&& !pos_equal(tmp->pos, env->target)
			&& !pos_equal(tmp->pos, env->pos))
			remove_pos(&adjacent_list, tmp->pos);
		tmp = backup_next;
	}
	return (adjacent_list);
}
