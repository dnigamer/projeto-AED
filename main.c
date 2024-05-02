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
    LinhaProdutos lojinha = criarLinhaProdutos(1, "Legumes");
    adicionarLinhaProdutos(&stockGeral, &lojinha);

    startGui(argc, argv, &stockGeral);
    system(CLEAR);

    return 0;
}