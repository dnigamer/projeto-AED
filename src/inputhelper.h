#include <termios.h>
#include <stdio.h>
#include <string.h>

static struct termios old, current;

void initTermios(int echo) 
{
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
  }
  tcsetattr(0, TCSANOW, &current);
}

void resetTermios(void) {
    tcsetattr(0, TCSANOW, &old);
}

char getch() 
{
  char ch;
  initTermios(0);
  ch = getchar();
  resetTermios();
  return ch;
}

char getche() 
{
  char ch;
  initTermios(1);
  ch = getchar();
  resetTermios();
  return ch;
}
