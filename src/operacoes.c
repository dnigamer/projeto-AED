//
// Created by Gonçalo Miranda on 10/05/2024.
//

#include "operacoes.h"
#include <string.h>
#include <stdlib.h>

// Cria uma nova instância de stock da loja
// Retorna o stock da loja
StockLoja* criarStockLoja(char* nome, int empty) {
    if (empty == 1) nome = "";
    StockLoja *stock = (StockLoja*) malloc(sizeof(StockLoja)); // alocar memória para o stock
    if (stock == NULL) return NULL; // se houver problema com a alocação de memória, retornar NULL
    strncpy(stock->nome, nome, 20); // copiar o nome para o stock
    stock->num_linhas = 0; // inicializar o número de linhas a 0
    stock->lista_linhas = NULL; // inicializar a lista de linhas a NULL
    return stock;
}

// Adiciona a funcionalidade de editar o nome da loja partindo do pointer e de uma string com o novo nome
int editarStockLoja(StockLoja* stockLoja, const char* nome) {
    if (stockLoja == NULL) return 1; // Stock não encontrado
    if (strlen(nome) == 0) return 2; // Nome vazio (não permitido)

    strncpy(stockLoja->nome, nome, 20); // só mudar o nome pois é o único campo editável
    return 0;
}

// Apaga todas as linhas de produtos do stock da loja
// Usada na tab de definições
int apagarLinhasProdutos(StockLoja* stockLoja) {
    if (stockLoja->lista_linhas == NULL) return 1; // se não houver linhas, não há nada para apagar

    ListaLinhaProdutos* current = stockLoja->lista_linhas; // pegar no primeiro nó
    while (current != NULL) { // enquanto houver nós
        ListaLinhaProdutos* next = (ListaLinhaProdutos *) current->prox_linha; // pegar no próximo nó
        if (current->linha->lista_produtos != NULL) { // se houver produtos
            ListaProdutos *currentProd = current->linha->lista_produtos; // pegar no primeiro produto da linha
            while (currentProd != NULL) { // enquanto houver produtos
                ListaProdutos *nextProd = (ListaProdutos *) currentProd->prox_produto; // pegar no próximo produto
                free(currentProd); // libertar o produto atual
                currentProd = nextProd; // avançar para o próximo produto
            }
        }
        current->linha->lista_produtos = NULL; // apontar a lista de produtos para NULL
        current->linha->num_produtos = 0; // definir o número de produtos a 0
        free(current); // libertar o nó atual
        current = next; // avançar para o próximo nó
    }
    stockLoja->lista_linhas = NULL; // apontar a lista de linhas para NULL
    stockLoja->num_linhas = 0; // definir o número de linhas a 0

    return 0; // sucesso
}

// Apaga todos os produtos das linhas de produtos do stock da loja
// Usada na tab de definições
int apagarProdutosLinhasStock(StockLoja* stockLoja) {
    if (stockLoja->lista_linhas == NULL) return 1; // se não houver linhas, não há nada para apagar

    ListaLinhaProdutos* current = stockLoja->lista_linhas;
    while (current != NULL) {
        ListaProdutos* currentProd = current->linha->lista_produtos;
        if (currentProd == NULL) {
            current = (ListaLinhaProdutos *) current->prox_linha;
            continue;
        }
        while (currentProd != NULL) {
            ListaProdutos* next = (ListaProdutos *) currentProd->prox_produto;
            free(currentProd);
            currentProd = next;
        }
        current->linha->lista_produtos = NULL;
        current->linha->num_produtos = 0;
        current = (ListaLinhaProdutos *) current->prox_linha;
    }

    return 0;
}


// Cria uma nova instância para a linha de produtos
// Retorna a linha de produtos
LinhaProdutos criarLinhaProdutos(char* nome) {
    LinhaProdutos linha; // criar uma nova instância de linha de produtos
    linha.linhaID = 0; // inicializar o ID da linha a 0
    strncpy(linha.nome, nome, 50); // copiar o nome para a linha
    linha.num_produtos = 0; // inicializar o número de produtos a 0
    linha.lista_produtos = NULL; // inicializar a lista de produtos a NULL
    return linha;
}

// Retorna a linha de produtos a partir do stock da loja e do código da linha
LinhaProdutos* obterLinhaProdutosPorID(StockLoja* stockLoja, int codigo) {
    if (stockLoja == NULL) return NULL; // se o stock não for encontrado, retornar NULL
    if (stockLoja->lista_linhas == NULL) return NULL; // se não houver linhas, retornar NULL

    ListaLinhaProdutos* current = stockLoja->lista_linhas; // pegar no primeiro nó
    while (current != NULL) { // enquanto houver nós
        if (current->linha->linhaID == codigo) { // se o ID da linha for igual ao código
            return (LinhaProdutos *) current->linha; // retornar a linha
        }
        current = (ListaLinhaProdutos *) current->prox_linha; // avançar para o próximo nó
    }

    return NULL; // se não for encontrada a linha, retornar NULL
}

