// Criação dos structs que guardam informações sobre os produtos e as linhas de produtos
#ifndef PRODUTOS_H
#define PRODUTOS_H

#define MAX_CHAR 50

#ifdef __cplusplus
extern "C" {
#endif

// definição de struct de parâmetros adicionais para um produto
typedef struct {
    char nome[MAX_CHAR];
    char valor[MAX_CHAR];
} ParamAdicionaisProduto;

// definição de struct de lista de parâmetros adicionais para um produto
typedef struct ListaParamAdicionaisProduto {
    ParamAdicionaisProduto* parametro;
    struct ListaParamAdicionaisProduto* prox_parametro;
} ListaParamAdicionaisProduto;


// definição de struct de produto
typedef struct 
{
    unsigned int codigo;
    char nome[MAX_CHAR]; // nome inteiro do produto (item + modelo + outros)
    char item[MAX_CHAR];
    char modelo[MAX_CHAR];
    unsigned int quantidade;
    double preco;
    unsigned int num_parametros;
    ListaParamAdicionaisProduto* parametros;
} Produto;

// definição de struct de lista de produtos para uma linha de produtos
typedef struct ListaProduto {
    Produto* produto;
    struct ListaProduto* prox_produto;
} ListaProduto;


// definição de struct de linha de produtos
typedef struct {
    unsigned int codigo;
    char nome[MAX_CHAR];
    unsigned int num_produtos;
    ListaProduto* lista_produtos;
} LinhaProdutos;

typedef struct ListaLinhaProdutos {
    LinhaProdutos* linha;
    struct ListaLinhaProdutos* prox_linha;
} ListaLinhaProdutos;


// definição de struct de stock para a loja
typedef struct {
    char nome[20];
    unsigned int num_linhas;
    ListaLinhaProdutos* lista_produtos;
} StockLoja;

// Funções para manipulação de linhas de produtos
LinhaProdutos criarLinhaProdutos(unsigned int codigo, char* nome);
LinhaProdutos* obterLinhaProdutos(StockLoja* stockLoja, int codigo);
int adicionarLinhaProdutos(StockLoja* stockLoja, LinhaProdutos* linha);
int removerLinhaProdutos(StockLoja* stockLoja, int codigo);
int atualizarLinhaProdutos(StockLoja* stockLoja, LinhaProdutos* linha);
unsigned int getNumeroLinhasProdutos(StockLoja* stockLoja);

// Funções para manipulação de produtos
Produto criarProduto(unsigned int codigo, char* nome, char* item, char* modelo, unsigned int quantidade, double preco, ListaParamAdicionaisProduto* parametros);
Produto* obterProduto(LinhaProdutos* linha, int codigo);
int adicionarProduto(LinhaProdutos* linha, Produto* produto);
int removerProduto(LinhaProdutos* linha, int codigo);
int atualizarProduto(LinhaProdutos* linha, Produto* produto);

// Outras funções
int numeroProdutosLinha(LinhaProdutos* linha);
int numeroProdutosStock(StockLoja* stock);

#ifdef __cplusplus
}
#endif

#endif