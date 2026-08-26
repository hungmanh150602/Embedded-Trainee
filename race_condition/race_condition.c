#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        /* CHILD */
        printf("Child: preparing important data...\n");

        FILE *fp = fopen("important.txt", "w");
        fprintf(fp, "IMPORTANT DATA\n");

        printf("Child: now exec another program...\n");

        /* call exec() */
        execl("/home/hungubuntu/Vim_C_code/test", "test", NULL);

        perror("exec");
        exit(127);
    }
    else
    {
        wait(NULL);
        printf("I am parent.  Child already done!\n");
    }
}