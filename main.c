#include <stdio.h>
#include <stdlib.h>
#include "src/visual.h"

void initmenu(int*, int*);

int main()
{
    system("clear");

    int tipo = 0, operation = 0;
    initmenu(&tipo, &operation);

    printf("Tipo: %d\n", tipo);
    printf("Operation: %d\n", operation);

    return 0;
}

// Função para inicializar o menu e obter o tipo e a operação
void initmenu(int* tipo, int* operation) {
    int temp;
    do {
        *tipo = mainMenu();
        switch(*tipo) {
            case 1:
                *tipo = 1;
                temp = productsMenu();
                if (temp == 0) {
                    *operation = 0;
                    break;
                }
                *operation = temp;
                return;
            case 2:
                *tipo = 2;
                temp = listsMenu();
                if (temp == 0) {
                    *operation = 0;
                    break;
                }
                *operation = temp;
                return;
            case 0:
                *tipo = 0;
                *operation = 0;
                return;
            default:
                break;
        }
    } while (*operation == 0);
}