// Retorna a linha de produtos a partir do stock da loja e do nome da linha
LinhaProdutos* obterLinhaProdutosPorNome(StockLoja* stockLoja, char* nome) {
    if (stockLoja == NULL) return NULL; // se o stock não for encontrado, retornar NULL
    if (stockLoja->lista_linhas == NULL) return NULL; // se não houver linhas, retornar NULL
    if (strlen(nome) == 0) return NULL; // se o nome for vazio, retornar NULL

    ListaLinhaProdutos* current = stockLoja->lista_linhas; // pegar no primeiro nó
    while (current != NULL) { // enquanto houver nós
        if (strcmp(current->linha->nome, nome) == 0) { // se o nome da linha for igual ao nome
            return (LinhaProdutos *) current->linha; // retornar a linha
        }
        current = (ListaLinhaProdutos *) current->prox_linha; // avançar para o próximo nó
    }

    return NULL; // se não for encontrada a linha, retornar NULL
}

// Criar linha de produtos dentro do stock da loja
// Usa stock da loja como pointer e usa uma instância de LinhaProdutos a incluir no stock
int adicionarLinhaProdutos(StockLoja* stockLoja, LinhaProdutos linha) {
    if (stockLoja == NULL) return -1; // stock não encontrado
    if (strlen(linha.nome) == 0) return -1; // nome vazio
    if (obterLinhaProdutosPorNome(stockLoja, linha.nome) != NULL) return 2; // encontrada linha com mesmo nome

    ListaLinhaProdutos* novo_no = (ListaLinhaProdutos*) malloc(sizeof(ListaLinhaProdutos));
    if (novo_no == NULL) return 1; // problema com alocação de memória

    if (stockLoja->lista_linhas == NULL) {
        linha.linhaID = 1; // ID da linha é 1 se não houver linhas
    } else {
        linha.linhaID = stockLoja->lista_linhas->linha->linhaID + 1; // ID da linha é o ID da última linha + 1
    }

    novo_no->linha = (LinhaProdutos *) malloc(sizeof(LinhaProdutos));
    if (novo_no->linha == NULL) return 1; // problema com alocação de memória

    *novo_no->linha = linha; // copiar a linha de produtos para o novo nó
    novo_no->prox_linha = (struct ListaLinhaProdutos *) stockLoja->lista_linhas; // apontar para a lista de linhas atual
    stockLoja->lista_linhas = novo_no; // apontar para o novo nó

    stockLoja->num_linhas++; // incrementar o número de linhas
    return 0; // sucesso
}

// Remove uma linha de produtos do stock da loja
// Utiliza o stock da loja como pointer e o código da linha a remover
int removerLinhaProdutos(StockLoja* stockLoja, unsigned int codigoLinha) {
    if (stockLoja->lista_linhas == NULL) return 2;  // stock sem linhas
    if (codigoLinha == 0) return 1; // código da linha a remover é 0

    ListaLinhaProdutos* temp = stockLoja->lista_linhas; // pegar no primeiro no
    ListaLinhaProdutos* prev = NULL; // nó anterior

    // se a linha a remover for a primeira
    if (temp != NULL && temp->linha->linhaID == codigoLinha) { // se o ID da linha for igual ao código
        stockLoja->lista_linhas = (ListaLinhaProdutos *) temp->prox_linha; // apontar para o próximo nó
        stockLoja->num_linhas--; // decrementar o número de linhas
        free(temp); // libertar o nó
        return 0;
    }

    // se a linha a remover não for a primeira
    while (temp != NULL && temp->linha->linhaID != codigoLinha) { // enquanto houver nós e o ID da linha for diferente do código
        prev = temp; // guardar o nó anterior
        temp = (ListaLinhaProdutos *) temp->prox_linha; // avançar para o próximo nó
    }
    if (temp == NULL) return 2; // se não for encontrada a linha, retornar 2

    if (prev != NULL)
        prev->prox_linha = temp->prox_linha; // apontar o nó anterior para o próximo nó
    stockLoja->num_linhas--; // decrementar o número de linhas

    free(temp); // libertar o nó
    return 0; // sucesso
}

// Atualiza uma linha de produtos no stock da loja
// Utiliza o stock da loja como pointer e a linha de produtos a atualizar como pointer
int atualizarLinhaProdutos(StockLoja* stockLoja, LinhaProdutos* linha) {
    if (stockLoja == NULL || linha == NULL) return 1; // Stock ou linha de produtos não encontrada
    if (strlen(linha->nome) == 0) return -1; // Nome da linha vazio

    ListaLinhaProdutos* current = stockLoja->lista_linhas; // pegar no primeiro nó
    while (current != NULL) { // enquanto houver nós
        if (current->linha != NULL && current->linha->linhaID == linha->linhaID) { // se o ID da linha for igual ao ID da linha a atualizar
            strncpy(current->linha->nome, linha->nome, sizeof(current->linha->nome) - 1); // copiar o nome da linha
            current->linha->nome[sizeof(current->linha->nome) - 1] = '\0'; // terminar a string
            if (obterLinhaProdutosPorNome(stockLoja, current->linha->nome) == NULL) return 2; // nome nao foi atualizado corretamente
            return 0; // Linha de produtos atualizada
        }
        current = current->prox_linha; // avançar para o próximo nó se não for a linha a atualizar
    }

    return 1; // Linha de produtos com linhaID não encontrada
}

// Retorna a lista de produtos a partir do stock da loja e do código da linha
// Não é usada
ListaProdutos* obterListaProdutosPorIDLinha(StockLoja* stock, int codLinha) {
    ListaLinhaProdutos* current = stock->lista_linhas;
    while (current != NULL) {
        if (current->linha->linhaID == codLinha) {
            return current->linha->lista_produtos;
        }
        current = (ListaLinhaProdutos *) current->prox_linha;
    }
    return NULL;
}

