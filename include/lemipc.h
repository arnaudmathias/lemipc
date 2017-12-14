/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/09 18:31:40 by amathias          #+#    #+#             */
/*   Updated: 2017/12/14 16:56:06 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIPC_H
# define LEMIPC_H
# define BOARD_SIZE 8
# define SHARED_BOARD "/board"
# define SEM_BOARD "/board_sem"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_BOLD   "\x1b[1m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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
	int 		msq_target;
	int			team_id;
	t_pos		pos;
	t_pos 		target;
	int			is_ready;
}				t_env;

typedef	struct 	s_msg_target
{
	long 		mtype;
	t_pos		target;
}				t_msg_target;

t_env			g_env;

typedef	struct 	s_lpos
{
	t_pos		pos;
	int			counter;
	void		*next;
}				t_lpos;

void			disconnect_player(t_env *env);
void			init_shared_memory(t_env *env);
void			delete_shared_memory(t_env *env);

void			game_display(t_env *env);
void			game_loop(t_env *env);

void			place_player(t_env *env);
int				is_pos_free(t_env *env, int x, int y);
int 			inbound(int x, int y);
int				is_trapped(t_env *env);
int				num_of_player_around(t_env *env, int team_num);

void			init_msqs(t_env *env);
void			delete_msqs(t_env *env);

void			receive_loop(t_env *env);

int				receive_target(t_env *env);
void			broadcast_target(t_env *env, t_pos pos);
void			update_target(t_env *env);

void			perr_exit(char *msg);
void			err_exit(char *msg);
void			sig_handler(int signum);

t_pos			find_path(t_env *env, t_pos	start, t_pos target);
void	push_back_remaining(t_lpos **main_list, t_lpos **adja_list);
void	print_lpos(t_lpos *lpos);
t_lpos	*get_adjacent_pos_list(t_pos pos, int counter);

t_pos	get_pos(int x, int y);
int		pos_equal(t_pos	pos1, t_pos pos2);
float	get_distance(t_pos pos1, t_pos pos2);
int		are_adjacent(t_pos pos1, t_pos pos2);
void	append_pos(t_lpos **lpos, t_lpos *toadd);
void	push_back_pos(t_lpos **lpos, t_lpos *toadd);
t_lpos	*new_pos(t_pos pos, int counter);
void	remove_pos(t_lpos **lpos, t_pos pos);
void	clear_poslist(t_lpos **begin);
#endif
