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

void handle(int sig)
{
    /* detach */
    if (shmdt(ptr) == -1)
    {
        perror("shmdt");
    }

    exit(123);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handle);

    /* generate the key */
    // key_t key = ftok("/tmp/myshare", 'A');

    /* create the shared memory segment */
    shm_id = shmget(SHARED_MEMORY_KEY, SEGMENT_SIZE, 0666);

    if (shm_id == -1) /* check error */
    {
        perror("shmget");
        return -1;
    }
    printf("Shared memory id: %d\n", shm_id);

    /* attach */
    ptr = shmat(shm_id, NULL, 0);
    if ((void *)ptr == (void *)-1) /* check error */
    {
        perror("shmat");
        return -1;
    }
    printf("Shared memory address: %p\n", ptr);

    /* get semaphore */
    sem_id = semget(SEMAPHORE_KEY, 3, 0666);

    if (sem_id == -1) /* check error */
    {
        perror("semget");
        return -1;
    }
    printf("Semaphore id: %d\n", sem_id);

    /* create semaphore operation */
    sem_opr[0].sem_flg = 0;
    sem_opr[0].sem_num = 0;
    sem_opr[0].sem_op = -1;

    sem_opr[1].sem_flg = 0;
    sem_opr[1].sem_num = 1;
    sem_opr[1].sem_op = 1;

    /* loop */
    while (1)
    {
        semop(sem_id, &sem_opr[0], 1);
        printf("client 1 received: %s\n", ptr->text);
        ptr->money -= 50;
        sprintf(ptr->text, "client consume 50 money. Now left: %d", ptr->money);
        // printf("client consume 50 money. Now left: %d\n", ptr->money);
        semop(sem_id, &sem_opr[1], 1);
        sleep(1);
    }

    return 0;
}