// Retorna a lista de produtos a partir do stock da loja e do nome da linha
// Não é usada
ListaProdutos * obterListaProdutosPorNomeLinha(StockLoja* stock, char* nomeLinha) {
    ListaLinhaProdutos* current = stock->lista_linhas;
    while (current != NULL) {
        if (strcmp(current->linha->nome, nomeLinha) == 0) {
            return current->linha->lista_produtos;
        }
        current = (ListaLinhaProdutos *) current->prox_linha;
    }
    return NULL;
}

// Retorna uma lista de todos os produtos com o nome do produto fornecido
// Usada no onProdutosLVClicked
ListaProdutos* procurarStockPorNomeProduto(StockLoja* stock, char* nomeItem) {
    if (stock == NULL) return NULL; // se o stock não for encontrado, retornar NULL
    if (stock->lista_linhas == NULL) return NULL; // se não houver linhas, retornar NULL
    if (strlen(nomeItem) == 0) return NULL; // se o nome for vazio, retornar NULL
    if (stock->num_linhas == 0) return NULL; // se não houver linhas, retornar NULL

    ListaLinhaProdutos* currentLinha = stock->lista_linhas; // pegar na primeira linha
    ListaProdutos* produtos = NULL; // inicializar a lista de produtos a NULL

    while (currentLinha != NULL) { // enquanto houver linhas
        ListaProdutos* currentProduto = currentLinha->linha->lista_produtos; // pegar no primeiro produto da linha
        if (currentProduto == NULL) { // se não houver produtos
            currentLinha = currentLinha->prox_linha; // avançar para a próxima linha
            continue;
        }
        while (currentProduto != NULL) { // enquanto houver produtos
            if (strcmp(currentProduto->produto->nome, nomeItem) == 0) { // se o nome do produto for igual ao nome fornecido
                ListaProdutos* newProduto = (ListaProdutos*) malloc(sizeof(ListaProdutos)); // alocar memória para o novo produto
                if (newProduto == NULL) return NULL; // se houver problema com a alocação de memória, retornar NULL
                newProduto->produto = currentProduto->produto; // copiar o produto para a lista a retornar
                newProduto->prox_produto = produtos; // apontar para a lista de produtos atual
                produtos = newProduto; // apontar para o novo produto
            }
            currentProduto = currentProduto->prox_produto; // avançar para o próximo produto
        }
        currentLinha = currentLinha->prox_linha; // avançar para a próxima linha
    }

    return produtos; // retornar a lista de produtos
}

ListaProdutos* obterListaProdutosPorItemLinha(LinhaProdutos* linha, char* nomeItem) {
    if (linha == NULL) return NULL; // se a linha não for encontrada, retornar NULL
    if (linha->lista_produtos == NULL) return NULL; // se não houver produtos, retornar NULL
    if (strlen(nomeItem) == 0) return NULL; // se o nome for vazio, retornar NULL

    ListaProdutos* current = linha->lista_produtos; // pegar no primeiro produto
    ListaProdutos* produtos = NULL; // inicializar a lista de produtos a NULL

    if (current == NULL) return NULL; // se não houver produtos na linha, retornar NULL

    while (current != NULL) { // enquanto houver produtos
        if (strcmp(current->produto->nome, nomeItem) == 0) { // se o nome do item for igual ao nome fornecido
            ListaProdutos* newProduto = (ListaProdutos*) malloc(sizeof(ListaProdutos)); // alocar memória para o novo produto
            if (newProduto == NULL) return NULL; // se houver problema com a alocação de memória, retornar NULL
            newProduto->produto = current->produto; // copiar o produto para a lista a retornar
            newProduto->prox_produto = produtos; // apontar para a lista de produtos atual
            produtos = newProduto; // apontar para o novo produto
        }
        current = current->prox_produto; // avançar para o próximo produto
    }

    return produtos; // retornar a lista de produtos
}


// Cria um novo produto usando os parâmetros fornecidos
// Retorna instância de produto
Produto criarProduto(char* nome, char* item, char* modelo, unsigned int quantidade, double preco, ListaParamAdicionalProduto* parametros) {
    Produto p;
    p.linhaID = 0;
    p.produtoID = 0;
    strncpy(p.nome, nome, 50);
    strncpy(p.item, item, 50);
    strncpy(p.modelo, modelo, 50);
    p.quantidade = quantidade;
    p.preco = preco;
    p.num_parametros = getNumeroParametrosAdicionais(parametros);
    p.parametros = parametros;
    return p; // { 0, 0, 0, nome, item, modelo, quantidade, preco, parametros };
}

// Retorna um pointer de produto de uma linha de produtos com base no ID do produto
// Usada na pesquisa de produtos
Produto* obterProdutoPorID(LinhaProdutos* linha, unsigned int IDProduto) {
    if (linha == NULL) return NULL; // se a linha não for encontrada, retornar NULL
    if (linha->lista_produtos == NULL) return NULL; // se não houver produtos, retornar NULL
    if (IDProduto == 0) return NULL; // se o ID for 0, retornar NULL

    ListaProdutos* current = linha->lista_produtos; // pegar no primeiro produto
    if (current == NULL) return NULL; // se não houver produtos na linha, retornar NULL
    while (current != NULL) { // enquanto houver produtos
        if (current->produto->produtoID == IDProduto) { // se o ID do produto for igual ao ID fornecido
            return current->produto; // retornar o produto
        }
        current = (ListaProdutos *) current->prox_produto; // avançar para o próximo produto
    }

    return NULL; // se não for encontrado o produto, retornar NULL
}

