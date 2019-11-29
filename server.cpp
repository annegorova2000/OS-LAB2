#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include "lin-2-list-barrier.h"

#define LISTS_NUM 10

int list_no(int *st);

int main(void)
{
    int pipeFdToMain[2]; // Передаем в main
    int pipeFdFromMain[2]; // Получаем от main
    std::string outStr;
    if (pipe(pipeFdToMain) == -1) { // Создание 1 пайпа
        std::cout << "Cannot create pipe\n";
        exit(-1);
    }
    if (pipe(pipeFdFromMain) == -1) { // Создание 2 пайпа
        std::cout << "Cannot create pipe\n";
        exit(-1);
    }
    int serverPid = fork(); // Создание дочернего процесса
    if (serverPid < 0) {
        std::cout << "Cannot create server procces\n"; // Ошибка
        exit(-1);
    }else if (serverPid == 0) { // Дочерний процесс
        std::cout << "Create main procces\n";
        close(pipeFdToMain[1]);
        close(pipeFdFromMain[0]);
        execl("./main.out",   std::to_string(pipeFdFromMain[1]).c_str(),
                              std::to_string(pipeFdToMain[0]).c_str(), (char*)NULL);
        std::cout << "Cannot execute program\n" << errno << "\n";
        exit(-1);
    }
    close(pipeFdToMain[0]); //Закрываю чтение в main
    close(pipeFdFromMain[1]); //Закрываю запись из main
    
    if (dup2(pipeFdFromMain[0], STDIN_FILENO) == -1) { //Перенаправление папйпа в стандартный ввод
        std::cout << "Cannot dup2\n";
        exit(-1);
    }
    
    int buf;
    char c;
    char fb;
    int st;
    int no;
    int val;
    List *A[LISTS_NUM];
    for (int i = 0; i < LISTS_NUM; i++) {
        A[i] = NULL;
    }
    while(true) {
        scanf("%c", &c);
        switch (c) {
            case 'c':
                if (list_no(&st)) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] == NULL) {
                    A[st] = list_create();
                    buf = 0;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                } else {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                }
                break;

            case 'd':
                if (list_no(&st)) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] != NULL) {
                    list_destroy(&A[st]);
                    buf = 0;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                } else {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                }
                break;
            case 'a':
                if (list_no(&st)) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] == NULL) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                if (scanf(" %c", &fb) != 1) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                switch (fb) {
                    case 'b':
                        while (scanf("%d", &val) == 1) {
                            list_push_front(A[st], val);
                            if (getchar() == '\n') {break;}
                        }
                        buf = 0;
                        write(pipeFdToMain[1], &buf, sizeof(int));
                        break;
             
                    case 'f':
                        while (scanf("%d", &val) == 1) {
                            list_push_back(A[st], val);
                            if (getchar() == '\n') {break;}
                        }
                        buf = 0;
                        write(pipeFdToMain[1], &buf, sizeof(int));
                        break;
                    case 'i':
                        if (scanf("%d %d",&no , &val) != 2) {
                            buf = -1;
                            write(pipeFdToMain[1], &buf, sizeof(int));
                            break;
                        }
                        if (list_insert(A[st], no, val)) {
                            buf = -1;
                            write(pipeFdToMain[1], &buf, sizeof(int));
                        } else {
                            buf = 0;
                            write(pipeFdToMain[1], &buf, sizeof(int));
                        }
                        break;
                    default:
                        buf = -1;
                        write(pipeFdToMain[1], &buf, sizeof(int));
                        break;
                }
                break;
            case 'o':
                if (list_no(&st)) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] == NULL) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                if (scanf(" %c", &fb) != 1) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                if (list_size(A[st]) == 0) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                switch (fb) {
                    case 'b':
                        buf = 1;
                        write(pipeFdToMain[1], &buf, sizeof(int));
                        buf = list_pop_front(A[st]);
                        write(pipeFdToMain[1], &buf, sizeof(int));
                        break;
                    case 'f':
                        buf = 1;
                        write(pipeFdToMain[1], &buf, sizeof(int));
                        buf = list_pop_back(A[st]);
                        write(pipeFdToMain[1], &buf, sizeof(int));
                        break;
                    case 'i':
                        if (scanf("%d",&no) != 1) {
                            buf = -1;
                            write(pipeFdToMain[1], &buf, sizeof(int));
                            break;
                        }
                        if (abs(no) > list_size(A[st]) || no == 0) {
                            buf = -1;
                            write(pipeFdToMain[1], &buf, sizeof(int));
                        } else {
                            buf = 1;
                            write(pipeFdToMain[1], &buf, sizeof(int));
                            buf = list_get_out(A[st], no);
                            write(pipeFdToMain[1], &buf, sizeof(int));
                        }
                        break;
                    case 'p':
                        if (scanf("%d",&no) != 1) {
                            buf = -1;
                            write(pipeFdToMain[1], &buf, sizeof(int));
                            break;
                        }
                        if (abs(no) > list_size(A[st]) || no == 0) {
                            buf = -1;
                            write(pipeFdToMain[1], &buf, sizeof(int));
                        } else {
                            buf = 1;
                            write(pipeFdToMain[1], &buf, sizeof(int));
                            buf = list_peak(A[st], no);
                            write(pipeFdToMain[1], &buf, sizeof(int));
                        }
                        break;
                    default:
                        buf = -1;
                        write(pipeFdToMain[1], &buf, sizeof(int));
                        break;
                }
                break;
            case 'r':
                if (list_no(&st)) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] == NULL) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                if (scanf("%d",&no) != 1) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                if (list_remove(A[st], no)) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                } else {
                    buf = 0;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                }
                break;
            case 'p':
                if (list_no(&st)) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] == NULL) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                list_print(A[st], pipeFdToMain[1]);
                break;
            case 'q':
                for (int i = 0; i < 10; i++) {
                    if (A[i] != NULL) {
                        list_destroy(&A[i]);
                    }
                }
                buf = -2;
                write(pipeFdToMain[1], &buf, sizeof(int));
                return 0;
                break;
            case 's':
                if (list_no(&st)) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] == NULL) {
                    buf = -1;
                    write(pipeFdToMain[1], &buf, sizeof(int));
                    break;
                }
                buf = 1;
                write(pipeFdToMain[1], &buf, sizeof(int));
                buf = list_size(A[st]);
                write(pipeFdToMain[1], &buf, sizeof(int));
                break;
            case ' ':
                break;
            case '\n':
                break;
            default:
                buf = -1;
                write(pipeFdToMain[1], &buf, sizeof(int));
                while (getchar() != '\n');
        }
        
    }	
}

int list_no(int *st)
{
    if (scanf("%d", st) != 1) {return 1;}
    return *st > LISTS_NUM ? 1 : 0;
}

