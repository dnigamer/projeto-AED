#include <stdio.h>
#include <stdlib.h>
#include "src/produtos.h"
#include "src/gui/gui.h"

#if defined(_WIN32) || defined(_WIN64)
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

int main(int argc, char *argv[])
{
    // Define uma instância de stock
    StockLoja stockGeral = { "Mercado do bolhão", 0, NULL};

    // TEMPORARIO
    // cria uma linha de produtos e adiciona ao stock
    LinhaProdutos linhatemp = criarLinhaProdutos(1, "Legumes");
    adicionarLinhaProdutos(&stockGeral, &linhatemp);

    // cria outra linha de produtos para stock
    linhatemp = criarLinhaProdutos(2, "Frutas");
    adicionarLinhaProdutos(&stockGeral, &linhatemp);

    // TEMPORARIO
    // adicionar produto a linha de produtos
    Produto produto = { 1, "Maçã", "Fruta", "Maçã", 10, 0.5, 0, NULL };
    adicionarProduto(obterLinhaProdutos(&stockGeral, 1), &produto);

    // TEMPORARIO
    // adicionar produto a linha de produtos
    Produto produto2 = { 2, "Banana", "Fruta", "Banana", 10, 0.5, 0, NULL };
    adicionarProduto(obterLinhaProdutos(&stockGeral, 1), &produto2);

    // chama a função para iniciar a GUI
    startGui(argc, argv, &stockGeral);
    system(CLEAR);

    return 0;
}