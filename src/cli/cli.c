//
// Created by Gonçalo Miranda on 03/06/2024.
//

#include <wchar.h>
#include "cli.h"

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

// Function to calculate the display width of a string considering special characters
int displayWidth(const char* str) {
    int width = 0;
    wchar_t wc;
    mbstate_t state = {0};
    const char *ptr = str;
    while (*ptr) {
        int len = (int) mbrtowc(&wc, ptr, MB_CUR_MAX, &state);
        if (len > 0) {
            if (iswprint(wc)) {
                width += wcwidth(wc);
            }
            ptr += len;
        } else {
            break;
        }
    }
    return width;
}

// Imprime uma string centralizada num espaço de tamanho especificado
void print(const char* str, int size) {
    setlocale(LC_ALL, "en_US.UTF-8");
    int len = displayWidth(str);
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
    if (strcmp(nomeLoja, "") == 0){
        strcpy(title, "Bem-vindo à loja!");
    } else {
        strcpy(title, "Bem-vindo à loja \"");
        strcat(title, nomeLoja);
        strcat(title, "\"!");
    }
    print(title, 52);

    // Opções
    line(50);
    print("1 - Abrir loja", 50);
    print("2 - Carregar/guardar stock", 50);
    print("3 - Estatísticas", 50);
    print("4 - Definições", 50);
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
    if (strcmp(nomeLoja, "") == 0){
        strcpy(title, "Loja");
    } else {
        strcpy(title, "Loja \"");
        strcat(title, nomeLoja);
        strcat(title, "\"");
    }
    print(title, 50);

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


int adicionarLinhaProdutosDialog(StockLoja *stock) {
    clear();

    line(50);
    print("Introduza o nome da nova linha de produtos: ", 50);
    line(50);
    char nome[100];
    printf(": ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    line(50);

    if (strlen(nome) == 0) {
        return -1;
    }

    int confirmar;
    clear();
    line(75);
    print("? Deseja confirmar a adição da linha de produtos? (1 - Sim, 0 - Não): ", 75);
    line(75);
    printf(": ");
    scanf("%d", &confirmar);
    fflush(stdin);

    if (confirmar == 1) {
        LinhaProdutos linha = criarLinhaProdutos(nome);
        if (adicionarLinhaProdutos(stock, linha) == 1) {
            return 0;
        }
    } else if (confirmar == 0) {
        return -1;
    }

    return 1;
}

int removerLinhaProdutosDialog(StockLoja *stock) {
    clear();

    line(50);
    print("Introduza o ID da linha de produtos a remover: ", 50);
    line(50);
    unsigned int id;
    printf(": ");

    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    id = (unsigned int) strtol(buffer, NULL, 10);

    line(50);

    if (id == 0) {
        return -1;
    }

    LinhaProdutos *linha = obterLinhaProdutosPorID(stock, (int) id);
    if (linha == NULL) {
        return 0;
    }

    int confirmar;
    clear();
    line(75);
    char title[75];
    strcpy(title, "Remover linha de produtos");
    strcat(title, " \"");
    strcat(title, linha->nome);
    strcat(title, "\"");
    print(title, 75);
    print("? Deseja confirmar a remoção da linha de produtos? (1 - Sim, 0 - Não): ", 75);
    line(75);
    printf(": ");
    scanf("%d", &confirmar);
    fflush(stdin);

    if (confirmar == 1) {
        if (removerLinhaProdutos(stock, id) == 1) {
            return 0;
        }
    } else {
        return -1;
    }

    return 1;
}

int editarLinhaProdutosDialog(StockLoja *stock) {
    clear();

    line(50);
    print("Introduza o ID da linha de produtos a editar: ", 50);
    line(50);
    unsigned int id;
    printf(": ");

    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    id = (unsigned int) strtol(buffer, NULL, 10);
    line(50);

    if (id == 0) {
        return -1;
    }

    LinhaProdutos *linha = obterLinhaProdutosPorID(stock, (int) id);
    if (linha == NULL) {
        return 0;
    }

    clear();
    line(50);
    print("Introduza o novo nome da linha de produtos: ", 50);
    line(50);
    char nome[100];
    printf(": ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;
    line(50);
    fflush(stdin);

    int confirmar;
    clear();
    line(75);
    char title[75];
    strcpy(title, "Editar linha de produtos");
    strcat(title, " \"");
    strcat(title, linha->nome);
    strcat(title, "\"");
    print(title, 75);
    print("? Deseja confirmar a edição da linha de produtos? (1 - Sim, 0 - Não): ", 75);
    line(75);
    printf(": ");
    scanf("%d", &confirmar);
    fflush(stdin);

    if (confirmar == 1) {
        strcpy(linha->nome, nome);
        if (atualizarLinhaProdutos(stock, linha) == 1) {
            return 0;
        }
    } else {
        return -1;
    }

    return 1;
}


void mostraMenuProdutos(int tipo, int campoSelecionado, char *campos[], Produto *prodtmp, int info) {
    clear();

    // Cabeçalho
    char titulo[40];
    sprintf(titulo, "%s um produto", tipo == 1 ? "Adicionar" : tipo == 2 ? "Remover" : "Editar");
    line(60);
    print(titulo, 60);
    line(60);

    // Corpo
    if (tipo == 1 || tipo == 3) {
        for (int i = 0; i < 6; ++i) {
            if (campoSelecionado == i)
                printf("| > ");
            else
                printf("|   ");
            char preco[15], quantidade[12];
            sprintf(preco, "%.2f EUR", prodtmp->preco);
            sprintf(quantidade, "%d", prodtmp->quantidade);

            char buffer[100];
            sprintf(buffer, "%-13s: %s", campos[i], i == 0 ? prodtmp->nome : i == 1 ? prodtmp->item : i == 2 ? prodtmp->modelo : i == 3 ? quantidade : i == 4 ? preco : "");
            print(buffer, 56);
        }
    } else if (tipo == 2) {
        for (int i = 0; i < 1; ++i) {
            if (campoSelecionado == i)
                printf("| > ");
            else
                printf("|   ");
            char buffer[100];
            sprintf(buffer, "%-13s: %d", campos[i], prodtmp->produtoID);
            print(buffer, 50);
        }
    }
    line(60);

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
    }
}

int produtosDialog(StockLoja *stock, int id_linha, int operacao) {
    clear();

    int key;
    int campoSelecionado = 0;
    int breakLoop = 0;
    char input[100];
    char *campos[] = {"Nome", "Item", "Modelo", "Quantidade", "Preço", "Parâmetros"};

    Produto prodtmp = {0, 0, 0, "", "", "", 0, 0.0, 0, NULL};

    switch (operacao) {
        case 1: // adicionar
            clear();
            do {
                mostraMenuProdutos(1, campoSelecionado, campos, &prodtmp, 1);
                key = (unsigned char) getch();
                switch (key) {
                    case '\033': // setas
                        getch();
                        switch (getch()) {
                            case 72:
                            case 65:
                                if (campoSelecionado > 0)
                                    campoSelecionado--;
                                break;
                            case 80:
                            case 66:
                                if (campoSelecionado < 5)
                                    campoSelecionado++;
                                break;
                        }
                        break;
                    case '\n': // enter
                        mostraMenuProdutos(1, campoSelecionado, campos, &prodtmp, 0);
                        printf("Introduza o valor para o campo %s: ", campos[campoSelecionado]);
                        fgets(input, sizeof(input), stdin);
                        input[strcspn(input, "\n")] = 0;

                        switch (campoSelecionado) {
                            case 0:
                                strcpy(prodtmp.nome, input);
                                break;
                            case 1:
                                strcpy(prodtmp.item, input);
                                break;
                            case 2:
                                strcpy(prodtmp.modelo, input);
                                break;
                            case 3:
                                prodtmp.quantidade = (unsigned int) strtol(input, NULL, 10);
                                break;
                            case 4:
                                prodtmp.preco = strtod(input, NULL);
                                break;
                            default:
                                break;
                        }
                        break;
                    case 102:
                        clear();
                        int confirmar;
                        line(60);
                        print("Deseja confirmar a adição do produto? (1 - Sim, 0 - Não): ", 60);
                        line(60);
                        printf(": ");
                        scanf("%d", &confirmar);
                        if (confirmar == 1) {
                            adicionarProduto(obterLinhaProdutosPorID(stock, id_linha), &prodtmp);
                            breakLoop = 1;
                        } else {
                            return -1;
                        }
                        break;
                    case 115:
                        return -1;
                    default:
                        break;
                }
            } while (!breakLoop);
            break;
        case 2: // remover
            clear();
            do {
                mostraMenuProdutos(2, campoSelecionado, campos, &prodtmp, 1);
                key = (unsigned char) getch();
                switch (key) {
                    case '\n': // enter
                        mostraMenuProdutos(2, campoSelecionado, campos, &prodtmp, 0);
                        printf("Introduza o código do produto a remover: ");
                        fgets(input, sizeof(input), stdin);
                        input[strcspn(input, "\n")] = 0;

                        unsigned int id_produto = (unsigned int) strtol(input, NULL, 10);
                        int confirmar;
                        clear();
                        line(60);
                        print("Deseja confirmar a remoção do produto? (1 - Sim, 0 - Não): ", 60);
                        line(60);
                        printf(": ");
                        scanf("%d", &confirmar);
                        fflush(stdin);

                        if (confirmar == 1) {
                            removerProduto(obterLinhaProdutosPorID(stock, id_linha), id_produto);
                            breakLoop = 1;
                        } else {
                            return -1;
                        }
                        break;
                    case 102:
                        breakLoop = 1;
                        break;
                    case 115:
                        return -1;
                    default:
                        break;
                }
            } while (!breakLoop);
            break;
        case 3:
            clear();

            // TODO: Escolher linha e produto na linha a editar

            do {
                mostraMenuProdutos(3, campoSelecionado, campos, &prodtmp, 1);
                key = (unsigned char) getch();
                switch (key) {
                    case '\033': // setas
                        getch();
                        switch (getch()) {
                            case 72:
                            case 65:
                                if (campoSelecionado > 0)
                                    campoSelecionado--;
                                break;
                            case 80:
                            case 66:
                                if (campoSelecionado < 5)
                                    campoSelecionado++;
                                break;
                        }
                        break;
                    case '\n': // enter
                        mostraMenuProdutos(3, campoSelecionado, campos, &prodtmp, 0);
                        printf("Introduza o valor para o campo %s: ", campos[campoSelecionado]);
                        fgets(input, sizeof(input), stdin);
                        input[strcspn(input, "\n")] = 0;

                        switch (campoSelecionado) {
                            case 0:
                                strcpy(prodtmp.nome, input);
                                break;
                            case 1:
                                strcpy(prodtmp.item, input);
                                break;
                            case 2:
                                strcpy(prodtmp.modelo, input);
                                break;
                            case 3:
                                prodtmp.quantidade = (unsigned int) strtol(input, NULL, 10);
                                break;
                            case 4:
                                prodtmp.preco = strtod(input, NULL);
                                break;
                            default:
                                break;
                        }
                        break;
                    case 102:
                        line(50);
                        int confirmar;
                        print("Deseja confirmar a edição do produto? (1 - Sim, 0 - Não): ", 50);
                        line(50);
                        printf(": ");
                        scanf("%d", &confirmar);
                        if (confirmar == 1) {
                            atualizarProduto(obterLinhaProdutosPorID(stock, id_linha), &prodtmp);
                            breakLoop = 1;
                        } else {
                            printf("Operaçao cancelada.\n");
                        }
                        break;
                    case 115:
                        return -1;
                    default:
                        break;
                }
            } while (!breakLoop);
            break;
        default:
            break;
    }
    return 1;
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
        return carregarStock(stock, filename);
    } else if (opcao == 2) {
        return guardarStock(stock, filename);
    }
    fflush(stdin);

    return 1;
}

int stockLoadSaveMenu(char* nomeLoja, int info, char* infoStr) {
    clear();

    // Título
    line(50);
    char title[50];
    strcpy(title, "Carregar ou guardar stock da loja");
    print(title, 50);

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
    char infoStr[50] = "";
    do {
        opcao = mainMenu(stock->nome, info, infoStr);
        switch (opcao) {
            case 1: // abrir loja
                if (stock->lista_linhas == NULL) {
                    info = 1;
                    strcpy(infoStr, "Stock não carregado!");
                    break;
                }

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
                            int resultadoAD = adicionarLinhaProdutosDialog(stock);
                            if (resultadoAD == 1) {
                                strcpy(infoStr, "Linha de produtos adicionada com sucesso!");
                            } else if (resultadoAD == -1) {
                                strcpy(infoStr, "Operação cancelada!");
                            } else {
                                strcpy(infoStr, "Erro ao adicionar linha de produtos!");
                            }
                            break;
                        case 3: // remover linha de produtos
                            info = 1;
                            strcpy(infoStr, "A remover linha de produtos...");
                            int resultadoRM = removerLinhaProdutosDialog(stock);
                            if (resultadoRM == 1) {
                                strcpy(infoStr, "Linha de produtos removida com sucesso!");
                            } else if (resultadoRM == -1) {
                                strcpy(infoStr, "Operação cancelada!");
                            } else {
                                strcpy(infoStr, "Erro ao remover linha de produtos!");
                            }
                            break;
                        case 4: // editar linha de produtos
                            info = 1;
                            strcpy(infoStr, "A editar linha de produtos...");
                            int resultadoED = editarLinhaProdutosDialog(stock);
                            if (resultadoED == 1) {
                                strcpy(infoStr, "Linha de produtos editada com sucesso!");
                            } else if (resultadoED == -1) {
                                strcpy(infoStr, "Operação cancelada!");
                            } else {
                                strcpy(infoStr, "Erro ao editar linha de produtos!");
                            }
                            break;
                        case 5: // adicionar produto
                            info = 1;
                            strcpy(infoStr, "A adicionar produto...");
                            int resultadoAP = produtosDialog(stock, 1, 1);
                            if (resultadoAP == 1) {
                                info = 1;
                                strcpy(infoStr, "Produto adicionado com sucesso!");
                            } else if (resultadoAP == -1) {
                                strcpy(infoStr, "Operação cancelada!");
                            } else {
                                strcpy(infoStr, "Erro ao adicionar produto!");
                            }
                            break;
                        case 6: // remover produto
                            info = 1;
                            strcpy(infoStr, "A remover produto...");
                            int resultadoRP = produtosDialog(stock, 2, 2);
                            if (resultadoRP == 1) {
                                info = 1;
                                strcpy(infoStr, "Produto removido com sucesso!");
                            } else if (resultadoRP == -1) {
                                strcpy(infoStr, "Operação cancelada!");
                            } else {
                                strcpy(infoStr, "Erro ao remover produto!");
                            }
                            break;
                        case 7: // editar produto
                            info = 1;
                            strcpy(infoStr, "A editar produto...");
                            int resultadoEP = produtosDialog(stock, 3, 3);
                            if (resultadoEP == 1) {
                                strcpy(infoStr, "Produto editado com sucesso!");
                            } else if (resultadoEP == -1) {
                                strcpy(infoStr, "Operação cancelada!");
                            } else {
                                strcpy(infoStr, "Erro ao editar produto!");
                            }
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
                            if(stockLoadSaveDialog(stock, 1) == 1) {
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
                            if(stockLoadSaveDialog(stock, 2) == 1) {
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

