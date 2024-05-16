//
// Created by Gonçalo Miranda on 16/05/2024.
//

#include <stdlib.h>
#include "stacks.h"

/* Definição das operações das stacks:
 *  - peek - devolve o elemento do topo da stack
 *  - push - adiciona um elemento à stack
 *  - pop - remove o elemento do topo da stack
 *  - create - cria uma nova stack
 *  - destroy - destroi a stack
 *  - isEmpty - verifica se a stack está vazia
*/


LinhaProdutos *peekListaLinhaProdutos(ListaLinhaProdutos* listalinhas) {
    return listalinhas->linha;
}

void pushListaLinhaProdutos(ListaLinhaProdutos** listalinhas, LinhaProdutos* linhaprodutos) {
    ListaLinhaProdutos *novaLista = (ListaLinhaProdutos*) malloc(sizeof(ListaLinhaProdutos));
    novaLista->linha = linhaprodutos;
    novaLista->prox_linha = *listalinhas;
    *listalinhas = novaLista;
}

LinhaProdutos *popListaLinhaProdutos(ListaLinhaProdutos** listalinhas) {
    if (*listalinhas == NULL) return NULL;

    ListaLinhaProdutos *temp = *listalinhas;
    LinhaProdutos *linha = temp->linha;
    *listalinhas = temp->prox_linha;
    free(temp);

    return linha;
}

ListaLinhaProdutos *createListaLinhaProdutos() {
    ListaLinhaProdutos *listalinhas = (ListaLinhaProdutos*) malloc(sizeof(ListaLinhaProdutos));
    listalinhas->linha = NULL;
    listalinhas->prox_linha = NULL;
    return listalinhas;
}

int isEmptyListaLinhaProdutos(ListaLinhaProdutos* listalinhas) {
    return listalinhas->linha == NULL;
}

void destroyListaLinhaProdutos(ListaLinhaProdutos** listalinhas) {
    while (!isEmptyListaLinhaProdutos(*listalinhas)) {
        LinhaProdutos* linha = popListaLinhaProdutos(listalinhas);
        free(linha);
    }
}


Produto *peekListaProdutos(ListaProdutos* listaprodutos) {
    return listaprodutos->produto;
}

void pushListaProdutos(ListaProdutos** listaprodutos, Produto* produto) {
    ListaProdutos *novaLista = (ListaProdutos*) malloc(sizeof(ListaProdutos));
    novaLista->produto = produto;
    novaLista->prox_produto = *listaprodutos;
    *listaprodutos = novaLista;
}

Produto *popListaProdutos(ListaProdutos** listaprodutos) {
    if (*listaprodutos == NULL) return NULL;

    ListaProdutos *temp = *listaprodutos;
    Produto *produto = temp->produto;
    *listaprodutos = temp->prox_produto;
    free(temp);

    return produto;
}

ListaProdutos *createListaProdutos() {
    ListaProdutos *listaprodutos = (ListaProdutos*) malloc(sizeof(ListaProdutos));
    listaprodutos->produto = NULL;
    listaprodutos->prox_produto = NULL;
    return listaprodutos;
}

int isEmptyListaProdutos(ListaProdutos* listaprodutos) {
    return listaprodutos->produto == NULL;
}

void destroyListaProdutos(ListaProdutos** listaprodutos) {
    while (!isEmptyListaProdutos(*listaprodutos)) {
        Produto* produto = popListaProdutos(listaprodutos);
        free(produto);
    }
}


ParamAdicionaisProduto *peekListaParamAdicionaisProduto(ListaParamAdicionaisProduto* listaparams) {
    return listaparams->parametro;
}

void pushListaParamAdicionaisProduto(ListaParamAdicionaisProduto** listaparams, ParamAdicionaisProduto* novoparams) {
    ListaParamAdicionaisProduto *novaLista = (ListaParamAdicionaisProduto*) malloc(sizeof(ListaParamAdicionaisProduto));
    novaLista->parametro = novoparams;
    novaLista->prox_parametro = *listaparams;
    *listaparams = novaLista;
}

ParamAdicionaisProduto *popListaParamAdicionaisProduto(ListaParamAdicionaisProduto** listaparams) {
    if (*listaparams == NULL) return NULL;

    ListaParamAdicionaisProduto *temp = *listaparams;
    ParamAdicionaisProduto *param = temp->parametro;
    *listaparams = temp->prox_parametro;
    free(temp);

    return param;
}

ListaParamAdicionaisProduto *createListaParamAdicionaisProduto() {
    ListaParamAdicionaisProduto *listaparams = (ListaParamAdicionaisProduto*) malloc(sizeof(ListaParamAdicionaisProduto));
    listaparams->parametro = NULL;
    listaparams->prox_parametro = NULL;
    return listaparams;
}

int isEmptyListaParamAdicionaisProduto(ListaParamAdicionaisProduto* listaparams) {
    return listaparams->parametro == NULL;
}

void destroyListaParamAdicionaisProduto(ListaParamAdicionaisProduto** listaparams) {
    while (!isEmptyListaParamAdicionaisProduto(*listaparams)) {
        ParamAdicionaisProduto* param = popListaParamAdicionaisProduto(listaparams);
        free(param);
    }
}
