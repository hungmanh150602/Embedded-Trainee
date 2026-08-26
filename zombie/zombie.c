#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child PID = %d\n", getpid());

        printf("Child exits now\n");

        _exit(0);
    }
    else
    {
        printf("Parent PID = %d\n", getpid());
        printf("Child PID = %d\n", pid);

        printf("Parent sleeps for 30 seconds...\n");

        sleep(30);

        printf("Parent calls waitpid()\n");

        int status;
        waitpid(pid, &status, 0);

        printf("Child reaped\n");
    }

    return 0;
}