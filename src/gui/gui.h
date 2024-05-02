//
// Created by Gonçalo Miranda on 30/04/2024.
//

#ifndef PROJETO_GUI_H
#define PROJETO_GUI_H

#include "../produtos.h"

#ifdef __cplusplus
extern "C" {
    void startGui(int argc, char **argv, StockLoja *stockGeral);
}
#endif

void startGui(int argc, char *argv[], StockLoja *stockGeral);

#endif
