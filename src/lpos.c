/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lpos.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 11:48:51 by amathias          #+#    #+#             */
/*   Updated: 2017/12/14 17:22:43 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	clear_poslist(t_lpos **begin)
{
	t_lpos *current;
	t_lpos *next;

	current = *begin;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*begin = NULL;
}

void	append_pos(t_lpos **lpos, t_lpos *toadd)
{
	toadd->next = *lpos;
	*lpos = toadd;
}

void	push_back_pos(t_lpos **begin, t_lpos *toadd)
{
	t_lpos *tmp;

	tmp = *begin;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = toadd;
}

t_lpos	*new_pos(t_pos pos, int counter)
{
	t_lpos *lpos;

	if ((lpos = malloc(sizeof(t_lpos))) == NULL)
		return (NULL);
	lpos->pos = pos;
	lpos->counter = counter;
	lpos->next = NULL;
	return (lpos);
}

void	remove_pos(t_lpos **lpos, t_pos pos)
{
	t_lpos	*tmp;
	t_lpos	*prev;

	tmp = *lpos;
	if (tmp != NULL && pos_equal(tmp->pos, pos))
	{
		*lpos = tmp->next;
		free(tmp);
		return ;
	}
	while (tmp != NULL && pos_equal(tmp->pos, pos) == 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp == NULL)
		return ;
	prev->next = tmp->next;
	free(tmp);
}
