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
# include "lemipc.h"

void	reset_semaphores()
{
	int		need_init;
	sem_t 	*sem;

	need_init = 0;
	if ((sem = sem_open(SEM_BOARD, O_CREAT, 0666, 1)) != SEM_FAILED)
		need_init = 1;
	else if ((sem = sem_open(SEM_BOARD, 0)) == SEM_FAILED)
		need_init = 0;
	int val;
	sem_getvalue(sem, &val);
	printf("sem getvalue: %d\n", val);
}

void	reset_shared_memory()
{
	int			shm_fd;
	t_shared	*shared;
	int			need_init;

	need_init = 0;
	if ((shm_fd = shm_open(SHARED_BOARD, O_RDWR | O_CREAT | O_EXCL, 0666))
			!= -1)
		need_init = 1;
	else if ((shm_fd = shm_open(SHARED_BOARD, O_RDWR | O_CREAT)) == -1)
		;
	ftruncate(shm_fd, sizeof(t_shared));
	if ((shared = mmap(NULL, sizeof(t_shared), PROT_READ | PROT_WRITE,
			MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
		;
	memset(shared, 0, sizeof(t_shared));
	if (munmap(shared, sizeof(t_shared)) == -1)
		;
	sem_unlink(SEM_BOARD);
	shm_unlink(SHARED_BOARD);
}

int		get_msq_id(char c)
{
	key_t key;
	int msqid;

	if (access("/tmp/lempipc", F_OK) != 0)
		creat("/tmp/lemipc", 0666);
	key = ftok("/tmp/lemipc", c);
	msqid = msgget(key, 0666 | IPC_CREAT);
	return (msqid);
}

void	reset_msqs()
{
	msgctl(get_msq_id('G'), IPC_RMID, NULL);
	msgctl(get_msq_id('R'), IPC_RMID, NULL);
}

int main(int argc, char *argv[])
{
	reset_semaphores();
	reset_shared_memory();
	reset_msqs();
	return 0;
}
