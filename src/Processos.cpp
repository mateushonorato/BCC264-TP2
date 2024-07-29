#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int saldo = 0;

void print_saldo(int *pipe_fd)
{
    char buffer[BUFFER_SIZE];
    close(pipe_fd[WRITE_END]);
    while (read(pipe_fd[READ_END], buffer, BUFFER_SIZE) > 0)
    {
        printf("Executando no processo com pid %i\n", getpid());
        printf("Saldo: %s\n", buffer);
    }
    close(pipe_fd[READ_END]);
}

void increment_saldo(int *pipe_fd)
{
    close(pipe_fd[READ_END]);
    saldo += 500;
    char buffer[BUFFER_SIZE];
    printf("Adicionando 500 ao saldo no processo com pid %i\n", getpid());
    snprintf(buffer, sizeof(buffer), "%d", saldo);
    write(pipe_fd[WRITE_END], buffer, strlen(buffer) + 1);
    close(pipe_fd[WRITE_END]);
}

void decrement_saldo(int *pipe_fd)
{
    close(pipe_fd[READ_END]);
    saldo -= 500;
    char buffer[BUFFER_SIZE];
    printf("Subtraindo 500 do saldo no processo com pid %i\n", getpid());
    snprintf(buffer, sizeof(buffer), "%d", saldo);
    write(pipe_fd[WRITE_END], buffer, strlen(buffer) + 1);
    close(pipe_fd[WRITE_END]);
}

int main()
{
    int pipe_fd[2];
    pid_t pid;

    if (pipe(pipe_fd) == -1)
    {
        fprintf(stderr, "O Pipe falhou!");
        return 1;
    }

    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "O Fork falhou!");
        return 1;
    }

    if (pid == 0)
    {
        print_saldo(pipe_fd);
    }
    else
    {
        pid = fork();
        if (pid == 0)
        {
            increment_saldo(pipe_fd);
        }
        else
        {
            pid = fork();
            if (pid == 0)
            {
                decrement_saldo(pipe_fd);
            }
            else
            {
                wait(nullptr);
                wait(nullptr);
                wait(nullptr);
            }
        }
    }
    return 0;
}