// Retorna um pointer produto com base no nome e modelo passados
Produto* obterProdutoPorNome(LinhaProdutos* linha, char* nome, char* modelo) {
    if (linha == NULL) return NULL; // se a linha não for encontrada, retornar NULL
    if (linha->lista_produtos == NULL) return NULL; // se não houver produtos, retornar NULL
    if (strlen(nome) == 0) return NULL; // se o nome for vazio, retornar NULL
    if (strlen(modelo) == 0) return NULL; // se o modelo for vazio, retornar NULL

    ListaProdutos* current = linha->lista_produtos; // pegar no primeiro produto
    while (current != NULL) { // enquanto houver produtos
        if (strcmp(current->produto->nome, nome) == 0 && strcmp(current->produto->modelo, modelo) == 0) {
            // se o nome e modelo do produto for igual ao nome fornecido
            return current->produto; // retornar o produto
        }
        current = (ListaProdutos *) current->prox_produto; // avançar para o próximo produto
    }

    return NULL; // se não for encontrado o produto, retornar NULL
}

// Adiciona um produto a uma linha de produtos
// Utiliza a linha de produtos como pointer e o produto a adicionar como pointer
int adicionarProduto(LinhaProdutos* linha, Produto* produto) {
    if (linha == NULL) return 2; // se a linha não for encontrada, retornar 1
    if (produto == NULL) return 2; // se o produto não for encontrado, retornar 1
    if (strlen(produto->nome) == 0) return 4; // se o nome do produto for vazio, retornar 4
    if (strlen(produto->modelo) == 0) return 5; // se o modelo do produto for vazio, retornar 5

    Produto* p = obterProdutoPorNome(linha, produto->nome, produto->modelo);
    if (p != NULL) return 3; // produto já existe na linha

    ListaProdutos* novo_no = (ListaProdutos*) malloc(sizeof(ListaProdutos));
    if (novo_no == NULL) return 1; // se houver problema com a alocação de memória, retornar 1

    Produto* novo_produto = (Produto*) malloc(sizeof(Produto));
    if (novo_produto == NULL) return 1; // se houver problema com a alocação de memória, retornar 1

    *novo_produto = *produto; // copiar o produto para um novo produto
    if (linha->lista_produtos == NULL) {
        novo_produto->produtoID = 1; // ID do produto é 1 se não houver produtos
    } else {
        novo_produto->produtoID = linha->lista_produtos->produto->produtoID + 1; // ID do produto é o ID do último produto + 1
    }
    novo_produto->linhaID = linha->linhaID; // ID da linha no produto é o ID da linha
    novo_produto->num_parametros = getNumeroParametrosAdicionais(produto->parametros); // número de parâmetros adicionais
    novo_produto->parametros = produto->parametros; // parâmetros adicionais

    novo_no->produto = novo_produto; // apontar para o novo produto
    novo_no->prox_produto = NULL; // apontar para NULL se for o primeiro produto
    if (linha->lista_produtos != NULL) { // se houver produtos
        novo_no->prox_produto = (ListaProdutos *) linha->lista_produtos; // apontar para a lista de produtos atual
    }

    linha->lista_produtos = novo_no; // apontar para o no do novo produto
    linha->num_produtos++; // incrementar o número de produtos

    return 0; // sucesso
}

// Remove um produto de uma linha de produtos
int removerProduto(LinhaProdutos* linha, unsigned int IDproduto) {
    if (linha == NULL) return 1;
    if (linha->lista_produtos == NULL) return 1;
    if (IDproduto == 0) return 1;
    if (linha->num_produtos == 0) return 1;

    ListaProdutos* temp = linha->lista_produtos; // pegar no primeiro produto
    ListaProdutos* prev = NULL; // nó anterior

    while (temp != NULL && temp->produto->produtoID != IDproduto) { // enquanto houver produtos e o ID do produto for diferente do ID fornecido
        prev = temp; // guardar o nó anterior
        temp = temp->prox_produto; // avançar para o próximo produto
    }
    if (temp == NULL) return 1; // se não for encontrado o produto, retornar 0

    if (prev == NULL) { // se for o primeiro produto
        linha->lista_produtos = temp->prox_produto; // apontar para o próximo produto
    } else {
        prev->prox_produto = temp->prox_produto; // apontar para o próximo produto se não for o primeiro
    }

    linha->num_produtos--; // decrementar o número de produtos

    free(temp); // libertar o produto
    return 0;
}

