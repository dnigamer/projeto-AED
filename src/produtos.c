#include "produtos.h"
#include <string.h>
#include <stdlib.h>

// Cria uma nova instância para a linha de produtos
// Retorna a linha de produtos
LinhaProdutos criarLinhaProdutos(int codigo, char nome[]) {
    LinhaProdutos lp;                                       // Cria uma nova instância para a linha de produtos
    lp.codigo = codigo;                                     // Atribui o código à linha de produtos
    strncpy(lp.nome, nome, 50);                             // Atribui o nome à linha de produtos
    lp.top = NULL;                                          // Inicializa o topo da stack como NULL
    return lp;                                              // Retorna a linha de produtos
}

LinhaProdutos obterLinhaProdutos(StockLoja* stockLoja, int codigo) {
    for (int i = 0; i < stockLoja->num_linhas; i++) {
        if (stockLoja->linhas[i].codigo == codigo) {
            return stockLoja->linhas[i];
        }
    }
    return criarLinhaProdutos(0, "");
}


// Criar linha de produtos dentro do stock da loja
// Usa stock da loja como pointer e usa linha de produtos a incluir no stock como pointer
int adicionarLinhaProdutos(StockLoja* stockLoja, LinhaProdutos* linha) {
    LinhaProdutos* novas_linhas = (LinhaProdutos*) malloc(sizeof(LinhaProdutos) * (stockLoja->num_linhas + 1)); // Aloca memória para um novo array de linhas
    if (novas_linhas == NULL) return 1;                    // Retorna 1 se houve um erro ao alocar memória
    for (int i = 0; i < stockLoja->num_linhas; i++) {
        novas_linhas[i] = stockLoja->linhas[i];            // Copia as linhas do array antigo para o novo array
    }
    novas_linhas[stockLoja->num_linhas] = *linha;          // Adiciona a nova linha ao novo array
    free(stockLoja->linhas);                               // Libera a memória do array antigo
    stockLoja->linhas = novas_linhas;                      // Atribui o novo array ao stock
    stockLoja->num_linhas++;                               // Incrementa o número de linhas
    return 0;
}

// Apaga uma linha de produtos dado um stock da loja
int apagaLinhaProdutos(StockLoja* stockLoja, LinhaProdutos* linha) {
    for (int i = 0; i < stockLoja->num_linhas; i++) {
        if (stockLoja->linhas[i].codigo == linha->codigo) {
            for (int j = i; j < stockLoja->num_linhas - 1; j++) {
                stockLoja->linhas[j] = stockLoja->linhas[j + 1];
            }
            stockLoja->num_linhas--;
            return 0;
        }
    }
    return 1;
}

// Cria um novo produto usando os parâmetros fornecidos
// Retorna um struct de produto
Produto criarProduto(int codigo, char* nome, char* marca, float peso, float preco, int quantidade) {
    Produto p;                                              // Cria uma nova instância para o produto
    p.codigo = codigo;                                      // Atribui o código ao produto
    strncpy(p.nome, nome, 50);                              // Atribui o nome ao produto
    strncpy(p.marca, marca, 50);                            // Atribui a marca ao produto
    p.peso = peso;                                          // Atribui o peso ao produto
    p.preco = preco;                                        // Atribui o preço ao produto
    p.quantidade = quantidade;                              // Atribui a quantidade ao produto
    return p;                                               // Retorna o produto
}

// Adiciona um produto a uma linha de produtos
// Retorna 0 se o produto foi adicionado com sucesso
// Retorna 1 se houve um erro ao alocar memória
int adicionarProduto(LinhaProdutos* linha, Produto produto) {  
    Node* novo_node = (Node*) malloc(sizeof(Node));    // Aloca memória para um novo nó
    if (novo_node == NULL) return 1;                        // Retorna 1 se houve um erro ao alocar memória
    novo_node->produto = produto;                           // Atribui o produto ao novo nó
    novo_node->next = linha->top;                           // O próximo nó do novo nó é o nó que estava no topo
    linha->top = novo_node;                                 // O novo nó é o novo topo
    return 0;
}

// Remove um produto de uma linha de produtos
// Retorna 0 se o produto foi removido com sucesso
// Retorna 1 se o produto não foi encontrado
int removerProduto(LinhaProdutos* linha, int codigo) {
    Node* current = linha->top;                             // Obtém o nó do topo
    Node* previous = NULL;                                  // Inicializa o nó anterior como NULL
    while (current != NULL) {                               // Enquanto o nó atual não for NULL
        if (current->produto.codigo == codigo) {            // Se o código do produto for igual ao código fornecido
            if (previous == NULL) {                         // Se o nó anterior for NULL
                linha->top = current->next;                 // O topo da stack é o próximo nó
            } else {
                previous->next = current->next;             // O próximo nó do nó anterior é o próximo nó do nó atual
            }
            free(current);                                  // Libera a memória do nó atual
            return 0;
        }
        previous = current;                                 // O nó anterior é o nó atual
        current = current->next;                            // O nó atual é o próximo nó
    }
    return 1;
}

// Retorna um produto de uma linha de produtos
// Retorna NULL se o produto não foi encontrado
// Retorna o apontador do produto se o produto foi encontrado
Produto* obterProduto(LinhaProdutos* linha, int codigo) {
    Node* current = linha->top;                             // Obtém o nó do topo
    if (current == NULL) return NULL;                       // Retorna NULL se o nó do topo for NULL
    while (current != NULL) {                               // Enquanto o nó atual não for NULL
        if (current->produto.codigo == codigo) {            // Se o código do produto for igual ao código fornecido
            return &current->produto;                       // Retorna o produto
        }
        current = current->next;                            // O próximo nó será o nó atual
    }
    return NULL;                                            // Retorna NULL se o produto não foi encontrado
}

// Atualiza um produto numa linha de produtos
// Retorna 0 se o produto foi atualizado com sucesso
// Retorna 1 se o produto não foi encontrado
int atualizarProduto(LinhaProdutos* linha, Produto Produto) {
    Node* current = linha->top;                             // Obtém o nó do topo
    if (current == NULL) return 1;                          // Retorna 1 se o nó do topo for NULL
    while (current != NULL) {                               // Enquanto o nó atual não for NULL
        if (current->produto.codigo == Produto.codigo) {    // Se o código do produto for igual ao código fornecido
            current->produto = Produto;                     // Atualiza o produto
            return 0;
        }
        current = current->next;                            // O próximo nó será o nó atual
    }
    return 1;
}

// Retorna o número de produtos numa linha de produtos
int numeroProdutosLinha(LinhaProdutos* linha) {
    Node* current = linha->top;                             // Obtém o nó do topo
    int count = 0;                                          // Inicializa o contador como 0
    while (current != NULL) {                               // Enquanto o nó atual não for NULL
        count++;                                            // Incrementa o contador
        current = current->next;                            // O próximo nó será o nó atual
    }
    return count;                                           // Retorna o contador
}

int numeroProdutosStock(StockLoja* stock) {
    int count = 0;                                                // Inicializa o contador como 0
    for (int i = 0; i < stock->num_linhas; i++) {                 // Para cada linha de produtos
        count += numeroProdutosLinha(&stock->linhas[i]);    // Incrementa o contador com o número de produtos na linha
    }
    return count;                                                 // Retorna o contador
}