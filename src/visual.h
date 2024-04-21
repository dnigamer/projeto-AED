#ifndef VISUAL_H
#define VISUAL_H

#include "produtos.h"

int mainMenu();
int productsInitMenu();
int listsInitMenu();

void initmenu(int* tipo, int* operation);
int confirmMenu();

int productsMenu(int operation, StockLoja* stock);

#endif