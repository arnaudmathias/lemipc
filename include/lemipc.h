/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/09 18:31:40 by amathias          #+#    #+#             */
/*   Updated: 2017/12/13 17:28:03 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIPC_H
# define LEMIPC_H
# define BOARD_SIZE 8
# define SHARED_BOARD "/board"
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
# include <signal.h>
# include <math.h>
# include <time.h>
# include "libft.h"

typedef struct	s_team
{
	char		*name;
}				t_team;

typedef struct	s_shared
{
	int			player_counter;
	int 		player_ready;
	int			board[BOARD_SIZE][BOARD_SIZE];
}				t_shared;

typedef struct	s_pos
{
	int			x;
	int			y;
}				t_pos;

typedef	struct	s_env
{
	sem_t		*sem_board;
	t_shared	*shared;
	int 		msq_gamestatus;
	int			team_id;
	t_pos		pos;
	t_pos 		target;
	int			is_ready;
}				t_env;

typedef	struct 	s_msg_gamestatus
{
	long 		mtype;
	int 		is_started;
}				t_msg_gamestatus;

t_env			g_env;

void			disconnect_player(t_env *env);
void			init_shared_memory(t_env *env);
void			delete_shared_memory(t_env *env);

void			game_display(t_env *env);
void			game_loop(t_env *env);

void			place_player(t_env *env);

void			init_msqs(t_env *env);
void			delete_msqs(t_env *env);

void			receive_loop(t_env *env);

void			update_target(t_env *env);

void			perr_exit(char *msg);
void			err_exit(char *msg);
void			sig_handler(int signum);
#endif
