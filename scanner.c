#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int status, fd, count = 0;
    char red[16];
    printf("Introduce prefijo de la red: ");
    scanf("%s", red);
    char *argumentos[5];
    argumentos[0] = "ping";
    argumentos[1] = "-c";
    argumentos[2] = "4";
    argumentos[3] = (char*)malloc(25);
    argumentos[4] = NULL;
    int pids[254];
    fd = open("/dev/null", O_WRONLY);
    for (int i = 0; i < 254; i++)
    {
        pids[i] = fork();
        switch (pids[i])
        {
        case 0:
            dup2(fd, 1);
            sprintf(argumentos[3], "%s.%d", red, i);
            execvp(argumentos[0], argumentos);
            break;
        default:
            break;
        }
    }
    for (int i = 0; i < 254; i++)
    {
        waitpid(pids[i], &status, 0);
        if (WEXITSTATUS(status) == 0){
            printf("El host %s.%d esta activo\n", red, i);
            count++;
        }
    }
    printf("Hay un total de %d dispositivos conectados\n", count);
    free(argumentos[3]);
    return 0;
}