// Atualiza um produto numa linha de produtos com base no ID do produto fornecido
int atualizarProduto(LinhaProdutos* linha, Produto* produto) {
    if (linha == NULL) return 1;
    if (linha->lista_produtos == NULL) return 1;
    if (produto == NULL) return 1;

    ListaProdutos* current = linha->lista_produtos; // pegar no primeiro produto
    while (current != NULL) {
        if (current->produto->produtoID == produto->produtoID) { // se o ID do produto for igual ao ID fornecido
            strncpy(current->produto->nome, produto->nome, sizeof(current->produto->nome));
            strncpy(current->produto->item, produto->item, sizeof(current->produto->item));
            strncpy(current->produto->modelo, produto->modelo, sizeof(current->produto->modelo));
            current->produto->quantidade = produto->quantidade;
            current->produto->preco = produto->preco;
            current->produto->num_parametros = getNumeroParametrosAdicionais(produto->parametros);
            current->produto->parametros = produto->parametros;
            return 0; // Produto atualizado
        }
        current = current->prox_produto;
    }

    return 1; // Produto não encontrado
}


// Cria uma nova instância de lista de parâmetros adicionais de produtos
// Retorna a lista de parâmetros adicionais de produtos
ListaParamAdicionalProduto* criarListaParamAdicionaisProduto() {
    return NULL;
}

// Cria uma nova instância de parâmetro adicional de produto
ParamAdicionalProduto* criarParametroAdicionalProduto(char* nome, char* valor) {
    if (strlen(nome) == 0) return NULL;
    if (strlen(valor) == 0) return NULL;

    ParamAdicionalProduto* parametro = (ParamAdicionalProduto*) malloc(sizeof(ParamAdicionalProduto));
    if (parametro == NULL) return NULL;
    parametro->id = 0;
    strncpy(parametro->nome, nome, MAX_CHAR);
    strncpy(parametro->valor, valor, MAX_CHAR);

    return parametro;
}

// Adiciona um parâmetro adicional a uma lista de parâmetros adicionais de produtos
// Utiliza a lista de parâmetros adicionais de produtos como pointer e o parâmetro a adicionar como pointer
int adicionarParametroAdicionalProduto(Produto* produto, ParamAdicionalProduto* parametro) {
    if (produto == NULL) return 1; // se o produto não for encontrado, retornar 1
    if (parametro == NULL) return 1; // se o parâmetro não for encontrado, retornar 1

    ListaParamAdicionalProduto* novo_no = (ListaParamAdicionalProduto*) malloc(sizeof(ListaParamAdicionalProduto));
    if (novo_no == NULL) return 1; // se houver problema com a alocação de memória, retornar 1

    if (produto->parametros == NULL) {
        parametro->id = 1; // ID do parâmetro é 1 se não houver parâmetros
    } else {
        parametro->id = produto->parametros->parametro->id + 1; // ID do parâmetro é o ID do último parâmetro + 1
    }

    novo_no->parametro = parametro; // apontar para o novo parâmetro
    novo_no->prox_parametro = produto->parametros; // apontar para a lista de parâmetros atual
    produto->parametros = novo_no; // apontar para o novo parâmetro
    produto->num_parametros++; // incrementar o número de parâmetros

    return 0; // sucesso
}

// Adiciona uma lista de parâmetros adicionais a um produto
int adicionarListaParametroAdicionalProduto(Produto* produto, ListaParamAdicionalProduto* parametros) {
    if (produto == NULL) return 1; // se o produto não for encontrado, retornar 1
    if (parametros == NULL) return 1; // se a lista de parâmetros não for encontrada, retornar 1

    ListaParamAdicionalProduto* current = parametros; // pegar no primeiro parâmetro
    while (current != NULL) { // enquanto houver parâmetros
        adicionarParametroAdicionalProduto(produto, current->parametro); // adicionar o parâmetro ao produto
        current = current->prox_parametro; // avançar para o próximo parâmetro
    }

    return 0; // sucesso
}


// Adiciona um parâmetro adicional a uma lista de parâmetros adicionais de produtos
// Utiliza a lista de parâmetros adicionais do produto como pointer e o parâmetro a adicionar como pointer
int adicionarParametroAdicionalLista(ListaParamAdicionalProduto** lista, ParamAdicionalProduto* parametro) {
    if (parametro == NULL) return 1; // se o parâmetro não for encontrado, retornar 1
    if (strlen(parametro->nome) == 0) return 1; // se o nome do parâmetro for vazio, retornar 1
    if (strlen(parametro->valor) == 0) return 1; // se o valor do parâmetro for vazio, retornar 1

    ListaParamAdicionalProduto* novo_no = (ListaParamAdicionalProduto*) malloc(sizeof(ListaParamAdicionalProduto));
    if (novo_no == NULL) return 1; // se houver problema com a alocação de memória, retornar 1

    ParamAdicionalProduto* novo_parametro = (ParamAdicionalProduto*) malloc(sizeof(ParamAdicionalProduto));
    if (novo_parametro == NULL) return 1; // se houver problema com a alocação de memória, retornar 1

    *novo_parametro = *parametro; // copiar o parâmetro para o novo parâmetro
    if (*lista == NULL) {
        novo_parametro->id = 1; // ID do parâmetro é 1 se não houver parâmetros
    } else {
        novo_parametro->id = (*lista)->parametro->id + 1; // ID do parâmetro é o ID do último parâmetro + 1
    }

    novo_no->parametro = novo_parametro; // apontar para o novo parâmetro
    novo_no->prox_parametro = *lista; // apontar para a lista de parâmetros atual
    *lista = novo_no; // apontar lista para o novo parâmetro

    return 0; // sucesso
}

