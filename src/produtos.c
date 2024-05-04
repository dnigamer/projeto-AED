#include "produtos.h"
#include <string.h>
#include <stdlib.h>

// Cria uma nova instância para a linha de produtos
// Retorna a linha de produtos
LinhaProdutos criarLinhaProdutos(unsigned int codigo, char* nome) {
    LinhaProdutos linha;
    linha.codigo = codigo;
    strncpy(linha.nome, nome, 50);
    linha.num_produtos = 0;
    linha.lista_produtos = NULL;
    return linha;
}

LinhaProdutos* obterLinhaProdutos(StockLoja* stockLoja, int codigo) {
    ListaLinhaProdutos* atual_linha = stockLoja->lista_linhas;
    while (atual_linha != NULL) {
        if (atual_linha->linha->codigo == codigo) {
            return atual_linha->linha;
        }
        atual_linha = (ListaLinhaProdutos *) atual_linha->prox_linha;
    }
    return NULL;
}

// Criar linha de produtos dentro do stock da loja
// Usa stock da loja como pointer e usa linha de produtos a incluir no stock como pointer
int adicionarLinhaProdutos(StockLoja* stockLoja, LinhaProdutos* linha) {
    ListaLinhaProdutos* novo_no = (ListaLinhaProdutos*) malloc(sizeof(ListaLinhaProdutos));
    if (novo_no == NULL) return 1;

    novo_no->linha = linha;
    novo_no->prox_linha = (struct ListaLinhaProdutos *) stockLoja->lista_linhas;
    stockLoja->lista_linhas = novo_no;

    stockLoja->num_linhas++;
    return 0;
}

// Remove uma linha de produtos do stock da loja
// Utiliza o stock da loja como pointer e o código da linha a remover
int removerLinhaProdutos(StockLoja* stockLoja, int codigo) {
    if (stockLoja->lista_linhas == NULL) return 1;

    ListaLinhaProdutos* temp = stockLoja->lista_linhas;
    ListaLinhaProdutos* prev = NULL;

    // Se o primeiro nó contém a chave a ser excluída
    if (temp != NULL && temp->linha->codigo == codigo) {
        stockLoja->lista_linhas = (ListaLinhaProdutos *) temp->prox_linha;
        free(temp);
        return 0;
    }

    // Procura a chave a ser excluída, mantendo o nó anterior, pois precisamos alterar 'prev->next'
    while (temp != NULL && temp->linha->codigo != codigo) {
        prev = temp;
        temp = (ListaLinhaProdutos *) temp->prox_linha;
    }

    // Se a chave não estava presente na lista
    if (temp == NULL) return 1;

    // Remove o linha da lista
    if (prev != NULL) {
        prev->prox_linha = temp->prox_linha;
    }

    free(temp);  // Libera a memória da linha

    return 0;
}

// Atualiza uma linha de produtos no stock da loja
// Utiliza o stock da loja como pointer e a linha de produtos a atualizar como pointer
int atualizarLinhaProdutos(StockLoja* stockLoja, LinhaProdutos* linha) {
    ListaLinhaProdutos* current = stockLoja->lista_linhas;
    while (current != NULL) {
        if (current->linha->codigo == linha->codigo) {
            current->linha = linha;
            return 0;
        }
        current = (ListaLinhaProdutos *) current->prox_linha;
    }
    return 1;
}

// Retorna o número de linhas de produtos no stock da loja
unsigned int getNumeroLinhasProdutos(StockLoja* stockLoja) {
    return stockLoja->num_linhas;
}


// Cria um novo produto usando os parâmetros fornecidos
// Retorna um struct de produto
Produto criarProduto(unsigned int codigo, char* nome, char* item, char* modelo, unsigned int quantidade, double preco, ListaParamAdicionaisProduto* parametros) {
    Produto p;
    p.codigo = codigo;
    strncpy(p.nome, nome, 50);
    strncpy(p.item, item, 50);
    strncpy(p.modelo, modelo, 50);
    p.quantidade = quantidade;
    p.preco = preco;
    p.parametros = parametros;
    return p;
}

// Retorna um produto de uma linha de produtos
Produto* obterProduto(LinhaProdutos* linha, int codigo) {
    ListaProduto* current = linha->lista_produtos;
    while (current != NULL) {
        if (current->produto->codigo == codigo) {
            return (Produto *) &current->produto;
        }
        current = (ListaProduto *) current->prox_produto;
    }
    return NULL;
}


// Adiciona um produto a uma linha de produtos
// Utiliza a linha de produtos como pointer e o produto a adicionar como pointer
// Usa o método de push para adicionar um produto a lista dentro da linha de produtos
int adicionarProduto(LinhaProdutos* linha, Produto* produto) {
    ListaProduto* novo_no = (ListaProduto*) malloc(sizeof(ListaProduto));
    if (novo_no == NULL) return 1;

    novo_no->produto = produto;
    novo_no->prox_produto = (struct ListaProduto *) linha->lista_produtos;
    linha->lista_produtos = novo_no;

    linha->num_produtos++;
    return 0;
}

// Remove um produto de uma linha de produtos
// Usar metodos de pop para remover
int removerProduto(LinhaProdutos* linha, int codigo) {
    if (linha->lista_produtos == NULL || linha->num_produtos == 0)
        return 1; // Linha vazia

    ListaProduto* atualLista = linha->lista_produtos;
    ListaProduto* anteriorLista = NULL;

    while (atualLista != NULL) {
        if (atualLista->produto->codigo == codigo) {
            if (anteriorLista == NULL) {
                linha->lista_produtos = (ListaProduto *) atualLista->prox_produto;
            } else {
                anteriorLista->prox_produto = (struct ListaProduto *) atualLista->prox_produto;
            }
            free(atualLista);
            return 0;
        }
        anteriorLista = atualLista;
        atualLista = (ListaProduto *) atualLista->prox_produto;
    }
    return 1;
}

// Atualiza um produto numa linha de produtos
// Retorna 0 se o produto foi atualizado com sucesso
// Retorna 1 se o produto não foi encontrado
int atualizarProduto(LinhaProdutos* linha, Produto* produto) {
    ListaProduto* current = linha->lista_produtos;
    while (current != NULL) {
        if (current->produto->codigo == produto->codigo) {
            current->produto = produto;
            return 0;
        }
        current = (ListaProduto *) current->prox_produto;
    }
    return 1;
}

// Retorna o número de produtos numa linha de produtos vendo quantos produtos existem em cada lista
int numeroProdutosLinha(LinhaProdutos* linha) {
    int count = 0;
    ListaProduto* current = linha->lista_produtos;
    while (current != NULL) {
        count++;
        current = (ListaProduto *) current->prox_produto;
    }
    return count;
}

// Retorna o número de produtos total no stock da loja
int numeroProdutosStock(StockLoja* stock) {
    int count = 0;
    ListaLinhaProdutos* current = stock->lista_produtos;
    while (current != NULL) {
        count += numeroProdutosLinha(current->linha);
        current = (ListaLinhaProdutos *) current->prox_linha;
    }
    return count;
}