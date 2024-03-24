#include <stdio.h>
#include <stdlib.h>
#include "src/visual.h"

#if defined(_WIN32) || defined(_WIN64)
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

void initmenu(int*, int*);

int main()
{
    system(CLEAR);

    int tipo = 0, operation = 0;
    initmenu(&tipo, &operation);

    switch (tipo) {
        case 1:
            switch (operation) {
                case 1:
                    productsMenu(1);
                    break;
                case 2:
                    productsMenu(2);
                    break;
                case 3:
                    productsMenu(3);
                    break;
                case 4:
                    productsMenu(4);
                    break;
                case 5:
                    productsMenu(5);
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
        default:
            break;
    }

    return 0;
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
                return;
            case 2:
                temp = listsInitMenu();
                if (temp == 0)
                    break;
                *operation = temp;
                return;
            case 0:
                *operation = 0;
                return;
            default:
                break;
        }
    } while (*operation == 0);
}