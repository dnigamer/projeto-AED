#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iohelp.h"
#include "produtos.h"

#if defined(_WIN32) || defined(_WIN64)
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define clear() system(CLEAR)

// Draw a line with defined size or 40
void line(int size) {
    printf("+");
    if (size == 0)
        size = 40;
    for (int i = 0; i < size; ++i)
        printf("-");
    printf("+\n");
}

// Products menu
int productsInitMenu() {
    char pick;

    clear();
    line(0);
    printf("| Escolha uma opção:                     |\n");
    line(0);
    printf("| 1. Adicionar um produto                |\n");
    printf("| 2. Remover um produto                  |\n");
    printf("| 3. Editar um produto                   |\n");
    printf("| 4. Mostrar produto                     |\n");
    printf("| 5. Procurar um produto                 |\n");
    printf("|                                        |\n");
    printf("| 0. Voltar                              |\n");
    line(0);

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
int listsInitMenu() {
    char pick;

    clear();
    line(0);
    printf("| Escolha uma opção:                     |\n");
    line(0);
    printf("| 1. Adicionar uma nova linha            |\n");
    printf("| 2. Remover uma linha                   |\n");
    printf("| 3. Editar uma linha                    |\n");
    printf("| 4. Mostrar uma linha                   |\n");
    printf("| 5. Mostrar todas as linhas             |\n");
    printf("| 6. Mostrar número total de linhas      |\n");
    printf("|                                        |\n");
    printf("| 0. Voltar                              |\n");
    line(0);

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

    clear();
    fflush(stdin);
    line(0);
    printf("| Bem-vindo ao sistema de produtos!      |\n");
    line(0);
    line(0);
    printf("| Escolha uma opção:                     |\n");
    line(0);
    printf("| 1. Opções sobre produtos               |\n");
    printf("| 2. Opções sobre linhas                 |\n");
    printf("|                                        |\n");
    printf("| 0. Sair                                |\n");
    line(0);

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

void mostraEscolherLinha(int campSel, LinhaProdutos linhatmp, int info) {
    clear();

    // Cabeçalho
    int windowSize = 50 + (int) strlen("Escolher uma linha");
    line(windowSize);
    printf("| Escolher uma linha");
    for (int i = 1; i < 50; ++i)
        printf(" ");
    printf("|\n");
    line(windowSize);

    // Corpo
    for (int i = 0; i < 2; ++i) {
        if (campSel == i)
            printf("| > ");
        else
            printf("|   ");
        if (i == 0)
            printf("%-13s: %-*d |\n", "Codigo", (windowSize - 19), linhatmp.codigo);
        else
            printf("%-13s: %-*s |\n", "Nome", (windowSize - 19), linhatmp.nome);
    }
    line(windowSize);

    // Informação pé
    if (info == 1) {
        printf("Pressione F para concluir. Pressione S para descartar e sair. ");
    }
}

LinhaProdutos* escolherLinha(StockLoja *stock) { // Mostra as linhas de produtos e permite escolher uma
    clear();

    LinhaProdutos linhatmp = criarLinhaProdutos(0, "");
    int campSel = 0;
    char input[50];
    int key;

    do {
        clear();
        mostraEscolherLinha(campSel, linhatmp, 1);

        key = (unsigned char) getch();
        if (key == '\033'){
            getch();
            switch (getch()) {
                case 72:
                case 65:
                    if (campSel > 0)
                        campSel--;
                    break;
                case 80:
                case 66:
                    if (campSel < 1)
                        campSel++;
                    break;
            }
        } else if (key == '\n') {
            mostraEscolherLinha(campSel, linhatmp, 0);

            printf("Introduza %s: ", campSel == 0 ? "Codigo" : "Nome");
            fgets(input, 50, stdin);
            input[strcspn(input, "\n")] = 0;
            if (campSel == 0) {
                linhatmp.codigo = strtol(input, NULL, 10);
            } else {
                strcpy(linhatmp.nome, input);
            }
        } else if (key == 102) {
            for (int i = 0; i < stock->num_linhas; ++i) {
                if (linhatmp.codigo != 0) {
                    if (stock->linhas[i].codigo == linhatmp.codigo) {
                        return &stock->linhas[i];
                    }
                } else if (strcmp(stock->linhas[i].nome, linhatmp.nome) == 0) {
                    return &stock->linhas[i];
                }
            }
            clear();
            printf("Linha não encontrada!\n\n");
            return NULL;
        } else if (key == 115) {
            clear();
            printf("Operação cancelada!\n\n");
            return NULL;
        }
    } while (1);
}

void mostraMenuProdutos(int tipo, int campoSelecionado, char *campos[], LinhaProdutos *linhatmp, Produto *prodtmp, int info) {
    clear();

    // Cabeçalho
    char titulo[40];
    sprintf(titulo, "%s um produto", tipo == 1 ? "Adicionar" : tipo == 2 ? "Remover" : tipo == 3 ? "Editar" : tipo == 4 ? "Mostrar" : "Procurar");
    int windowSize = 50 + (int) strlen(titulo);
    line(windowSize);
    printf("| %s", titulo);
    for (int i = 0; i < 49; ++i)
        printf(" ");
    printf("|\n");
    line(windowSize);
    printf("|\033[1m Nome da linha: \033[0m %s", linhatmp->nome);
    for (int i = 0; i < 49; ++i)
        printf(" ");
    printf("|\n");
    printf("| Codigo da linha: %d", linhatmp->codigo);
    for (int i = 0; i < 49; ++i)
        printf(" ");
    printf("|\n");
    line(windowSize);

    // Corpo
    if (tipo == 1 || tipo == 3 || tipo == 5) {
        for (int i = 0; i < 6; ++i) {
            if (campoSelecionado == i)
                printf("| > ");
            else
                printf("|   ");
            char codigo[12], preco[15], peso[16], quantidade[12];
            sprintf(codigo, "%d", prodtmp->codigo);
            sprintf(preco, "%.2f EUR", prodtmp->preco);
            sprintf(peso, "%.3f kg", prodtmp->peso);
            sprintf(quantidade, "%d", prodtmp->quantidade);
            printf("%-13s: %-*s |\n", campos[i], (windowSize - 19), (i == 0 ? codigo : i == 1 ? prodtmp->nome : i == 2 ? prodtmp->marca: i == 3 ? peso: i == 4 ? preco: quantidade));
        }
    } else if (tipo == 2 || tipo == 4) {
        char codigo[12];
        sprintf(codigo, "%d", prodtmp->codigo);
        printf("| > %-13s: %-*s |\n", "Codigo", (windowSize - 19), codigo);
    }
    line(windowSize);

    // Informação pé
    if (info == 1 && tipo == 1) {
        printf("Introduza valores para todos os campos.\n");
        printf("Pressione F para concluir. Pressione S para descartar e sair. ");
    } else if (info == 1 && tipo == 2) {
        printf("Introduza o código do produto a remover.\n");
        printf("Pressione F para concluir a remoção. Pressione S para descartar e sair. ");
    } else if (info == 1 && tipo == 3) {
        printf("Edite tudo o que deseja\n");
        printf("Pressione F para concluir. Pressione S para descartar e sair. ");
    } else if (info == 1 && tipo == 4) {
        printf("Pressione F para fechar a visualização. Pressione S para descartar e sair. ");
    } else if (info == 1 && tipo == 5) {
        printf("Introduza valores para os campos a considerar na procura.\n");
        printf("Pressione F para concluir. Pressione S para descartar e sair. ");
    }
}

int confirmaMenu() {
    clear();

    printf("\n");
    line(0);
    printf("| Deseja confirmar a operação?           |\n");
    line(0);
    printf("| 1. Sim                                 |\n");
    printf("| 2. Não                                 |\n");
    line(0);
    printf("Selecione uma opção: ");

    do {
        char pick = getche();
        switch (pick) {
            case '1':
                printf("\n");
                return 1;
            case '2':
                printf("\n");
                return 0;
            default:
                break;
        }
    } while (1);

}

int productsMenu(int operation, StockLoja *stock) {
    clear();

    int key;
    int campSel = 0;
    int breakLoop = 0;
    char input[50];
    Produto prodtmp = criarProduto(0, "", "", 0, 0, 0);
    LinhaProdutos *linhatmp;
    char *fields[] = {"Codigo", "Nome", "Marca", "Peso", "Preco", "Quantidade"};

    switch (operation) {
        case 1: // Adicionar produto
            clear();

            linhatmp = escolherLinha(stock);
            if (linhatmp == NULL)
                return -1;

            do {
                mostraMenuProdutos(1, campSel, fields, linhatmp, &prodtmp, 1);    // mostra o menu com a informação
                key = (unsigned char) getch();

                if (key == '\033'){
                    getch();
                    switch (getch()) {
                        case 72:
                        case 65:
                            if (campSel > 0)
                                campSel--;
                            break;
                        case 80:
                        case 66:
                            if (campSel < 5)
                                campSel++;
                            break;
                    }
                } else if (key == '\n') {
                    mostraMenuProdutos(1, campSel, fields, linhatmp, &prodtmp, 0); // mostra o menu sem a informação
                    printf("Introduza %s: ", fields[campSel]);
                    fgets(input, 50, stdin);
                    input[strcspn(input, "\n")] = 0;

                    switch (campSel) {
                        case 0:
                            prodtmp.codigo = strtol(input, NULL, 10);
                            break;
                        case 1:
                            strcpy(prodtmp.nome, input);
                            break;
                        case 2:
                            strcpy(prodtmp.marca, input);
                            break;
                        case 3:
                            prodtmp.peso = strtof(input, NULL);
                            break;
                        case 4:
                            prodtmp.preco = strtof(input, NULL);
                            break;
                        case 5:
                            prodtmp.quantidade = strtol(input, NULL, 10);
                            break;
                        default:
                            break;
                    }
                } else if (key == 102) { // quando F pressionado
                    int confirm = confirmaMenu();
                    if (confirm == 1) {
                        //adicionarProduto(&prodtmp);
                        printf("\nprodtmp: %d \"%s\" \"%s\" %.3f %.2f %d\n", prodtmp.codigo, prodtmp.nome, prodtmp.marca, prodtmp.peso, prodtmp.preco, prodtmp.quantidade);
                        printf("Produto adicionado com sucesso!\n");
                        breakLoop = 1;
                    } else {
                        printf("Operação cancelada!\n");
                        breakLoop = 1;
                    }
                    printf("\n");
                } else if (key == 115) { // quando S pressionado
                    return -1; // retorna cancelado
                }
            } while (breakLoop == 0);
            break;
        case 2: // Remover produto
            clear();

            linhatmp = escolherLinha(stock);
            if (linhatmp == NULL)
                return -1;

            do {
                mostraMenuProdutos(2, campSel, fields, linhatmp, &prodtmp, 1);
                key = (unsigned char) getch();

                if (key == '\n') {
                    mostraMenuProdutos(2, campSel, fields, linhatmp, &prodtmp, 0);
                    printf("Introduza o código: ");
                    fgets(input, 50, stdin);
                    input[strcspn(input, "\n")] = 0;
                    prodtmp.codigo = strtol(input, NULL, 10);
                } else if (key == 102) {
                    int confirm = confirmaMenu();
                    if (confirm == 1) {
                        //removerProduto(&prodtmp);
                        printf("\nprodtmp: %d \"%s\" \"%s\" %.3f %.2f %d\n", prodtmp.codigo, prodtmp.nome, prodtmp.marca, prodtmp.peso, prodtmp.preco, prodtmp.quantidade);
                        printf("Produto removido com sucesso!\n");
                        breakLoop = 1;
                    } else {
                        printf("Operação cancelada!\n");
                        breakLoop = 1;
                    }
                    printf("\n");
                }

            } while (breakLoop == 0);
            break;
        case 3: // Editar produto
            clear();

            linhatmp = escolherLinha(stock);
            if (linhatmp == NULL)
                return -1;

            do {
                mostraMenuProdutos(3, campSel, fields, linhatmp, &prodtmp, 1);
                key = (unsigned char) getch();

                if (key == '\n') {
                    mostraMenuProdutos(3, campSel, fields, linhatmp, &prodtmp, 0);
                    printf("Introduza %s: ", fields[campSel]);
                    fgets(input, 50, stdin);
                    input[strcspn(input, "\n")] = 0;

                    switch (campSel) {
                        case 0:
                            prodtmp.codigo = strtol(input, NULL, 10);
                            break;
                        case 1:
                            strcpy(prodtmp.nome, input);
                            break;
                        case 2:
                            strcpy(prodtmp.marca, input);
                            break;
                        case 3:
                            prodtmp.peso = strtof(input, NULL);
                            break;
                        case 4:
                            prodtmp.preco = strtof(input, NULL);
                            break;
                        case 5:
                            prodtmp.quantidade = strtol(input, NULL, 10);
                            break;
                        default:
                            break;
                    }
                } else if (key == '\033'){
                    getch();
                    switch (getch()) {
                        case 72:
                        case 65:
                            if (campSel > 0)
                                campSel--;
                            break;
                        case 80:
                        case 66:
                            if (campSel < 5)
                                campSel++;
                            break;
                    }
                } else if (key == 102) {
                    int confirm = confirmaMenu();
                    if (confirm == 1) {
                        //atualizarProduto(&prodtmp);
                        printf("\nprodtmp: %d \"%s\" \"%s\" %.3f %.2f %d\n", prodtmp.codigo, prodtmp.nome, prodtmp.marca, prodtmp.peso, prodtmp.preco, prodtmp.quantidade);
                        printf("Produto atualizado com sucesso!\n");
                        breakLoop = 1;
                    } else {
                        printf("Operação cancelada!\n\n");
                        breakLoop = 1;
                    }
                    printf("\n");
                } else if (key == 115) {
                    printf("Operação cancelada!\n\n");
                    return -1;
                }
            } while (breakLoop == 0);
            break;
        case 4:
            printf("Mostrar um produto\n");
            break;
        case 5:
            printf("Procurar um produto\n");
            break;
        default:
            break;
    }
    return 0;
}