// Obtem um parâmetro adicional de uma lista de parâmetros adicionais de produtos com base no ID do parâmetro
ParamAdicionalProduto* obterParametroAdicionalPorID(Produto* produto, unsigned int codigoParametro) {
    if (produto == NULL) return NULL; // se o produto não for encontrado, retornar NULL
    if (produto->parametros == NULL || produto->num_parametros == 0) return NULL; // se não houver parâmetros, retornar NULL
    if (codigoParametro == 0) return NULL; // se o ID do parâmetro for 0, retornar NULL

    ListaParamAdicionalProduto* current = produto->parametros; // pegar no primeiro parâmetro
    while (current != NULL) { // enquanto houver parâmetros
        if (current->parametro->id == codigoParametro) { // se o ID do parâmetro for igual ao ID fornecido
            return current->parametro; // retornar o parâmetro
        }
        current = current->prox_parametro; // avançar para o próximo parâmetro se não
    }

    return NULL; // se não for encontrado o parâmetro, retornar NULL
}

// Obtem um parâmetro adicional de uma lista de parâmetros adicionais de produtos com base no nome do parâmetro
ParamAdicionalProduto* obterParametroAdicionalPorNome(Produto* produto, char* nomeParametro) {
    if (produto == NULL) return NULL; // se o produto não for encontrado, retornar NULL
    if (produto->parametros == NULL || produto->num_parametros == 0) return NULL; // se não houver parâmetros, retornar NULL
    if (strlen(nomeParametro) == 0) return NULL; // se o nome do parâmetro for vazio, retornar NULL

    ListaParamAdicionalProduto* current = produto->parametros; // pegar no primeiro parâmetro
    while (current != NULL) { // enquanto houver parâmetros
        if (strcmp(current->parametro->nome, nomeParametro) == 0) { // se o nome do parâmetro for igual ao nome fornecido
            return current->parametro; // retornar o parâmetro
        }
        current = current->prox_parametro; // avançar para o próximo parâmetro se não
    }

    return NULL; // se não for encontrado o parâmetro, retornar NULL
}

// Retorna o número de parâmetros adicionais numa lista de parâmetros adicionais de produtos
unsigned int getNumeroParametrosAdicionais(ListaParamAdicionalProduto* lista) {
    if (lista == NULL) return 0; // se a lista não for encontrada, retornar 0

    unsigned int count = 0; // inicializar a contagem a 0
    ListaParamAdicionalProduto* current = lista; // pegar no primeiro parâmetro
    while (current != NULL) { // enquanto houver parâmetros
        count++; // incrementar a contagem
        current = current->prox_parametro; // avançar para o próximo parâmetro
    }

    return count; // retornar a contagem
}

////
// Adicional
////

// Retorna o número de linhas de produtos no stock da loja
unsigned int getNumeroLinhasProdutos(StockLoja* stockLoja) {
    if (stockLoja == NULL) return 0; // se o stock não for encontrado, retornar 0
    if (stockLoja->lista_linhas == NULL) return 0; // se não houver linhas, retornar 0
    return stockLoja->num_linhas; // retornar o número de linhas
}

// Retorna o número de produtos numa linha de produtos vendo quantos produtos existem em cada lista
unsigned int getNumeroProdutosLinha(LinhaProdutos* linha) {
    if (linha == NULL) return 0; // se a linha não for encontrada, retornar 0
    if (linha->lista_produtos == NULL || linha->num_produtos == 0) return 0; // se a lista de produtos for vazia, retornar 0

    unsigned int count = 0; // inicializar a contagem a 0
    ListaProdutos* current = linha->lista_produtos; // pegar no primeiro produto
    while (current != NULL) { // enquanto houver produtos
        count++; // incrementar a contagem
        current = (ListaProdutos *) current->prox_produto; // avançar para o próximo produto
    }

    return count; // retornar a contagem
}

// Retorna o número de produtos numa dada lista de produtos
unsigned int getNumeroProdutosLista(ListaProdutos* lista) {
    if (lista == NULL) return 0; // se a lista não for encontrada, retornar 0

    unsigned int count = 0; // inicializar a contagem a 0
    ListaProdutos* current = lista; // pegar no primeiro produto
    while (current != NULL) { // enquanto houver produtos
        count++; // incrementar a contagem
        current = (ListaProdutos *) current->prox_produto; // avançar para o próximo produto
    }

    return count; // retornar a contagem
}

// Retorna o número de produtos total no stock da loja
unsigned int getNumeroProdutosStock(StockLoja* stock) {
    if (stock == NULL) return 0; // se o stock não for encontrado, retornar 0
    if (getNumeroLinhasProdutos(stock) == 0) return 0; // se não houver linhas, retornar 0

    unsigned int count = 0;
    ListaLinhaProdutos* current = stock->lista_linhas;
    while (current != NULL) {
        count += getNumeroProdutosLinha(current->linha);
        current = (ListaLinhaProdutos *) current->prox_linha;
    }
    return count;
}

// Nó de tipo para contar tipos de produtos (Lista ligada)
typedef struct TipoNode {
    char tipo[MAX_CHAR];
    struct TipoNode* next;
} TipoNode;

