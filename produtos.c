#include "produtos.h"
#include <string.h>

// Cria uma nova instância para a linha de produtos
// Retorna a linha de produtos
LinhaProdutos criarLinhaProdutos(int codigo, char nome[]) {
    LinhaProdutos lp;                                       // Cria uma nova instância para a linha de produtos
    lp.codigo = codigo;                                     // Atribui o código à linha de produtos
    strncpy(lp.nome, nome, 50);                             // Atribui o nome à linha de produtos
    lp.top = NULL;                                          // Inicializa o topo da stack como NULL
    return lp;                                              // Retorna a linha de produtos
}

// Apaga uma linha de produtos
// Retorna 0 se a linha foi apagada com sucesso
// Retorna 1 se a linha não foi apagada
int apagaLinhaProdutos(LinhaProdutos* linha) {
    if (linha->top == NULL) return 1;                       // Retorna 1 se a linha não foi apagada
    Node* top_node = linha->top;                            // Obtém o ponteiro para o nó do topo
    free(top_node);                                         // Libera a memória do nó do topo, apagando a stack inteira
    return 0;
}

// Cria um novo produto usando os parâmetros fornecidos
// Retorna o produto
Produto criarProduto(int codigo, char* nome, char* marca, float preco, int quantidade) {
    Produto p;                                              // Cria uma nova instância para o produto
    p.codigo = codigo;                                      // Atribui o código ao produto
    strncpy(p.nome, nome, 50);                              // Atribui o nome ao produto
    strncpy(p.marca, marca, 50);                            // Atribui a marca ao produto
    p.preco = preco;                                        // Atribui o preço ao produto
    p.quantidade = quantidade;                              // Atribui a quantidade ao produto
    return p;                                               // Retorna o produto
}

// Adiciona um produto a uma linha de produtos
// Retorna 0 se o produto foi adicionado com sucesso
// Retorna 1 se houve um erro ao alocar memória
int adicionarProduto(LinhaProdutos* linha, Produto produto) {  
    Node* novo_node = (Node*) malloc(sizeof(Node));         // Aloca memória para um novo nó
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
int numeroProdutos(LinhaProdutos* linha) {
    Node* current = linha->top;                             // Obtém o nó do topo
    int count = 0;                                          // Inicializa o contador como 0
    while (current != NULL) {                               // Enquanto o nó atual não for NULL
        count++;                                            // Incrementa o contador
        current = current->next;                            // O próximo nó será o nó atual
    }
    return count;                                           // Retorna o contador
}