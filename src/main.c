#include "operacoes.h"
#include "gui/gui.h"

int main(int argc, char *argv[])
{
    StockLoja stock2 = *criarStockLoja("", 1);
    startGui(argc, argv, &stock2);

    return 0;
}