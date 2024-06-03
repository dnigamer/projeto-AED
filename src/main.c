#include <stdio.h>
#include <stdlib.h>
#include "operacoes.h"
#include "gui/gui.h"
#include "cli/cli.h"

#if defined(_WIN32) || defined(_WIN64)
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

int main(int argc, char *argv[])
{
    // Define uma instância de stock
    StockLoja stockGeral = { "Mercado do bolhão", 0, NULL};

    // cria uma linha de produtos e adiciona ao stock
    LinhaProdutos linhatemp;

    linhatemp = criarLinhaProdutos("Legumes");
    adicionarLinhaProdutos(&stockGeral, linhatemp);

    linhatemp = criarLinhaProdutos("Frutas");
    adicionarLinhaProdutos(&stockGeral, linhatemp);

    linhatemp = criarLinhaProdutos("Iogurtes");
    adicionarLinhaProdutos(&stockGeral, linhatemp);

    // TEMPORARIO
    // adicionar produto a linha de produtos
    ListaParamAdicionalProduto *parametros = NULL;
    ParamAdicionalProduto parametro = { 0, "Cor", "Vermelho" };
    Produto produto = { 0, 0, 0,"Maçã", "Fruta", "Golden", 10, 0.5, 0, parametros };
    adicionarParametroAdicionalProduto(&produto, &parametro);
    adicionarProduto(obterLinhaProdutosPorID(&stockGeral, 2), &produto);

    Produto produto2 = { 0, 0, 0,"Maçã", "Fruta", "Grande", 10, 0.5, 0, NULL };
    adicionarProduto(obterLinhaProdutosPorID(&stockGeral, 2), &produto2);

    // adicionar produto a linha de produtos
    Produto produto3 = { 0, 0, 0, "Banana", "Fruta", "Madeira", 10, 0.5, 0, NULL };
    adicionarProduto(obterLinhaProdutosPorID(&stockGeral, 2), &produto3);

    Produto produto4 = { 0, 0, 0, "Pêra", "Fruta", "Rocha", 10, 0.5, 0, NULL };
    adicionarProduto(obterLinhaProdutosPorID(&stockGeral, 2), &produto4);

    // chama a função para iniciar a GUI
    //startGui(argc, argv, &stockGeral);
    startCLI(&stockGeral);
    system(CLEAR);

    return 0;
}