unsigned int getNumeroTipoProdutosStock(StockLoja* stock) {
    unsigned int contagem = 0; // Inicializar a contagem a 0
    TipoNode* tiposContados = NULL; // Inicializar a lista de tipos contados a NULL

    ListaLinhaProdutos* linhaAtual = stock->lista_linhas; // Pegar na primeira linha
    while (linhaAtual != NULL) { // Enquanto houver linhas
        ListaProdutos* produtoAtual = linhaAtual->linha->lista_produtos; // Pegar no primeiro produto
        while (produtoAtual != NULL) { // Enquanto houver produtos
            TipoNode* tipoAtual = tiposContados; // Pegar no primeiro tipo
            while (tipoAtual != NULL) { // Enquanto houver tipos
                if (strcmp(tipoAtual->tipo, produtoAtual->produto->nome) == 0) { // Se o tipo for igual ao tipo do produto
                    break; // Parar
                }
                tipoAtual = tipoAtual->next; // Avançar para o próximo tipo
            }
            if (tipoAtual == NULL) { // Se o tipo não for encontrado/ainda não tiver sido encontrado
                TipoNode* novoTipo = (TipoNode*) malloc(sizeof(TipoNode)); // Alocar memória para o novo tipo
                if (novoTipo == NULL) { // Se houver problema com a alocação de memória
                    TipoNode* tipoAtualNodes = tiposContados; // Pegar no primeiro tipo
                    while (tipoAtualNodes != NULL) { // Enquanto houver tipos
                        TipoNode* nextType = tipoAtualNodes->next; // Pegar no próximo tipo
                        free(tipoAtualNodes); // Libertar o tipo
                        tipoAtualNodes = nextType; // Avançar para o próximo tipo
                    }
                    return contagem; // Retornar a contagem sem tipos todos
                }
                strncpy(novoTipo->tipo, produtoAtual->produto->nome, MAX_CHAR); // Copiar o atual tipo para um novo tipo
                novoTipo->next = tiposContados; // Apontar para a lista de tipos contados
                tiposContados = novoTipo; // Apontar para o novo tipo
                contagem++; // Incrementar a contagem
            }
            produtoAtual = produtoAtual->prox_produto; // Avançar para o próximo produto
        }
        linhaAtual = linhaAtual->prox_linha; // Avançar para a próxima linha
    }

    return contagem; // Retornar a contagem
}

////
// Ordenação
////

// Dá merge a duas listas de produtos (usado por mergeSort para getNumeroTipoProdutosStock)
ListaProdutos* sortedMerge(ListaProdutos* a, ListaProdutos* b) {
    ListaProdutos dummy; // Nó temporário
    ListaProdutos* tail = &dummy; // Ponteiro para o final da lista mesclada
    dummy.prox_produto = NULL;

    // Enquanto houver elementos em ambas as listas
    while (a != NULL && b != NULL) {
        if (strcmp(a->produto->nome, b->produto->nome) <= 0) {
            tail->prox_produto = a;
            a = a->prox_produto;
        } else {
            tail->prox_produto = b;
            b = b->prox_produto;
        }
        tail = tail->prox_produto;
    }

    if (a != NULL) {
        tail->prox_produto = a;
    } else {
        tail->prox_produto = b;
    }

    return dummy.prox_produto;
}

// Função para dividir a lista de produtos em duas e partir dai
void split(ListaProdutos* fonte, ListaProdutos** frontRef, ListaProdutos** backRef) {
    ListaProdutos* rapido;
    ListaProdutos* lento;
    lento = fonte;
    rapido = fonte->prox_produto;

    while (rapido != NULL) {
        rapido = rapido->prox_produto;
        if (rapido != NULL) {
            lento = lento->prox_produto;
            rapido = rapido->prox_produto;
        }
    }

    *frontRef = fonte;
    *backRef = lento->prox_produto;
    lento->prox_produto = NULL;
}

// Função para ordenar a lista de produtos usando o merge sort
void mergeSort(ListaProdutos** headRef) {
    if (*headRef == NULL || (*headRef)->prox_produto == NULL) {
        return;
    }

    ListaProdutos* head = *headRef;
    ListaProdutos* a;
    ListaProdutos* b;
    ListaProdutos* sorted = NULL;

    split(head, &a, &b);

    ListaProdutos* stack[100];
    int top = -1;
    if (a != NULL) {
        stack[++top] = a;
    }
    if (b != NULL) {
        stack[++top] = b;
    }

    while (top >= 0) {
        ListaProdutos* current = stack[top--];
        if (current->prox_produto == NULL) {
            sorted = sortedMerge(sorted, current);
        } else {
            split(current, &a, &b);
            stack[++top] = a;
            stack[++top] = b;
        }
    }

    *headRef = sorted;
}


////
// Pesquisa
////

