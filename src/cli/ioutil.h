//
// Created by Gonçalo Miranda on 03/06/2024.
//

#ifndef PROJETO_IOUTIL_H
#define PROJETO_IOUTIL_H

#if defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#define CLEAR "cls"
#else
#define CLEAR "clear"
#include <termios.h>

static struct termios old, current;
void initTermios(int echo);
void resetTermios(void);
char getch();
char getche();

#endif

#endif //PROJETO_IOUTIL_H
