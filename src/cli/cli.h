//
// Created by Gonçalo Miranda on 03/06/2024.
//

#ifndef PROJETO_CLI_H
#define PROJETO_CLI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "../operacoes.h"
#include "ioutil.h"
#include "../storage.h"

#define clear() system(CLEAR)

void line(int size);
void startCLI(StockLoja *stock);

#endif //PROJETO_CLI_H