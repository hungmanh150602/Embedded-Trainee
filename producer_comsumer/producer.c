#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "type.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

int ret; /* stored the return value of function to check error */
int shm_id;
int sem_id;
struct data *ptr;
struct sembuf sem_opr[2];
unsigned short sem_val[3];

/*
function handle the signal Ctrl + C
clenup and exit
*/
void handle(int sig)
{
    /* detach */
    if (shmdt(ptr) == -1)
    {
        perror("shmdt");
    }

    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);

    exit(123);
}


int main(int argc, char *argv[])
{
    signal(SIGINT, handle); /* signal handle Ctrl + C */

    /* create the shared memory segment */
    shm_id = shmget(SHARED_MEMORY_KEY, SEGMENT_SIZE, 0666 | IPC_CREAT | IPC_EXCL);

    if (shm_id == -1) /* check error */
    {
        perror("shmget");
    }
    else
    {
        printf("Shared memory id: %d\n", shm_id);

        /* attach shared memory */
        ptr = shmat(shm_id, NULL, 0);
        if ((void *)ptr == (void *)-1) /* check error */
        {
            perror("shmat");
            return -1;
        }
        printf("Shared memory address: %p\n", ptr);

        /* init the first value of shared memory */
        ptr->money = 0;
    }

    /* create semaphore */
    sem_id = semget(SEMAPHORE_KEY, 3, IPC_CREAT | 0666);

    if (sem_id == -1) /* already existed semaphore */
    {
        perror("semget");
    }
    else
    {
        printf("Semaphore id: %d\n", sem_id);

        /* semaphore control, set all value of sem is 0 */
        sem_val[0] = 0;
        sem_val[1] = 0;
        sem_val[2] = 0;

        if (semctl(sem_id, 0, SETALL, sem_val) == -1)
        {
            perror("semctl");
            return -1;
        }
    }

    /* create semaphore operation */
    sem_opr[0].sem_flg = 0;
    sem_opr[0].sem_num = 0; /* sem[0] : client */
    sem_opr[0].sem_op = 1;

    sem_opr[1].sem_flg = 0;
    sem_opr[1].sem_num = 1; /* sem[1] : server */
    sem_opr[1].sem_op = -1;

    /* loop */
    while (1)
    {
        ptr->money += 10;

        if (ptr->money >= 100)
        {
            strcpy(ptr->text, "server put in 100 money.");
            semop(sem_id, &sem_opr[0], 1);
            // printf("server add %d money.\n", ptr->money);
            semop(sem_id, &sem_opr[1], 1);
            printf("server received: %s\n", ptr->text);
        }
        sleep(1);
    }

    return 0;
}