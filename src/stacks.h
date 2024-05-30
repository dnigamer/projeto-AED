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
void pushListaLinhaProdutos(ListaLinhaProdutos*, LinhaProdutos*);
LinhaProdutos *popListaLinhaProdutos(ListaLinhaProdutos*);
ListaLinhaProdutos *createListaLinhaProdutos();
void destroyListaLinhaProdutos(ListaLinhaProdutos*);
int isEmptyListaLinhaProdutos(ListaLinhaProdutos*);

Produto *peekListaProdutos(ListaProdutos*);
void pushListaProdutos(ListaProdutos*, Produto*);
Produto *popListaProdutos(ListaProdutos*);
ListaProdutos *createListaProdutos();
void destroyListaProdutos(ListaProdutos*);
int isEmptyListaProdutos(ListaProdutos*);

ParamAdicionaisProduto *peekListaParamAdicionaisProduto(ListaParamAdicionaisProduto*);
void pushListaParamAdicionaisProduto(ListaParamAdicionaisProduto*, ParamAdicionaisProduto*);
ParamAdicionaisProduto *popListaParamAdicionaisProduto(ListaParamAdicionaisProduto*);
ListaParamAdicionaisProduto *createListaParamAdicionaisProduto();
void destroyListaParamAdicionaisProduto(ListaParamAdicionaisProduto*);
int isEmptyListaParamAdicionaisProduto(ListaParamAdicionaisProduto*);


#ifdef __cplusplus
}
#endif

#endif //PROJETO_STACKS_H
