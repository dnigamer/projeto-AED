//
// Created by Gonçalo Miranda on 10/05/2024.
//

#include "operacoes.h"
#include <string.h>
#include <stdlib.h>

// Cria uma nova instância de stock da loja
// Retorna o stock da loja
StockLoja criarStockLoja(char* nome) {
    StockLoja stock;
    strncpy(stock.nome, nome, 20);
    stock.num_linhas = 0;
    stock.lista_linhas = NULL;
    return stock;
}

// Adiciona a funcionalidade de editar o nome da loja partindo do pointer e de uma string com o novo nome
int editarStockLoja(StockLoja* stockLoja, const char* nome) {
    strncpy(stockLoja->nome, nome, 20);
    return 0;
}

void apagarLinhasProdutos(StockLoja* stockLoja) {
    ListaLinhaProdutos* current = stockLoja->lista_linhas;
    while (current != NULL) {
        ListaLinhaProdutos* next = (ListaLinhaProdutos *) current->prox_linha;
        free(current);
        current = next;
    }
    stockLoja->lista_linhas = NULL;
    stockLoja->num_linhas = 0;
}

void apagarProdutosLinhasStock(StockLoja* stockLoja) {
    ListaLinhaProdutos* current = stockLoja->lista_linhas;
    while (current != NULL) {
        ListaProdutos* currentProd = current->linha->lista_produtos;
        while (currentProd != NULL) {
            ListaProdutos* next = (ListaProdutos *) currentProd->prox_produto;
            free(currentProd);
            currentProd = next;
        }
        current->linha->lista_produtos = NULL;
        current->linha->num_produtos = 0;
        current = (ListaLinhaProdutos *) current->prox_linha;
    }
}


// Cria uma nova instância para a linha de produtos
// Retorna a linha de produtos
LinhaProdutos criarLinhaProdutos(char* nome) {
    LinhaProdutos linha;
    linha.linhaID = 0;
    strncpy(linha.nome, nome, 50);
    linha.num_produtos = 0;
    linha.lista_produtos = NULL;
    return linha;
}

LinhaProdutos* obterLinhaProdutosPorID(StockLoja* stockLoja, int codigo) {
    ListaLinhaProdutos* current = stockLoja->lista_linhas;
    while (current != NULL) {
        if (current->linha->linhaID == codigo) {
            return (LinhaProdutos *) current->linha;
        }
        current = (ListaLinhaProdutos *) current->prox_linha;
    }
    return NULL;
}

LinhaProdutos* obterLinhaProdutosPorNome(StockLoja* stockLoja, char* nome) {
    ListaLinhaProdutos* current = stockLoja->lista_linhas;
    while (current != NULL) {
        if (strcmp(current->linha->nome, nome) == 0) {
            return (LinhaProdutos *) current->linha;
        }
        current = (ListaLinhaProdutos *) current->prox_linha;
    }
    return NULL;
}

// Criar linha de produtos dentro do stock da loja
// Usa stock da loja como pointer e usa uma instância de LinhaProdutos a incluir no stock
int adicionarLinhaProdutos(StockLoja* stockLoja, LinhaProdutos linha) {
    ListaLinhaProdutos* novo_no = (ListaLinhaProdutos*) malloc(sizeof(ListaLinhaProdutos));
    if (novo_no == NULL) return 1;

    linha.linhaID = stockLoja->num_linhas + 1;
    novo_no->linha = (LinhaProdutos *) malloc(sizeof(LinhaProdutos));
    if (novo_no->linha == NULL) return 1;

    *novo_no->linha = linha;
    novo_no->prox_linha = (struct ListaLinhaProdutos *) stockLoja->lista_linhas;
    stockLoja->lista_linhas = novo_no;

    stockLoja->num_linhas++;
    return 0;
}

// Remove uma linha de produtos do stock da loja
// Utiliza o stock da loja como pointer e o código da linha a remover
int removerLinhaProdutos(StockLoja* stockLoja, unsigned int codigoLinha) {
    if (stockLoja->lista_linhas == NULL) return 1;  // stock sem linhas
    ListaLinhaProdutos* temp = stockLoja->lista_linhas; // pegar no primeiro no
    ListaLinhaProdutos* prev = NULL; // nó anterior

    if (temp != NULL && temp->linha->linhaID == codigoLinha) {
        stockLoja->lista_linhas = (ListaLinhaProdutos *) temp->prox_linha;
        free(temp);
        return 0;
    }

    while (temp != NULL && temp->linha->linhaID != codigoLinha) {
        prev = temp;
        temp = (ListaLinhaProdutos *) temp->prox_linha;
    }
    if (temp == NULL) return 1;

    if (prev != NULL) {
        prev->prox_linha = temp->prox_linha;
    }

    free(temp);
    return 0;
}

