// Criação dos structs que guardam informações sobre os produtos e as linhas de produtos
#ifndef PRODUTOS_H
#define PRODUTOS_H

#define MAX_CHAR 50

typedef struct 
{
    int codigo;
    char nome[MAX_CHAR];
    char marca[MAX_CHAR];
    float peso;
    float preco;
    int quantidade;
} Produto;

typedef struct Node
{
    Produto produto; // IMP: Primeiro produto da pilha.
    struct Node* next; // IMP: Ponteiro para o próximo produto da pilha.
} Node;

typedef struct
{
    int codigo;
    char nome[MAX_CHAR];
    Node* top; // IMP: Ponteiro para o topo da pilha de produtos.
    int num_produtos; // IMP: Número de produtos na linha. RES: Contagem de num de produtos em array
} LinhaProdutos;

LinhaProdutos criarLinhaProdutos(int codigo, char nome[]);
int apagaLinhaProdutos(LinhaProdutos *linha);

Produto criarProduto(int codigo, char nome[], char marca[], float peso, float preco, int quantidade);
Produto* obterProduto(LinhaProdutos* linha, int codigo);
int adicionarProduto(LinhaProdutos *linha, Produto produto);
int removerProduto(LinhaProdutos* linha, int codigo);

#endif