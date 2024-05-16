//
// Created by Gonçalo Miranda on 16/05/2024.
//

#ifndef PROJETO_STACKS_H
#define PROJETO_STACKS_H

#include "produtos.h"

#ifdef __cplusplus
extern "C" {
#endif

LinhaProdutos *peekListaLinhaProdutos(ListaLinhaProdutos*);
void pushListaLinhaProdutos(ListaLinhaProdutos**, LinhaProdutos*);
LinhaProdutos *popListaLinhaProdutos(ListaLinhaProdutos**);
ListaLinhaProdutos *createListaLinhaProdutos();
int isEmptyListaLinhaProdutos(ListaLinhaProdutos*);
void destroyListaLinhaProdutos(ListaLinhaProdutos**);

Produto *peekListaProdutos(ListaProdutos*);
void pushListaProdutos(ListaProdutos**, Produto*);
Produto *popListaProdutos(ListaProdutos**);
ListaProdutos *createListaProdutos();
int isEmptyListaProdutos(ListaProdutos*);
void destroyListaProdutos(ListaProdutos**);

ParamAdicionaisProduto *peekListaParamAdicionaisProduto(ListaParamAdicionaisProduto*);
void pushListaParamAdicionaisProduto(ListaParamAdicionaisProduto**, ParamAdicionaisProduto*);
ParamAdicionaisProduto *popListaParamAdicionaisProduto(ListaParamAdicionaisProduto**);
ListaParamAdicionaisProduto *createListaParamAdicionaisProduto();
int isEmptyListaParamAdicionaisProduto(ListaParamAdicionaisProduto*);
void destroyListaParamAdicionaisProduto(ListaParamAdicionaisProduto**);


#ifdef __cplusplus
}
#endif

#endif //PROJETO_STACKS_H