// Atualiza uma linha de produtos no stock da loja
// Utiliza o stock da loja como pointer e a linha de produtos a atualizar como pointer
int atualizarLinhaProdutos(StockLoja* stockLoja, LinhaProdutos* linha) {
    ListaLinhaProdutos* current = stockLoja->lista_linhas;
    while (current != NULL) {
        if (current->linha->linhaID == linha->linhaID) {
            strncpy(current->linha->nome, linha->nome, sizeof(current->linha->nome));
            return 0;
        }
        current = (ListaLinhaProdutos *) current->prox_linha;
    }
    return 1;
}

// Retorna o número de linhas de produtos no stock da loja
unsigned int getNumeroLinhasProdutos(StockLoja* stockLoja) {
    return stockLoja->num_linhas;
}


// Retorna a lista de produtos a partir do stock da loja e do código da lista
ListaProdutos* obterListaProdutosPorIDProduto(StockLoja* stock, int codProduto) {
    ListaLinhaProdutos* current = stock->lista_linhas;
    while (current != NULL) {
        if (current->linha->lista_produtos->produto->listaID == codProduto) {
            return current->linha->lista_produtos;
        }
        current = (ListaLinhaProdutos *) current->prox_linha;
    }
    return NULL;
}

// Retorna a lista de produtos a partir do stock da loja e do código da linha
ListaProdutos* obterListaProdutosPorIDLinha(StockLoja* stock, int codLinha) {
    ListaLinhaProdutos* current = stock->lista_linhas;
    while (current != NULL) {
        if (current->linha->linhaID == codLinha) {
            return current->linha->lista_produtos;
        }
        current = (ListaLinhaProdutos *) current->prox_linha;
    }
    return NULL;
}

// Retorna a lista de produtos a partir do stock da loja e do nome da linha
ListaProdutos * obterListaProdutosPorNomeLinha(StockLoja* stock, char* nomeLinha) {
    ListaLinhaProdutos* current = stock->lista_linhas;
    while (current != NULL) {
        if (strcmp(current->linha->nome, nomeLinha) == 0) {
            return current->linha->lista_produtos;
        }
        current = (ListaLinhaProdutos *) current->prox_linha;
    }
    return NULL;
}

ListaProdutos* procurarStockPorNomeProduto(StockLoja* stock, char* nomeItem) {
    ListaProdutos* produtos = NULL;
    ListaLinhaProdutos* currentLinha = stock->lista_linhas;

    while (currentLinha != NULL) {
        ListaProdutos* currentProduto = currentLinha->linha->lista_produtos;
        while (currentProduto != NULL) {
            if (strcmp(currentProduto->produto->nome, nomeItem) == 0) {
                ListaProdutos* newProduto = (ListaProdutos*) malloc(sizeof(ListaProdutos));
                newProduto->produto = currentProduto->produto;
                newProduto->prox_produto = produtos;
                produtos = newProduto;
            }
            currentProduto = currentProduto->prox_produto;
        }
        currentLinha = currentLinha->prox_linha;
    }

    return produtos;
}

// Cria um novo produto usando os parâmetros fornecidos
// Retorna instância de produto
Produto criarProduto(char* nome, char* item, char* modelo, unsigned int quantidade, double preco, ListaParamAdicionalProduto* parametros) {
    Produto p;
    p.listaID = 0;
    p.linhaID = 0;
    p.produtoID = 0;
    strncpy(p.nome, nome, 50);
    strncpy(p.item, item, 50);
    strncpy(p.modelo, modelo, 50);
    p.quantidade = quantidade;
    p.preco = preco;
    p.num_parametros = getNumeroParametrosAdicionais(parametros);
    p.parametros = parametros;
    return p; // { 0, 0, 0, nome, item, modelo, quantidade, preco, parametros };
}

