#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h>

#include <iostream> 
using namespace std;

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
    bool flag = true;
    int sol = 0;
    int int_input = 0;
    string input;
    cout << "1. Чтобы вычислить сумму от 1 до n введите число" << endl;
    cout << "2. Чтобы завершить программу, введите 'q'" << endl;
    while(flag){
        cout << "> ";
        cin >> input;
        if(input.compare("q")==0){
            flag = false;
        }
        else if(!flag)
            cout << "Выход из программы...";
        else{
            try{
                int_input = stoi(input);
                if(int_input > 0)
                    sol = fact(stoi(input));
                else if(int_input == 0)
                    sol = 1;
                cout << "Итоговая сумма = " << sol << endl;
            }catch(...){
                cout << "Ошибка, введено некорректное число!" << endl;
            }
        }    
    }
}