#ifndef INPUTHELPER_H
#define INPUTHELPER_H

#if defined(_WIN32) || defined(_WIN64) // Windows
#include <conio.h>
#else                                  // Linux or macOS
#include <termios.h>
static struct termios old, current;
void initTermios(int echo);
void resetTermios(void);
char getch();
char getche();
#endif

#endif