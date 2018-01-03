/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 12:53:22 by amathias          #+#    #+#             */
/*   Updated: 2018/01/03 12:08:41 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	remove_wrong_path(t_lpos **main_list, t_lpos **adja_list)
{
	t_lpos	*backup_next;
	t_lpos	*tmp_adja;
	t_lpos	*tmp_main;

	tmp_adja = *adja_list;
	while (tmp_adja)
	{
		backup_next = tmp_adja->next;
		tmp_main = *main_list;
		while (tmp_main)
		{
			if (pos_equal(tmp_main->pos, tmp_adja->pos)
				&& tmp_main->counter <= tmp_adja->counter)
			{
				remove_pos(adja_list, tmp_adja->pos);
				break ;
			}
			tmp_main = tmp_main->next;
		}
		tmp_adja = backup_next;
	}
}

void	eval_adjalist(t_env *env, t_lpos **main_list, t_lpos **adja_list)
{
	t_lpos	*backup_next;
	t_lpos	*tmp_adja;

	tmp_adja = *adja_list;
	(void)env;
	while (tmp_adja)
	{
		backup_next = tmp_adja->next;
		if (inbound(tmp_adja->pos.x, tmp_adja->pos.y) == 0
				|| pos_equal(tmp_adja->pos, env->target))
			remove_pos(adja_list, tmp_adja->pos);
		tmp_adja = backup_next;
	}
	remove_wrong_path(main_list, adja_list);
	push_back_remaining(main_list, adja_list);
}

t_pos	get_next_pos(t_lpos *main_list, t_lpos *final)
{
	while (main_list)
	{
		if (main_list->counter == final->counter - 1)
		{
			if (are_adjacent(main_list->pos, final->pos) == 1)
				return (main_list->pos);
		}
		main_list = main_list->next;
	}
	return (get_pos(0, 0));
}

t_pos	find_path(t_env *env, t_pos start, t_pos target)
{
	t_lpos	*main_list;
	t_lpos	*adja_list;
	t_lpos	*tmp;
	t_lpos	*previous;

	main_list = new_pos(target, 0);
	tmp = main_list;
	while (tmp)
	{
		if (pos_equal(tmp->pos, start))
		{
			target = get_next_pos(main_list, tmp);
			break ;
		}
		adja_list = get_adjacent_pos_list(tmp->pos, tmp->counter);
		eval_adjalist(env, &main_list, &adja_list);
		clear_poslist(&adja_list);
		previous = tmp;
		tmp = tmp->next;
	}
	clear_poslist(&main_list);
	return (target);
}
