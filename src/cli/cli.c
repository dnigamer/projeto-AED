//
// Created by Gonçalo Miranda on 03/06/2024.
//

#include "cli.h"

#define JANELA 100

// Ajusta o tamanho da janela/terminal
void setWindowSize(int width, int height) {
    char command[30];
    sprintf(command, "printf '\e[8;%d;%dt'", height, width);
    system(command);
}

// Cria uma linha de separação tipo "+----...----+" com tamanho especificado
void line(int size) {
    printf("+");
    if (size == 0)
        size = 40;
    for (int i = 0; i < size; ++i)
        printf("-");
    printf("+\n");
}

// Imprime uma string centrada numa linha de tamanho especificado
void print(char* str, int size) {
    setlocale(LC_ALL, "Portuguese");
    int len = (int) strlen(str);
    int numSpaces = size - len - 2;
    printf("| %s", str);
    for (int i = 0; i < numSpaces; i++)
        printf(" ");
    printf(" |\n");
}

// Menu principal
int mainMenu(char* nomeLoja, int info, char* infoStr) {
    clear();

    // Título
    line(50);
    char title[50];
    strcpy(title, "Bem-vindo à loja \"");
    strcat(title, nomeLoja);
    strcat(title, "\"!");
    print(title, 52);

    // Opções
    line(50);
    print("1 - Abrir loja", 50);
    print("2 - Carregar/guardar stock", 50);
    print("3 - Estatísticas", 51);
    print("4 - Definições", 52);
    print("", 50);
    print("5 - Sair", 50);
    line(50);

    // Processa opção
    int opcao;
    if (info) {
        printf("!! %s\n", infoStr);
    }
    printf("Escolha uma opção: ");
    opcao = (int) getch();
    fflush(stdin);

    return opcao - '0';
}

// Menu da loja (1 do menu principal)
int menuLoja(char* nomeLoja, int info, char* infoStr) {
    clear();

    // Título
    line(50);
    char title[50];
    strcpy(title, "Loja \"");
    strcat(title, nomeLoja);
    strcat(title, "\"");
    print(title, 51);

    // Opções
    line(50);
    print("1 - Explorar stock", 50);
    print(" - Linhas de produtos:", 50);
    print("2 - Adicionar linha de produtos", 50);
    print("3 - Remover linha de produtos", 50);
    print("4 - Editar linha de produtos", 50);
    print(" - Produtos:", 50);
    print("5 - Adicionar produto", 50);
    print("6 - Remover produto", 50);
    print("7 - Editar produto", 50);
    print("", 50);
    print("8 - Voltar", 50);
    line(50);

    // Processa opção
    int opcao;
    if (info) {
        printf("!! %s\n", infoStr);
    }
    printf("Escolha uma opção: ");
    opcao = (int) getch();
    fflush(stdin);

    return opcao - '0';
}

int stockLoadSaveDialog(StockLoja *stock, int opcao) {
    clear();

    line(50);
    print("Introduza o nome do ficheiro: ", 50);
    line(50);
    char filename[100];
    printf(": ");
    fgets(filename, sizeof(filename), stdin);

    filename[strcspn(filename, "\n")] = 0;

    line(50);

    if (opcao == 1) {
        carregarStock(stock, filename);
        return 0;
    } else if (opcao == 2) {
        guardarStock(stock, filename);
        return 0;
    }
    fflush(stdin);

    return 1;
}

int stockLoadSaveMenu(char* nomeLoja, int info, char* infoStr) {
    clear();

    // Título
    line(50);
    char title[50];
    strcpy(title, "Loja \"");
    strcat(title, nomeLoja);
    strcat(title, "\"");
    print(title, 51);

    // Opções
    line(50);
    print("1 - Carregar stock", 50);
    print("2 - Guardar stock", 50);
    print("", 50);
    print("3 - Voltar", 50);
    line(50);

    // Processa opção
    int opcao;
    if (info) {
        printf("!! %s\n", infoStr);
    }
    printf("Escolha uma opção: ");
    opcao = (int) getch();
    fflush(stdin);

    return opcao - '0';
}

// Inicia a interface de linha de comandos
void startCLI(StockLoja *stock) {
    clear();
    setWindowSize(110, 30);

    int opcao, info = 0, finishStockLoadSave = 0;
    char infoStr[50];
    do {
        opcao = mainMenu(stock->nome, info, infoStr);
        switch (opcao) {
            case 1: // abrir loja
                info = 1;
                strcpy(infoStr, "A abrir loja...");

                int opcaoLoja;
                do {
                    opcaoLoja = menuLoja(stock->nome, info, infoStr);
                    switch (opcaoLoja) {
                        case 1: // explorar stock
                            info = 1;
                            strcpy(infoStr, "A explorar stock...");
                            break;
                        case 2: // adicionar linha de produtos
                            info = 1;
                            strcpy(infoStr, "A adicionar linha de produtos...");
                            break;
                        case 3: // remover linha de produtos
                            info = 1;
                            strcpy(infoStr, "A remover linha de produtos...");
                            break;
                        case 4: // editar linha de produtos
                            info = 1;
                            strcpy(infoStr, "A editar linha de produtos...");
                            break;
                        case 5: // adicionar produto
                            info = 1;
                            strcpy(infoStr, "A adicionar produto...");
                            break;
                        case 6: // remover produto
                            info = 1;
                            strcpy(infoStr, "A remover produto...");
                            break;
                        case 7: // editar produto
                            info = 1;
                            strcpy(infoStr, "A editar produto...");
                            break;
                        case 8: // voltar
                            info = 1;
                            strcpy(infoStr, "A voltar...");
                            break;
                        default:
                            info = 1;
                            strcpy(infoStr, "Opção inválida!");
                            break;
                    }
                } while (opcaoLoja != 8);
                break;
            case 2: // carregar/guardar stock
                info = 1;
                strcpy(infoStr, "A carregar/guardar stock...");

                int opcaoLoadSave;
                do {
                    opcaoLoadSave = stockLoadSaveMenu(stock->nome, info, infoStr);
                    switch (opcaoLoadSave) {
                        case 1: // carregar stock
                            info = 1;
                            strcpy(infoStr, "A carregar stock...");
                            if(stockLoadSaveDialog(stock, 1) == 0) {
                                info = 1;
                                strcpy(infoStr, "Stock carregado com sucesso!");
                                finishStockLoadSave = 1;
                            } else {
                                strcpy(infoStr, "Erro ao carregar stock!");
                            }
                            break;
                        case 2: // guardar stock
                            info = 1;
                            strcpy(infoStr, "A guardar stock...");
                            if(stockLoadSaveDialog(stock, 2) == 0) {
                                info = 1;
                                strcpy(infoStr, "Stock guardado com sucesso!");
                                finishStockLoadSave = 1;
                            } else {
                                info = 1;
                                strcpy(infoStr, "Erro ao guardar stock!");
                            }
                            break;
                        case 3: // voltar
                            info = 1;
                            strcpy(infoStr, "A voltar...");
                            break;
                        default:
                            break;
                    }
                } while (opcaoLoadSave != 3 && !finishStockLoadSave);
                break;
            case 3: // estatísticas
                info = 1;
                strcpy(infoStr, "A calcular estatísticas...");
                break;
            case 4: // definições
                info = 1;
                strcpy(infoStr, "A abrir definições...");
                break;
            case 5: // saida
                clear();
                printf("Obrigado por usar o nosso programa!\n");
                printf("Até à próxima!\n\n");
                break;
            default:
                break;
        }
    } while (opcao != 5);
}

