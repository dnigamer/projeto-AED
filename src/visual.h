#ifndef VISUAL_H
#define VISUAL_H

#if defined(_WIN32) || defined(_WIN64)
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

void mainMenu();
void productsMenu();
void listsMenu();

#endif