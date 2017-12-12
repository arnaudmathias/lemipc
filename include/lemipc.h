/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/09 18:31:40 by amathias          #+#    #+#             */
/*   Updated: 2017/12/12 11:30:31 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIPC_H
# define LEMIPC_H
# define BOARD_SIZE 16
# define SHARED_BOARD "/board6"
# define SEM_BOARD "/board_sem"

# include <sys/stat.h>
# include <sys/msg.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/mman.h>
# include <semaphore.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <assert.h>
# include "libft.h"

typedef struct	s_shared
{
	int			player_counter;
	int			board[BOARD_SIZE][BOARD_SIZE];
}				t_shared;

typedef	struct	s_env
{
	t_shared	*board;
	sem_t		*sem_board;
}				t_env;

void	init_shared_memory(t_env *env);
void	delete_shared_memory(t_env *env);

void	perr_exit(char *msg);
#endif