// Retorna um pointer de produto de uma linha de produtos com base no ID do produto
Produto* obterProdutoPorID(LinhaProdutos* linha, unsigned int IDProduto) {
    ListaProdutos* current = linha->lista_produtos;
    while (current != NULL) {
        if (current->produto->produtoID == IDProduto) {
            return current->produto;
        }
        current = (ListaProdutos *) current->prox_produto;
    }
    return NULL;
}

// Retorna um pointer de produto de uma linha de produtos com base no nome do produto
Produto* obterProdutoPorNome(LinhaProdutos* linha, char* nome, char* modelo) {
    ListaProdutos* current = linha->lista_produtos;
    while (current != NULL) {
        if (strcmp(current->produto->nome, nome) == 0 && strcmp(current->produto->modelo, modelo) == 0) {
            return current->produto;
        }
        current = (ListaProdutos *) current->prox_produto;
    }
    return NULL;
}


// Adiciona um produto a uma linha de produtos
// Utiliza a linha de produtos como pointer e o produto a adicionar como pointer
// Usa o método de push para adicionar um produto a lista dentro da linha de produtos
int adicionarProduto(LinhaProdutos* linha, Produto* produto) {
    ListaProdutos* novo_no = (ListaProdutos*) malloc(sizeof(ListaProdutos));
    if (novo_no == NULL) return 1;

    Produto* novo_produto = (Produto*) malloc(sizeof(Produto));
    if (novo_produto == NULL) {
        free(novo_no);
        return 1;
    }

    *novo_produto = *produto;
    novo_produto->produtoID = linha->num_produtos + 1;
    novo_produto->linhaID = linha->linhaID;
    novo_produto->num_parametros = getNumeroParametrosAdicionais(produto->parametros);

    novo_no->produto = novo_produto;
    novo_no->prox_produto = NULL;
    if (linha->lista_produtos != NULL) {
        novo_no->prox_produto = (struct ListaProdutos *) linha->lista_produtos;
        novo_no->produto->listaID = linha->lista_produtos->produto->listaID + 1; // Não causa problema se produto anterior tiver sido apagado (conflito IDs)
    }

    linha->lista_produtos = novo_no;
    linha->num_produtos++;

    return 0;
}
// Remove um produto de uma linha de produtos
// Usar metodos de pop para remover
int removerProduto(LinhaProdutos* linha, unsigned int IDproduto) {
    ListaProdutos* temp = linha->lista_produtos;
    ListaProdutos* prev = NULL;

    while (temp != NULL && temp->produto->produtoID != IDproduto) {
        prev = temp;
        temp = temp->prox_produto;
    }
    if (temp == NULL) return 0;

    if (prev == NULL) {
        linha->lista_produtos = temp->prox_produto;
    } else {
        prev->prox_produto = temp->prox_produto;
    }

    free(temp);
    return 1;
}

// Atualiza um produto numa linha de produtos com base no ID do produto fornecido
int atualizarProduto(LinhaProdutos* linha, Produto* produto) {
    ListaProdutos* current = linha->lista_produtos;
    while (current != NULL) {
        if (current->produto->produtoID == produto->produtoID) {
            strncpy(current->produto->nome, produto->nome, sizeof(current->produto->nome));
            strncpy(current->produto->item, produto->item, sizeof(current->produto->item));
            strncpy(current->produto->modelo, produto->modelo, sizeof(current->produto->modelo));
            current->produto->quantidade = produto->quantidade;
            current->produto->preco = produto->preco;

            ListaParamAdicionalProduto* current_param = current->produto->parametros;
            while (current_param != NULL) {
                ListaParamAdicionalProduto* next = current_param->prox_parametro;
                free(current_param);
                current_param = next;
            }
            current->produto->parametros = produto->parametros;

            return 0;
        }
        current = current->prox_produto;
    }
    return 1;
}


// Cria uma nova instância de lista de parâmetros adicionais de produtos
// Retorna a lista de parâmetros adicionais de produtos
ListaParamAdicionalProduto* criarListaParamAdicionaisProduto() {
    return NULL;
}

ParamAdicionalProduto* criarParametroAdicionalProduto(char* nome, char* valor) {
    ParamAdicionalProduto* parametro = (ParamAdicionalProduto*) malloc(sizeof(ParamAdicionalProduto));
    if (parametro == NULL) return NULL;
    parametro->id = 0;
    strncpy(parametro->nome, nome, MAX_CHAR);
    strncpy(parametro->valor, valor, MAX_CHAR);
    return parametro;
}

