#include <stdio.h>
#include <stdlib.h>
#include "inputhelper.h"

#if defined(_WIN32) || defined(_WIN64)
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

// Products menu
int productsMenu() {
    char pick;

    system(CLEAR);
    printf("+----------------------------------------+\n");
    printf("| Escolha uma opção:                     |\n");
    printf("+----------------------------------------+\n");
    printf("| 1. Adicionar um produto                |\n");
    printf("| 2. Remover um produto                  |\n");
    printf("| 3. Editar um produto                   |\n");
    printf("| 4. Mostrar produto                     |\n");
    printf("| 5. Procurar um produto                 |\n");
    printf("|                                        |\n");
    printf("| 0. Voltar                              |\n");
    printf("+----------------------------------------+\n");

    do {
        pick = getch();
        switch (pick) {
            case '1':
                return 1;
            case '2':
                return 2;
            case '3':
                return 3;
            case '4':
                return 4;
            case '5':
                return 5;
            case '0':
                return 0;
            default:
                break;
        }
    } while (1);
}

// Lists menu
int listsMenu() {
    char pick;

    system(CLEAR);
    printf("+----------------------------------------+\n");
    printf("| Escolha uma opção:                     |\n");
    printf("+----------------------------------------+\n");
    printf("| 1. Adicionar uma nova linha            |\n");
    printf("| 2. Remover uma linha                   |\n");
    printf("| 3. Editar uma linha                    |\n");
    printf("| 4. Mostrar uma linha                   |\n");
    printf("| 5. Mostrar todas as linhas             |\n");
    printf("| 6. Mostrar número total de linhas      |\n");
    printf("|                                        |\n");
    printf("| 0. Voltar                              |\n");
    printf("+----------------------------------------+\n");

    do {
        pick = getch();
        switch (pick) {
            case '1':
                return 1;
            case '2':
                return 2;
            case '3':
                return 3;
            case '4':
                return 4;
            case '5':
                return 5;
            case '6':
                return 6;
            case '0':
                return 0;
            default:
                break;
        }
    } while (1);
}

// Main menu
int mainMenu() {
    char pick;

    system(CLEAR);
    printf("+----------------------------------------+\n");
    printf("| Bem-vindo ao sistema de produtos!      |\n");
    printf("+----------------------------------------+\n\n");
    printf("+----------------------------------------+\n");
    printf("| Escolha uma opção:                     |\n");
    printf("+----------------------------------------+\n");
    printf("| 1. Opções sobre produtos               |\n");
    printf("| 2. Opções sobre linhas                 |\n");
    printf("|                                        |\n");
    printf("| 0. Sair                                |\n");
    printf("+----------------------------------------+\n");

    do {
        pick = getch();
        switch (pick) {
            case '1':
                return 1;
            case '2':
                return 2;
            case '0':
                return 0;
            default:
                break;
        }
    } while (1);

}