#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

typedef struct scanner
{
    int pid;
    char *dir;
}scanner;

scanner *punt;
int counter = 0;
int fd; 
char *argumentos[5];

int dirmask(char *dir, char *mask, int** dirr, int** maskr){
    int i;
    char *saveptr, *saveptr2;
    *dirr = (int*)malloc(sizeof(int)*4);
    *maskr = (int*)malloc(sizeof(int)*4);
    char *punt = strtok_r(dir, ".", &saveptr);
    char *punt2 = strtok_r(mask, ".", &saveptr2);
    for (i = 0; punt != NULL && punt2 != NULL && i < 4; i++)
    {
        dirr[0][i] = atoi(punt);
        punt = strtok_r(NULL, ".", &saveptr);

        maskr[0][i] = atoi(punt2);
        punt2 = strtok_r(NULL, ".", &saveptr2);
    }
    return 0;
}

int checkDir(int *direc, int *masc){
    int i, check[4];
    for (i = 0; i < 4; i++)
        check[i] = direc[i] & ~(masc[i]);
    if (check[0] == 0 && check[1] == 0 && check[2] == 0 && check[3] == 0)
        return 0;
    else if((char)check[0] == ~(char)masc[0] && (char)check[1] == ~(char)masc[1] && (char)check[2] == ~(char)masc[2] && (char)check[3] == ~(char)masc[3])    
        return 0;
    else
        return 1;
}

void generateR(int *direc, int *masc, int indice, int cursor){
    if (indice > 3){
        if(checkDir(direc, masc)){
            punt[counter].dir = (char*)malloc(20);
            sprintf(punt[counter].dir, "%d.%d.%d.%d", direc[0], direc[1], direc[2], direc[3]);
            argumentos[3] = punt[counter].dir;
            punt[counter].pid = fork();
            switch (punt[counter].pid)
            {
            case 0:
                dup2(fd, 1);
                execvp(argumentos[0], argumentos);
                break;
            default:
                counter++;
                break;
            }
        }
    }
    else if (cursor > 7)
        generateR(direc, masc, indice+1, 0);
    else if (masc[indice] & 1 << cursor)
        generateR(direc, masc, indice+1, 0);
    else
    {
        direc[indice] = direc[indice] | 1 << cursor;
        generateR(direc, masc, indice, cursor+1);
        direc[indice] = direc[indice] & ~(1 << cursor);
        generateR(direc, masc, indice, cursor+1);
    }
}

void generate(int *direc, int *masc){
    generateR(direc, masc, 0, 0);
}

int possibleAdrr(int *masc){
    int i, j, size=0;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 8; j++)
            if ((masc[i] & 1 << j) == 0)
                size++;            
    }
    return (int)pow(2, (double)size)-2;
}

int main(int argc, char **argv)
{
    int status, count = 0;
    int *direc, *masc, length;

    argumentos[0] = "ping";
    argumentos[1] = "-c";
    argumentos[2] = "4";
    argumentos[4] = NULL;

    if(argc != 3){
        fprintf(stderr, "The program needs to be given two arguments\n");
        exit(1);
    }
    dirmask(argv[1], argv[2], &direc, &masc);

    length = possibleAdrr(masc);
    scanner mem[length];
    punt = mem;
    fd = open("/dev/null", O_WRONLY);

    generate(direc, masc);
    for (int i = 0; i < length; i++)
    {
        waitpid(punt[i].pid, &status, 0);
        if (WEXITSTATUS(status) == 0){
            printf("El host %s esta activo\n", punt[i].dir);
            free(punt[i].dir);
            count++;
        }
    }
    printf("Hay un total de %d dispositivos conectados\n", count);
    return 0;
}
