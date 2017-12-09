/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/09 18:32:27 by amathias          #+#    #+#             */
/*   Updated: 2017/12/09 19:37:02 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	init_shared_memory()
{
	key_t key;
	int shmid;
	char *data;

	if (access("/tmp/ipc_board", F_OK) != 0) {
		creat("/tmp/ipc_board", 0644);
	}
	key = ftok("/tmp/ipc_board", 'R');
	if ((shmid = shmget(key, 1024, 0644 | IPC_CREAT)) < 0) {
		perror("shmget");
		exit(1);
	}
	if ((data = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		exit(1);
	}
	/* shmctl(shmid, IPC_RMID, NULL); // Destroy shared memory */
}

int main()
{
	init_shared_memory();
	return 0;
}
