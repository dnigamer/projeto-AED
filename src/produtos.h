// Criação dos structs que guardam informações sobre os produtos e as linhas de produtos
#ifndef PRODUTOS_H
#define PRODUTOS_H

#define MAX_CHAR 50

#ifdef __cplusplus
extern "C" {
#endif

// definição de struct de parâmetros adicionais para um produto
typedef struct {
    unsigned int id;
    char nome[MAX_CHAR];
    char valor[MAX_CHAR];
} ParamAdicionalProduto;

// definição de struct de lista de parâmetros adicionais para um produto
typedef struct ListaParamAdicionalProduto {
    ParamAdicionalProduto* parametro;
    struct ListaParamAdicionalProduto* prox_parametro;
} ListaParamAdicionalProduto;


// definição de struct de produto
typedef struct {
    unsigned int linhaID;
    unsigned int produtoID;
    char nome[MAX_CHAR];
    char modelo[MAX_CHAR];
    char item[MAX_CHAR];
    unsigned int quantidade;
    double preco;
    unsigned int num_parametros;
    ListaParamAdicionalProduto* parametros;
} Produto;

// definição de struct de lista de produtos para uma linha de produtos
typedef struct ListaProdutos {
    Produto* produto;
    struct ListaProdutos* prox_produto;
} ListaProdutos;


// definição de struct de linha de produtos
typedef struct {
    unsigned int linhaID;
    char nome[MAX_CHAR];
    unsigned int num_produtos;
    ListaProdutos* lista_produtos;
} LinhaProdutos;

typedef struct ListaLinhaProdutos {
    LinhaProdutos* linha;
    struct ListaLinhaProdutos* prox_linha;
} ListaLinhaProdutos;


// definição de struct de stock para a loja
typedef struct {
    char nome[30];
    unsigned int num_linhas;
    ListaLinhaProdutos* lista_linhas;
} StockLoja;


#ifdef __cplusplus
}
#endif

#endif