// Adiciona um parâmetro adicional a uma lista de parâmetros adicionais de produtos
// Utiliza a lista de parâmetros adicionais de produtos como pointer e o parâmetro a adicionar como pointer
// Usa o método de push para adicionar um parâmetro a lista de parâmetros adicionais de produtos
int adicionarParametroAdicionalProduto(Produto* produto, ParamAdicionalProduto* parametro) {
    ListaParamAdicionalProduto* novo_no = (ListaParamAdicionalProduto*) malloc(sizeof(ListaParamAdicionalProduto));
    if (novo_no == NULL) return 1;

    ParamAdicionalProduto* novo_parametro = (ParamAdicionalProduto*) malloc(sizeof(ParamAdicionalProduto));
    if (novo_parametro == NULL) {
        free(novo_no);
        return 1;
    }

    *novo_parametro = *parametro;
    novo_parametro->id = produto->num_parametros + 1;

    novo_no->parametro = novo_parametro;
    novo_no->prox_parametro = NULL;
    if (produto->parametros != NULL) {
        novo_no->prox_parametro = produto->parametros;
    }

    produto->parametros = novo_no;
    produto->num_parametros++;

    return 0;
}

// Remove um parâmetro adicional de uma lista de parâmetros adicionais de produtos
// Utiliza a lista de parâmetros adicionais de produtos como pointer e o código do parâmetro a remover
int removerParametroAdicionalProduto(Produto* produto, unsigned int codigoParametro) {
    ListaParamAdicionalProduto* temp = produto->parametros;
    ListaParamAdicionalProduto* prev = NULL;

    while (temp != NULL && temp->parametro->id != codigoParametro) {
        prev = temp;
        temp = temp->prox_parametro;
    }
    if (temp == NULL) return 0;

    if (prev == NULL) {
        produto->parametros = temp->prox_parametro;
    } else {
        prev->prox_parametro = temp->prox_parametro;
    }

    free(temp);
    return 1;
}

int atualizarParametroAdicional(Produto* produto, ParamAdicionalProduto* parametro) {
    ListaParamAdicionalProduto* current = produto->parametros;
    while (current != NULL) {
        if (current->parametro->id == parametro->id) {
            strncpy(current->parametro->nome, parametro->nome, sizeof(current->parametro->nome));
            strncpy(current->parametro->valor, parametro->valor, sizeof(current->parametro->valor));
            return 0;
        }
        current = current->prox_parametro;
    }
    return 1;
}

int adicionarParametroAdicionalLista(ListaParamAdicionalProduto** lista, ParamAdicionalProduto* parametro) {
    ListaParamAdicionalProduto* novo_no = (ListaParamAdicionalProduto*) malloc(sizeof(ListaParamAdicionalProduto));
    if (novo_no == NULL) {
        return 1;
    }

    ParamAdicionalProduto* novo_parametro = (ParamAdicionalProduto*) malloc(sizeof(ParamAdicionalProduto));
    if (novo_parametro == NULL) {
        free(novo_no);
        return 1;
    }

    *novo_parametro = *parametro;
    novo_parametro->id = getNumeroParametrosAdicionais(*lista) + 1;

    novo_no->parametro = novo_parametro;
    novo_no->prox_parametro = *lista;
    *lista = novo_no;

    return 0;
}


ParamAdicionalProduto* obterParametroAdicionalPorID(Produto* produto, unsigned int codigoParametro) {
    ListaParamAdicionalProduto* current = produto->parametros;
    while (current != NULL) {
        if (current->parametro->id == codigoParametro) {
            return current->parametro;
        }
        current = current->prox_parametro;
    }
    return NULL;
}

ParamAdicionalProduto* obterParametroAdicionalPorNome(Produto* produto, char* nomeParametro) {
    ListaParamAdicionalProduto* current = produto->parametros;
    while (current != NULL) {
        if (strcmp(current->parametro->nome, nomeParametro) == 0) {
            return current->parametro;
        }
        current = current->prox_parametro;
    }
    return NULL;
}

unsigned int getNumeroParametrosAdicionais(ListaParamAdicionalProduto* lista) {
    unsigned int count = 0;
    ListaParamAdicionalProduto* current = lista;
    while (current != NULL) {
        count++;
        current = current->prox_parametro;
    }
    return count;
}


