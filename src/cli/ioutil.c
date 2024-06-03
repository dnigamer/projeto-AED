//
// Created by Gonçalo Miranda on 03/06/2024.
//

#include "ioutil.h"

#if defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#define CLEAR "cls"
#else
#define CLEAR "clear"
#include <termios.h>
#include <stdio.h>
#include <unistd.h>

static struct termios old, current;

void initTermios(int echo) {
    tcgetattr(0, &old);
    current = old;
    current.c_lflag &= ~ICANON;
    switch (echo) {
        case 1:
            current.c_lflag |= ECHO;
            break;
        case 0:
            current.c_lflag &= ~ECHO;
            break;
        default:
            break;
    }
    tcsetattr(0, TCSANOW, &current);
}

void resetTermios(void) {
    tcsetattr(0, TCSANOW, &old);
}

char getch() {
    char ch;
    initTermios(0);
    ch = (char) getchar();
    resetTermios();
    return ch;
}

char getche() {
    char ch;
    initTermios(1);
    ch = (char) getchar();
    resetTermios();
    return ch;
}

#endif