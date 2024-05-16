//
// Created by Gonçalo Miranda on 16/05/2024.
//

#include "stacks.h"

// retorna a linha de produtos no topo da lista das linha
LinhaProdutos *peekListaLinhaProdutos(ListaLinhaProdutos *lista) {
    return lista->linha;
}