unsigned int getNumeroLinhasStock(StockLoja* stock) {
    return stock->num_linhas;
}

// Retorna o número de produtos numa linha de produtos vendo quantos produtos existem em cada lista
unsigned int getNumeroProdutosLinha(LinhaProdutos* linha) {
    unsigned int count = 0;
    ListaProdutos* current = linha->lista_produtos;
    while (current != NULL) {
        count++;
        current = (ListaProdutos *) current->prox_produto;
    }
    return count;
}

// Retorna o número de produtos total no stock da loja
unsigned int getNumeroProdutosStock(StockLoja* stock) {
    unsigned int count = 0;
    ListaLinhaProdutos* current = stock->lista_linhas;
    while (current != NULL) {
        count += getNumeroProdutosLinha(current->linha);
        current = (ListaLinhaProdutos *) current->prox_linha;
    }
    return count;
}

// Dá merge a duas listas de produtos (usado por mergeSort para getNumeroTipoProdutosStock)
ListaProdutos* sortedMerge(ListaProdutos* a, ListaProdutos* b) {
    ListaProdutos* result = NULL;

    // casos base
    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    // escolher entre a ou b e retornar recursivamente
    if (strcmp(a->produto->nome, b->produto->nome) <= 0) {
        result = a;
        result->prox_produto = sortedMerge(a->prox_produto, b);
    } else {
        result = b;
        result->prox_produto = sortedMerge(a, b->prox_produto);
    }

    return result;
}

// Função para dividir a lista de produtos em duas e partir dai
void split(ListaProdutos* fonte, ListaProdutos** frontRef, ListaProdutos** backRef) {
    ListaProdutos* rapido;
    ListaProdutos* lento;
    lento = fonte;
    rapido = fonte->prox_produto;

    while (rapido != NULL) {
        rapido = rapido->prox_produto;
        if (rapido != NULL) {
            lento = lento->prox_produto;
            rapido = rapido->prox_produto;
        }
    }

    *frontRef = fonte;
    *backRef = lento->prox_produto;
    lento->prox_produto = NULL;
}

// Função para ordenar a lista de produtos usando o merge sort
void mergeSort(ListaProdutos** headRef) {
    ListaProdutos* head = *headRef;
    ListaProdutos* a;
    ListaProdutos* b;

    if ((head == NULL) || (head->prox_produto == NULL)) {
        return;
    }

    split(head, &a, &b);

    // Recursivamente ordenar as sublistas
    mergeSort(&a);
    mergeSort(&b);

    *headRef = sortedMerge(a, b);
}

// Nó de tipo para contar tipos de produtos (Lista ligada)
typedef struct TipoNode {
    char tipo[MAX_CHAR];
    struct TipoNode* next;
} TipoNode;

unsigned int getNumeroTipoProdutosStock(StockLoja* stock) {
    unsigned int contagem = 0;
    TipoNode* tiposContados = NULL;

    ListaLinhaProdutos* linhaAtual = stock->lista_linhas;
    while (linhaAtual != NULL) {
        ListaProdutos* produtoAtual = linhaAtual->linha->lista_produtos;
        while (produtoAtual != NULL) {
            TipoNode* tipoAtual = tiposContados;
            while (tipoAtual != NULL) {
                if (strcmp(tipoAtual->tipo, produtoAtual->produto->nome) == 0) {
                    break;
                }
                tipoAtual = tipoAtual->next;
            }
            if (tipoAtual == NULL) {
                TipoNode* novoTipo = (TipoNode*) malloc(sizeof(TipoNode));
                if (novoTipo == NULL) {
                    TipoNode* tipoAtualNodes = tiposContados;
                    while (tipoAtualNodes != NULL) {
                        TipoNode* nextType = tipoAtualNodes->next;
                        free(tipoAtualNodes);
                        tipoAtualNodes = nextType;
                    }
                    return contagem;
                }
                strncpy(novoTipo->tipo, produtoAtual->produto->nome, MAX_CHAR);
                novoTipo->next = tiposContados;
                tiposContados = novoTipo;
                contagem++;
            }
            produtoAtual = produtoAtual->prox_produto;
        }
        linhaAtual = linhaAtual->prox_linha;
    }

    return contagem;
}

