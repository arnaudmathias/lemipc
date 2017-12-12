/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/09 18:32:27 by amathias          #+#    #+#             */
/*   Updated: 2017/12/12 11:30:18 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

int		main(void)
{
	t_env	env;

	ft_memset(&env, 0, sizeof(t_env));
	init_shared_memory(&env);
	env.board->player_counter++;
	getc(stdin);
	if (sem_wait(env.sem_board) == 0) {
		printf("sem_trywait\n");
		env.board->player_counter--;
		printf("%d\n", env.board->player_counter);
		if (env.board->player_counter == 0) {
			sem_post(env.sem_board);
			delete_shared_memory(&env);
		} else {
			sem_post(env.sem_board);
		}
	} else {
		perr_exit("sem_trywait");
	}
	if (munmap(env.board, sizeof(t_shared)) == -1)
		perr_exit("munmap");
	return (EXIT_SUCCESS);
}
