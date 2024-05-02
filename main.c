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
    StockLoja stockGeral = { "Mercado do bolhão", 0, NULL};
    LinhaProdutos linhatemp = criarLinhaProdutos(1, "Legumes");
    adicionarLinhaProdutos(&stockGeral, &linhatemp);
    linhatemp = criarLinhaProdutos(2, "Frutas");
    adicionarLinhaProdutos(&stockGeral, &linhatemp);

    // TEMPORARIO
    // adicionar produto a linha de produtos
    Produto produto = criarProduto(1, "Maçã", "Golden", 0.2, 0.5, 10);
    adicionarProduto(&stockGeral.linhas[0], produto);

    produto = criarProduto(2, "Banana", "Madeira", 0.1, 0.3, 20);
    adicionarProduto(&stockGeral.linhas[1], produto);



    startGui(argc, argv, &stockGeral);
    system(CLEAR);

    return 0;
}