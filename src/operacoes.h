//
// Created by Gonçalo Miranda on 10/05/2024.
//

#ifndef PROJETO_OPERACOES_H
#define PROJETO_OPERACOES_H

#include "produtos.h"

#ifdef __cplusplus
extern "C" {
#endif

// Funções para manipulação de stock
StockLoja criarStockLoja(char* nome);
int editarStockLoja(StockLoja* stockLoja, const char* nome);
void apagarLinhasProdutos(StockLoja* stockLoja);
void apagarProdutosLinhasStock(StockLoja* stockLoja);


// Funções para manipulação de linhas de produtos
LinhaProdutos criarLinhaProdutos(char* nome);
LinhaProdutos* obterLinhaProdutosPorID(StockLoja* stockLoja, int codigo);
LinhaProdutos* obterLinhaProdutosPorNome(StockLoja* stockLoja, char* nome);
int adicionarLinhaProdutos(StockLoja* stockLoja, LinhaProdutos linha);
int removerLinhaProdutos(StockLoja* stockLoja, unsigned int codigoLinha);
int atualizarLinhaProdutos(StockLoja* stockLoja, LinhaProdutos* linha);


// Funções para manipulação de listas de produtos
ListaProdutos* obterListaProdutosPorIDProduto(StockLoja* stock, int codLista);
ListaProdutos* obterListaProdutosPorIDLinha(StockLoja* stock, int codLinha);
ListaProdutos* obterListaProdutosPorNomeLinha(StockLoja* stock, char* nomeLinha);
ListaProdutos* procurarStockPorNomeProduto(StockLoja* stock, char* nomeItem);


// Funções para manipulação de produtos
Produto criarProduto(char* nome, char* item, char* modelo, unsigned int quantidade, double preco, ListaParamAdicionaisProduto* parametros);
Produto* obterProdutoPorID(LinhaProdutos* linha, unsigned int IDProduto);
Produto* obterProdutoPorNome(LinhaProdutos* linha, char* nomeProduto);
int adicionarProduto(LinhaProdutos* linha, Produto* produto);
int removerProduto(LinhaProdutos* linha, int codigo);
int atualizarProduto(LinhaProdutos* linha, Produto* produto);


// Outras funções
unsigned int getNumeroLinhasProdutos(StockLoja* stockLoja);
unsigned int getNumeroLinhasStock(StockLoja* stock);
unsigned int getNumeroProdutosLinha(LinhaProdutos* linha);
unsigned int getNumeroProdutosStock(StockLoja* stock);
unsigned int getNumeroTipoProdutosStock(StockLoja* stock);


#ifdef __cplusplus
}
#endif

#endif //PROJETO_OPERACOES_H
