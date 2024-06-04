//
// Created by Gonçalo Miranda on 03/06/2024.
//

#ifndef PROJETO_STORAGE_H
#define PROJETO_STORAGE_H

#include "produtos.h"

int guardarStock(StockLoja* stock, const char* nome_arquivo);
int carregarStock(StockLoja *stock, const char* nomeFicheiro);

#endif //PROJETO_STORAGE_H
