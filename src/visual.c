#include <stdio.h>
#include <stdlib.h>
#include "inputhelper.h"
#include "visual.h"

void productsMenu() {
    char opt;
    int invalid = 0;
    do {
        system(CLEAR); // limpa a tela (adaptado para Windows: ver visual.h)
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
        if (invalid) {
            printf("Opção inválida!\n");
        }
        opt = getch();
        switch (opt) {
            case '1':
                printf("Adicionar produto\n");
                break;
            case '2':
                printf("Remover produto\n");
                break;
            case '3':
                printf("Editar produto\n");
                break;
            case '4':
                printf("Mostrar produto\n");
                break;
            case '5':
                printf("Procurar um produto\n");
                break;
            case '0':
                break;
            default:
                invalid = 1;
                break;
        }
    } while (opt != '0');
}

// Lists menu
void listsMenu() {
    char opt;
    int invalid = 0;
    do {
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
        if (invalid) {
            printf("Opção inválida!\n");
        }
        opt = getch();
        switch (opt) {
            case '1':
                printf("Adicionar linha\n");
                break;
            case '2':
                printf("Remover linha\n");
                break;
            case '3':
                printf("Editar linha\n");
                break;
            case '0':
                break;
            default:
                invalid = 1;
                break;
        }
    } while (opt != '0');
}

// Main menu
void mainMenu() {
    char opt;
    int invalid = 0;
    do {
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
        if (invalid) {
            printf("Opção inválida!\n");
        }
        opt = getch();
        switch (opt) {
            case '1':
                productsMenu();
                break;
            case '2':
                listsMenu();
                break;
            case '0':
                printf("Até logo!\n");
                break;
            default:
                invalid = 1;
                break;
        }
    } while (opt != '0');
}