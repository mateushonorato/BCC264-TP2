#include <pthread.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/wait.h>

#define NUMTHREADS 3
pthread_t threads[NUMTHREADS];
int saldo = 0;

void *mostrarSaldo(void *arg)
{
    printf("----------------------------------------------------------\n");
    printf("Executando na thread com TID %i\n", gettid());
    printf("Saldo: %i$\n", saldo);
    pthread_exit(0);
}

void *adicionarSaldo(void *arg)
{
    printf("----------------------------------------------------------\n");
    printf("Adicionando 500$ ao saldo na thread com TID %i\n", gettid());
    printf("Saldo Anterior: %i$\n", saldo);
    saldo += 500;
    printf("Saldo Atual: %i$\n", saldo);
    pthread_exit(0);
}

void *subtrairSaldo(void *arg)
{
    printf("----------------------------------------------------------\n");
    printf("Subtraindo 500$ do saldo na thread com TID %i\n", gettid());
    printf("Saldo Anterior: %i$\n", saldo);
    saldo -= 500;
    printf("Saldo Atual: %i$\n", saldo);
    pthread_exit(0);
}

char menuPrincipal()
{
    printf("----------------------------------------------------------\n");
    printf("Executando no processo com PID %i\n", getpid());
    printf("Insira + opção desejada:\n");
    printf("+ -> Adicionar 500$\n");
    printf("- -> Subtrair 500$\n");
    printf("m -> Mostrar o saldo\n");
    printf("s -> Sair\n");
    printf("> ");
    char opcao = getchar();
    getchar();
    while (opcao != '+' && opcao != '-' && opcao != 'm' && opcao != 's')
    {
        printf("> ");
        opcao = getchar();
        getchar();
    }
    return opcao;
}

int main()
{
    char opcao;

    // Executando o menu de opções
    do
    {
        opcao = menuPrincipal();

        if (opcao == '+')
        {
            pthread_create(&(threads[0]), NULL, adicionarSaldo, NULL);
            pthread_join(threads[0], NULL);
        }
        else if (opcao == '-')
        {
            pthread_create(&(threads[1]), NULL, subtrairSaldo, NULL);
            pthread_join(threads[1], NULL);
        }
        else if (opcao == 'm')
        {
            pthread_create(&(threads[2]), NULL, mostrarSaldo, NULL);
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
            pthread_join(threads[2], NULL);
        }
    } while (opcao != 's');

    // Quando o usuário insere a opção 's', o loop é finalizado, e podemos assim encerrar as threads e terminar a execução
    pthread_kill(threads[0], 0);
    pthread_kill(threads[1], 0);
    pthread_kill(threads[2], 0);
    return 0;
}
