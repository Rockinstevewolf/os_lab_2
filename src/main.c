#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdbool.h>
#include "itoa.h"

int fact(int a){
    if(a!=1){
        pid_t pid;
        int pipe_1[2], pipe_2[2], res=0, buff_a = a-1;
        if((pipe(pipe_1) == -1) || (pipe(pipe_2) == -1)){
        	write(STDERR_FILENO, "ERROR: pipe\n", sizeof "ERROR: pipe\n" - 1);
        	exit(EXIT_FAILURE);
        }
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
        else if(pid < 0){
        	write(STDERR_FILENO, "ERROR: fork\n", sizeof "ERROR: fork\n" - 1);
        	exit(EXIT_FAILURE);
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
    bool flag = true;
    int input;
    char save[100];
    int save_len;
    int sol = 0;
    char sol_str[100];
    int sol_str_len;
    write(STDOUT_FILENO, "1. Чтобы вычислить сумму от 1 до n введите число\n", sizeof "1. Чтобы вычислить сумму от 1 до n введите число\n" - 1);
    write(STDOUT_FILENO, "2. Чтобы завершить программу, введите '0'\n", sizeof "2. Чтобы завершить программу, введите '0'\n" - 1);
    while(flag){
        write(STDOUT_FILENO, "> ", sizeof "> " - 1);
        save_len = read(STDIN_FILENO, save, 100); //считывание команды
        save[save_len-1] = '\0';
       	input = atoi(save);
        if(input == 0){
            flag = false;
            write(STDOUT_FILENO, "Выход из программы...\n", sizeof "Выход из программы...\n" - 1);
        }
        else{
            if(input < 0)
            	write(STDOUT_FILENO, "Ошибка, введено некорректное число!\n", sizeof "Ошибка, введено некорректное число!\n" - 1);
            else if(input > 0){
                sol = fact(input);
                sol_str_len = itoa(sol, sol_str);
                write(STDOUT_FILENO, "Итоговая сумма = ", sizeof "Итоговая сумма = " - 1);
            	write(STDOUT_FILENO, sol_str, sol_str_len);
                write(STDOUT_FILENO, "\n", sizeof "\n" - 1);
            }
        }
    }
}
