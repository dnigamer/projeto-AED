//
// Created by Gonçalo Miranda on 03/06/2024.
//

#ifndef PROJETO_STORAGE_H
#define PROJETO_STORAGE_H

#include "produtos.h"

#ifdef __cplusplus
extern "C" {
#endif

int guardarStock(StockLoja* stock, const char* nome_arquivo);
int carregarStock(StockLoja *stock, const char* nomeFicheiro);

#ifdef __cplusplus
};
#endif

#endif //PROJETO_STORAGE_H
