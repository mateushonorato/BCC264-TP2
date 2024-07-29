#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1

void mostrarSaldo(int *pipeSaldo)
{
    int saldo;
    read(pipeSaldo[READ_END], &saldo, sizeof(int));
    printf("----------------------------------------------------------\n");
    printf("Executando no processo com PID %i\n", getpid());
    printf("Saldo: %i$\n", saldo);
    write(pipeSaldo[WRITE_END], &saldo, sizeof(int));
}

void adicionarSaldo(int *pipeSaldo)
{
    printf("----------------------------------------------------------\n");
    printf("Adicionando 500$ ao saldo no processo com PID %i\n", getpid());
    int saldo;
    read(pipeSaldo[READ_END], &saldo, sizeof(int));
    printf("Saldo Anterior: %i$\n", saldo);
    saldo += 500;
    printf("Saldo Atual: %i$\n", saldo);
    write(pipeSaldo[WRITE_END], &saldo, sizeof(int));
}

void subtrairSaldo(int *pipeSaldo)
{
    printf("----------------------------------------------------------\n");
    printf("Subtraindo 500$ do saldo no processo com PID %i\n", getpid());
    int saldo;
    read(pipeSaldo[READ_END], &saldo, sizeof(int));
    printf("Saldo Anterior: %i$\n", saldo);
    saldo -= 500;
    printf("Saldo Atual: %i$\n", saldo);
    write(pipeSaldo[WRITE_END], &saldo, sizeof(int));
}

char menuPrincipal()
{
    printf("----------------------------------------------------------\n");
    printf("Executando no processo com PID %i\n", getpid());
    printf("Insira a opção desejada:\n");
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
    int pipeSaldo[2], pipeOpcao[2], saldo = 0;
    pid_t pidFilho1 = -1, pidFilho2 = -1, pidFilho3 = -1;
    char opcao;

    // Iniciando o Pipe
    if (pipe(pipeSaldo) == -1 || pipe(pipeOpcao) == -1)
    {
        fprintf(stderr, "O Pipe falhou!");
        return 1;
    }
    if (write(pipeSaldo[WRITE_END], &saldo, sizeof(int)) == -1)
    {
        fprintf(stderr, "Erro ao escrever no pipe\n");
        return 1;
    }

    // Iniciando os processos filho
    // Todos os processos filhos ficam aguardando receber a opção inserida pelo usuário através do pipeOpcao.
    // O processo verifica se a opção corresponde a ele. Caso sim, executa a operação. Caso não, repassa a opção para os demais processos.
    // O processo filho 1 será responsável por adicionar o saldo
    pidFilho1 = fork();
    if (pidFilho1 == 0)
    {
        while (read(pipeOpcao[READ_END], &opcao, sizeof(char)) == 1)
        {
            if (opcao == '+')
            {
                adicionarSaldo(pipeSaldo);
            }
            else
            {
                write(pipeOpcao[WRITE_END], &opcao, sizeof(char));
            }
        }
        return 0;
    }
    // O processo filho 1 será responsável por subtrair do saldo
    pidFilho2 = fork();
    if (pidFilho2 == 0)
    {
        while (read(pipeOpcao[READ_END], &opcao, sizeof(char)) == 1)
        {
            if (opcao == '-')
            {
                subtrairSaldo(pipeSaldo);
            }
            else
            {
                write(pipeOpcao[WRITE_END], &opcao, sizeof(char));
            }
        }
        return 0;
    }
    // O processo filho 1 será responsável por mostrar o saldo na tela (print)
    pidFilho3 = fork();
    if (pidFilho3 == 0)
    {
        while (read(pipeOpcao[READ_END], &opcao, sizeof(char)) == 1)
        {
            if (opcao == 'm')
            {
                mostrarSaldo(pipeSaldo);
            }
            else
            {
                write(pipeOpcao[WRITE_END], &opcao, sizeof(char));
            }
        }
        return 0;
    }

    // Verificando se todos os processos filhos iniciaram corretamente
    if (pidFilho1 < 0 || pidFilho2 < 0 || pidFilho3 < 0)
    {
        fprintf(stderr, "O Fork falhou!");
        return 1;
    }

    // Executando o menu de opções (apenas no processo pai)
    do
    {
        opcao = menuPrincipal();
        write(pipeOpcao[WRITE_END], &opcao, sizeof(char));
        sleep(1);
    } while (opcao != 's' && pidFilho1 > 0 && pidFilho2 > 0 && pidFilho3 > 0);

    // Quando o usuário insere a opção 's', o loop é finalizado, e podemos assim encerrar os processos filhos e terminar a execução
    close(pipeOpcao[WRITE_END]);
    close(pipeSaldo[READ_END]);
    close(pipeSaldo[WRITE_END]);
    kill(pidFilho1, SIGKILL);
    kill(pidFilho2, SIGKILL);
    kill(pidFilho3, SIGKILL);
    return 0;
}
