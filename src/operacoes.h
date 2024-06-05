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

// cria instância de stock
StockLoja* criarStockLoja(char* nome);
// edita o nome da loja (somente)
int editarStockLoja(StockLoja* stockLoja, const char* nome);
// apaga todas as linhas de produtos do stock
int apagarLinhasProdutos(StockLoja* stockLoja);
// apaga todos os produtos e todas as linhas de produtos do stock
int apagarProdutosLinhasStock(StockLoja* stockLoja);


// Funções para manipulação de Linhas de Produtos (mais geral)

// cria uma instância de uma Linha de Produtos com o nome especificado
LinhaProdutos criarLinhaProdutos(char* nome);
// procura por Linha de Produtos por ID/código
LinhaProdutos* obterLinhaProdutosPorID(StockLoja* stockLoja, int codigo);
// procura por Linha de Produtos por nome providenciado
LinhaProdutos* obterLinhaProdutosPorNome(StockLoja* stockLoja, char* nome);
// adiciona uma Linha de Produtos ao stock
int adicionarLinhaProdutos(StockLoja* stockLoja, LinhaProdutos linha);
// remove uma Linha de Produtos do stock baseado no código dessa Linha
int removerLinhaProdutos(StockLoja* stockLoja, unsigned int codigoLinha);
// atualiza uma Linha de Produtos no stock com uma nova linha
int atualizarLinhaProdutos(StockLoja* stockLoja, LinhaProdutos* linha);


/// Funções sobre Listas de Produtos (listas onde inclui somente os produtos

// obtem a Lista de Produtos inteira baseada no ID da linha em que está incluida essa lista
ListaProdutos* obterListaProdutosPorIDLinha(StockLoja* stock, int codLinha);
// obtem a Lista de Produtos inteira baseada no nome da linha em que está incluida essa lista
ListaProdutos* obterListaProdutosPorNomeLinha(StockLoja* stock, char* nomeLinha);
// obtem a Lista de Produtos inteira baseada no nome do produto nessa lista
ListaProdutos* procurarStockPorNomeProduto(StockLoja* stock, char* nomeItem);
// obtem uma lista de produtos baseada no nome do item numa linha
ListaProdutos* obterListaProdutosPorItemLinha(LinhaProdutos* linha, char* nomeItem);


/// Funções para manipulação de produtos

// cria uma instância de um produto
Produto criarProduto(char* nome, char* item, char* modelo, unsigned int quantidade, double preco, ListaParamAdicionalProduto* parametros);
// procura por um produto numa linha de produtos por ID
Produto* obterProdutoPorID(LinhaProdutos* linha, unsigned int IDProduto);
// procura por um produto numa linha de produtos por nome
Produto* obterProdutoPorNome(LinhaProdutos* linha, char* nome, char* modelo);
// procura por um produto numa linha de produtos por nome do item
int adicionarProduto(LinhaProdutos* linha, Produto* produto);
// remove um produto de uma linha de produtos
int removerProduto(LinhaProdutos* linha, unsigned int codigo);
// atualiza um produto numa linha de produtos
int atualizarProduto(LinhaProdutos* linha, Produto* produto);


/// Funções para manipulação de parâmetros adicionais de produtos

// cria uma instância de uma lista de parâmetros adicionais de produtos
ListaParamAdicionalProduto* criarListaParamAdicionaisProduto();
// cria uma instância de um parâmetro adicional para um produto
ParamAdicionalProduto* criarParametroAdicionalProduto(char* nome, char* valor);
// adiciona um parâmetro adicional a uma lista de parâmetros adicionais de produtos
int adicionarParametroAdicionalProduto(Produto* produto, ParamAdicionalProduto* parametro);
// adiciona uma lista de parâmetros adicionais a um produto
int adicionarListaParametroAdicionalProduto(Produto* produto, ListaParamAdicionalProduto* parametros);
// adiciona um parametro adicional a uma lista de parametros adicionais de produtos
int adicionarParametroAdicionalLista(ListaParamAdicionalProduto** lista, ParamAdicionalProduto* parametro);
// obtem o número de parâmetros adicionais numa lista de parâmetros adicionais de produtos
ParamAdicionalProduto* obterParametroAdicionalPorID(Produto* produto, unsigned int codigoParametro);
// procura por um parâmetro adicional numa lista de parâmetros adicionais de produtos por nome
ParamAdicionalProduto* obterParametroAdicionalPorNome(Produto* produto, char* nomeParametro);
// atualiza um parâmetro adicional numa lista de parâmetros adicionais de produtos
unsigned int getNumeroParametrosAdicionais(ListaParamAdicionalProduto* lista);


/// Outras funções (maioritariamente para GUI/estatísticas do programa)

// obtem o número de linhas de produtos no stock
unsigned int getNumeroLinhasProdutos(StockLoja* stockLoja);
// obtem o número de produtos numa linha de produtos
unsigned int getNumeroProdutosLinha(LinhaProdutos* linha);
// obtem o número de produtos numa lista de produtos
unsigned int getNumeroProdutosLista(ListaProdutos* lista);
// obtem o número de produtos no stock
unsigned int getNumeroProdutosStock(StockLoja* stock);
// obtem o número de tipos de produtos no stock
unsigned int getNumeroTipoProdutosStock(StockLoja* stock);


/// Pesquisa de itens
/// código, nome, modelo, categoria/item, parametro, linha

// procura por modelo de produto (apresenta Lista de produtos)
ListaProdutos* procurarStockPorModeloProduto(StockLoja* stock, char* modelo);
// procura por categoria de produto (apresenta Lista de produtos)
ListaProdutos* procurarStockPorItemProduto(StockLoja* stock, char* item);
// procura por parametro de produto (apresenta Lista de produtos)
ListaProdutos* procurarStockPorParametroProduto(StockLoja* stock, char* parametro);


#ifdef __cplusplus
}
#endif

#endif //PROJETO_OPERACOES_H