// procura por nome de produto
//// JÁ DEFINIDA EM procurarStockPorNomeProduto
// procura por modelo de produto (apresenta Lista de produtos)
ListaProdutos* procurarStockPorModeloProduto(StockLoja* stock, char* modelo) {
    if (stock == NULL) return NULL; // se o stock não for encontrado, retornar NULL
    if (stock->lista_linhas == NULL) return NULL; // se não houver linhas, retornar NULL
    if (strlen(modelo) == 0) return NULL; // se o modelo for vazio, retornar NULL
    if (stock->num_linhas == 0) return NULL; // se não houver linhas, retornar NULL

    ListaLinhaProdutos* currentLinha = stock->lista_linhas; // pegar na primeira linha
    ListaProdutos* produtos = NULL; // inicializar a lista de produtos a NULL

    while (currentLinha != NULL) { // enquanto houver linhas
        ListaProdutos* currentProduto = currentLinha->linha->lista_produtos; // pegar no primeiro produto da linha
        if (currentProduto == NULL) { // se não houver produtos
            currentLinha = currentLinha->prox_linha; // avançar para a próxima linha
            continue;
        }
        while (currentProduto != NULL) { // enquanto houver produtos
            if (strcmp(currentProduto->produto->modelo, modelo) == 0) { // se o modelo do produto for igual ao modelo fornecido
                ListaProdutos* newProduto = (ListaProdutos*) malloc(sizeof(ListaProdutos)); // alocar memória para o novo produto
                if (newProduto == NULL) return NULL; // se houver problema com a alocação de memória, retornar NULL
                newProduto->produto = currentProduto->produto; // copiar o produto para a lista a retornar
                newProduto->prox_produto = produtos; // apontar para a lista de produtos atual
                produtos = newProduto; // apontar para o novo produto
            }
            currentProduto = currentProduto->prox_produto; // avançar para o próximo produto
        }
        currentLinha = currentLinha->prox_linha; // avançar para a próxima linha
    }

    return produtos; // retornar a lista de produtos
}

// procura por categoria de produto (apresenta Lista de produtos)
ListaProdutos* procurarStockPorItemProduto(StockLoja* stock, char* item) {
    if (stock == NULL) return NULL; // se o stock não for encontrado, retornar NULL
    if (stock->lista_linhas == NULL) return NULL; // se não houver linhas, retornar NULL
    if (strlen(item) == 0) return NULL; // se o item for vazio, retornar NULL
    if (stock->num_linhas == 0) return NULL; // se não houver linhas, retornar NULL

    ListaLinhaProdutos* currentLinha = stock->lista_linhas; // pegar na primeira linha
    ListaProdutos* produtos = NULL; // inicializar a lista de produtos a NULL

    while (currentLinha != NULL) { // enquanto houver linhas
        ListaProdutos* currentProduto = currentLinha->linha->lista_produtos; // pegar no primeiro produto da linha
        if (currentProduto == NULL) { // se não houver produtos
            currentLinha = currentLinha->prox_linha; // avançar para a próxima linha
            continue;
        }
        while (currentProduto != NULL) { // enquanto houver produtos
            if (strcmp(currentProduto->produto->item, item) == 0) { // se o item do produto for igual ao item fornecido
                ListaProdutos* newProduto = (ListaProdutos*) malloc(sizeof(ListaProdutos)); // alocar memória para o novo produto
                if (newProduto == NULL) return NULL; // se houver problema com a alocação de memória, retornar NULL
                newProduto->produto = currentProduto->produto; // copiar o produto para a lista a retornar
                newProduto->prox_produto = produtos; // apontar para a lista de produtos atual
                produtos = newProduto; // apontar para o novo produto
            }
            currentProduto = currentProduto->prox_produto; // avançar para o próximo produto
        }
        currentLinha = currentLinha->prox_linha; // avançar para a próxima linha
    }

    return produtos; // retornar a lista de produtos
}

// procura por parametro de produto (apresenta Lista de produtos)
ListaProdutos* procurarStockPorParametroProduto(StockLoja* stock, char* parametro) {
    if (stock == NULL) return NULL; // se o stock não for encontrado, retornar NULL
    if (stock->lista_linhas == NULL) return NULL; // se não houver linhas, retornar NULL
    if (strlen(parametro) == 0) return NULL; // se o parametro for vazio, retornar NULL
    if (stock->num_linhas == 0) return NULL; // se não houver linhas, retornar NULL

    ListaLinhaProdutos* currentLinha = stock->lista_linhas; // pegar na primeira linha
    ListaProdutos* produtos = NULL; // inicializar a lista de produtos a NULL

    while (currentLinha != NULL) { // enquanto houver linhas
        ListaProdutos *currentProduto = currentLinha->linha->lista_produtos; // pegar no primeiro produto da linha
        if (currentProduto == NULL) { // se não houver produtos
            currentLinha = currentLinha->prox_linha; // avançar para a próxima linha
            continue;
        }
        while (currentProduto != NULL) { // enquanto houver produtos
            ListaParamAdicionalProduto *currentParam = currentProduto->produto->parametros; // pegar no primeiro parâmetro
            while (currentParam != NULL) { // enquanto houver parâmetros
                if (strcmp(currentParam->parametro->nome, parametro) ==
                    0) { // se o nome do parâmetro for igual ao parâmetro fornecido
                    ListaProdutos *newProduto = (ListaProdutos *) malloc(sizeof(ListaProdutos)); // alocar memória para o novo produto
                    if (newProduto == NULL) return NULL; // se houver problema com a alocação de memória, retornar NULL
                    newProduto->produto = currentProduto->produto; // copiar o produto para a lista a retornar
                    newProduto->prox_produto = produtos; // apontar para a lista de produtos atual
                    produtos = newProduto; // apontar para o novo produto
                    break; // parar
                }
                currentParam = currentParam->prox_parametro; // avançar para o próximo parâmetro
            }
            currentProduto = currentProduto->prox_produto; // avançar para o próximo
        }
        currentLinha = currentLinha->prox_linha; // avançar para a próxima linha
    }

    return produtos; // retornar a lista de produtos
}

