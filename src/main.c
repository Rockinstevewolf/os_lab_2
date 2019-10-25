#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h>
#include <string.h>

int fact(int a){
    if(a!=1){
        pid_t pid;
        int pipe_1[2], pipe_2[2], res=0, buff_a = a-1;
        pipe(pipe_1);
        pipe(pipe_2);
        pid = fork();
        if(pid > 0){    //Родитель
            close(pipe_1[0]);
            write(pipe_1[1], &buff_a, sizeof(buff_a));
            close(pipe_1[1]);
            wait(0);
            close(pipe_2[1]);
            read(pipe_2[0], &res, sizeof(res));
            close(pipe_2[0]);
            return res+a;
        }
        else{   //Ребенок
            int get = 0;
            close(pipe_1[1]);
            read(pipe_1[0], &get, sizeof(get));
            close(pipe_1[0]);
            get = fact(get);
            close(pipe_2[0]);
            write(pipe_2[1], &get, sizeof(get));
            close(pipe_2[1]);
            exit(0);
        }
    }
    else
        return a;
}

int main()
{
    int flag = 1;
    int sol = 0;
    int input;
    printf("1. Чтобы вычислить сумму от 1 до n введите число\n");
    printf("2. Чтобы завершить программу, введите '0'\n");
    while(flag){
        printf("> ");
        scanf("%d", &input);
        if(input == 0){
            flag = 0;
            printf("Выход из программы...\n");
        }
        else{
            if(input < 0)
            	printf("Ошибка, введено некорректное число!\n");
            else if(input > 0){
                sol = fact(input);
                printf("Итоговая сумма = %d\n", sol);
            }
        }
    }
}
