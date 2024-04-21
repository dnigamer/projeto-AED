#include <stdio.h>
#include <stdlib.h>
#include "src/visual.h"
#include "src/produtos.h"

#if defined(_WIN32) || defined(_WIN64)
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

void intmenu(int*, int*);

int main()
{
    system(CLEAR);

    StockLoja stockGeral = { "Loja Gira", 0, NULL };
    LinhaProdutos lojinha = criarLinhaProdutos(1, "Legumes");
    adicionarLinhaProdutos(&stockGeral, &lojinha);

    int tipo = 0, operation = 0;

    do {
        initmenu(&tipo, &operation);

        switch (tipo) {
            case 1:
                switch (operation) {
                    case 1:
                        productsMenu(1, &stockGeral);
                        break;
                    case 2:
                        productsMenu(2, &stockGeral);
                        break;
                    case 3:
                        productsMenu(3, &stockGeral);
                        break;
                    case 4:
                        productsMenu(4, &stockGeral);
                        break;
                    case 5:
                        productsMenu(5, &stockGeral);
                        break;
                    default:
                        break;
                }
                break;
            case 2:
                switch (operation) {
                    case 1:
                        printf("Adicionar uma nova linha\n");
                        break;
                    case 2:
                        printf("Remover uma linha\n");
                        break;
                    case 3:
                        printf("Editar uma linha\n");
                        break;
                    case 4:
                        printf("Mostrar uma linha\n");
                        break;
                    case 5:
                        printf("Procurar uma linha\n");
                        break;
                    default:
                        break;
                }
                break;
            case 0:
                system(CLEAR);
                printf("Saindo...\n\n");
                exit(0);
            default:
                break;
        }
    } while (1);
}

// Função para inicializar o menu e obter o tipo e a operação
void initmenu(int* tipo, int* operation) {
    int temp;
    do {
        *tipo = mainMenu();
        switch(*tipo) {
            case 1:
                temp = productsInitMenu();
                if (temp == 0)
                    break;
                *operation = temp;
                break;
            case 2:
                temp = listsInitMenu();
                if (temp == 0)
                    break;
                *operation = temp;
                break;
            case 0:
                *operation = 0;
                break;
            default:
                exit(0);
        }
    } while (*operation == 0 && *tipo != 0);
}