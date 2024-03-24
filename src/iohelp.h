#ifndef INPUTHELPER_H
#define INPUTHELPER_H

#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

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

int display_len(const char *s) {
    setlocale(LC_ALL, "");  // set locale to handle special characters
    int len = 0;
    wchar_t wc;
    mbstate_t state = {0};
    while (*s != '\0') {
        size_t n = mbrtowc(&wc, s, MB_CUR_MAX, &state);
        if (n == (size_t)-1 || n == (size_t)-2)
            break;  // stop on encoding error
        s += n;
        len += wcwidth(wc);
    }
    return len;
}

#endif