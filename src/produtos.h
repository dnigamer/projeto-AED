// Criação dos structs que guardam informações sobre os produtos e as linhas de produtos
#ifndef PRODUTOS_H
#define PRODUTOS_H

#define MAX_CHAR 50

typedef struct 
{
    unsigned int codigo;
    char nome[MAX_CHAR];
    char marca[MAX_CHAR];
    float peso;
    float preco;
    unsigned int quantidade;
} Produto;

typedef struct Node
{
    Produto produto; // IMP: Primeiro produto da pilha.
    struct Node* next; // IMP: Ponteiro para o próximo produto da pilha.
} Node;

typedef struct
{
    unsigned int codigo;
    char nome[MAX_CHAR];
    unsigned int num_produtos; // IMP: Número de produtos na linha. RES: Contagem de num de produtos em array
    Node* top; // IMP: Ponteiro para o topo da pilha de produtos.
} LinhaProdutos;

typedef struct
{
    char nome[20]; // Max 20 chars porque sai fora da label do mainmenu
    unsigned int num_linhas; // IMP: Número de linhas de produtos. RES: Contagem de num de linhas em array
    LinhaProdutos* linhas; // IMP: Array de linhas de produtos.
} StockLoja;

#ifdef __cplusplus
extern "C" {
#endif

LinhaProdutos criarLinhaProdutos(int codigo, char nome[]);
LinhaProdutos obterLinhaProdutos(StockLoja* stockLoja, int codigo);

#ifdef __cplusplus
}
#endif

int adicionarLinhaProdutos(StockLoja *stock, LinhaProdutos *linha);
int apagaLinhaProdutos(StockLoja *stock, LinhaProdutos *linha);

Produto criarProduto(int codigo, char* nome, char* marca, float peso, float preco, int quantidade);
Produto* obterProduto(LinhaProdutos* linha, int codigo);
int adicionarProduto(LinhaProdutos *linha, Produto produto);
int removerProduto(LinhaProdutos* linha, int codigo);
int atualizarProduto(LinhaProdutos* linha, Produto produto);
int numeroProdutos(LinhaProdutos* linha);

#endif