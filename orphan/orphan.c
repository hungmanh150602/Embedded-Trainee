#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        sleep(10);

        printf("Child PID  = %d\n", getpid());
        printf("Child PPID = %d\n", getppid());

        return 0;
    }
    else
    {
        printf("Parent PID  = %d\n", getpid());
        printf("Child PID = %d\n", pid);
    }

    printf("Parent exiting...\n");

    exit(0);
}