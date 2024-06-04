//
// Created by Gonçalo Miranda on 03/06/2024.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "storage.h"
#include "produtos.h"
#include "cJSON.h"

static char* read_file(const char* filename);
static void parse_parametros(Produto* produto, cJSON* parametros_json);
static void parse_produtos(LinhaProdutos* linha, cJSON* produtos_json);
static void parse_linhas(StockLoja* stock, cJSON* linhas_json);

void guardarStock(StockLoja* stock, const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo %s\n", nome_arquivo);
        return;
    }

    fprintf(arquivo, "{\n");
    fprintf(arquivo, "  \"nome\": \"%s\",\n", stock->nome);
    fprintf(arquivo, "  \"num_linhas\": %u,\n", stock->num_linhas);
    fprintf(arquivo, "  \"lista_linhas\": [\n");

    ListaLinhaProdutos* linhaAtual = stock->lista_linhas;
    while (linhaAtual != NULL) {
        LinhaProdutos *linha = linhaAtual->linha;

        fprintf(arquivo, "    {\n");
        fprintf(arquivo, "      \"linhaID\": %u,\n", linha->linhaID);
        fprintf(arquivo, "      \"nome\": \"%s\",\n", linha->nome);
        fprintf(arquivo, "      \"num_produtos\": %u,\n", linha->num_produtos);
        fprintf(arquivo, "      \"lista_produtos\": [");

        if (linha->lista_produtos == NULL) {
            fprintf(arquivo, "]\n");
        } else {
            fprintf(arquivo, "\n");
        }

        if (linha->lista_produtos != NULL) {
            ListaProdutos *produtoAtual = linha->lista_produtos;
            while (produtoAtual != NULL) {
                Produto *produto = produtoAtual->produto;

                fprintf(arquivo, "        {\n");
                fprintf(arquivo, "          \"linhaID\": %u,\n", linha->linhaID);
                fprintf(arquivo, "          \"listaID\": %u,\n", produto->listaID);
                fprintf(arquivo, "          \"produtoID\": %u,\n", produto->produtoID);
                fprintf(arquivo, "          \"nome\": \"%s\",\n", produto->nome);
                fprintf(arquivo, "          \"item\": \"%s\",\n", produto->item);
                fprintf(arquivo, "          \"modelo\": \"%s\",\n", produto->modelo);
                fprintf(arquivo, "          \"quantidade\": %u,\n", produto->quantidade);
                fprintf(arquivo, "          \"preco\": %f,\n", produto->preco);
                fprintf(arquivo, "          \"num_parametros\": %u,\n", produto->num_parametros);
                fprintf(arquivo, "          \"parametros\": [");

                if (produto->parametros == NULL) {
                    fprintf(arquivo, "]\n");
                } else {
                    fprintf(arquivo, "\n");
                }

                ListaParamAdicionalProduto *parametroAtual = produto->parametros;
                while (parametroAtual != NULL) {
                    ParamAdicionalProduto *parametro = parametroAtual->parametro;
                    fprintf(arquivo, "            {\n");
                    fprintf(arquivo, "              \"id\": %u,\n", parametro->id);
                    fprintf(arquivo, "              \"nome\": \"%s\",\n", parametro->nome);
                    fprintf(arquivo, "              \"valor\": \"%s\"\n", parametro->valor);
                    fprintf(arquivo, "            }");

                    parametroAtual = parametroAtual->prox_parametro;
                    if (parametroAtual != NULL) {
                        fprintf(arquivo, ",");
                    }
                    fprintf(arquivo, "\n");
                }
                if (produto->parametros != NULL) {
                    fprintf(arquivo, "          ]\n");
                }
                fprintf(arquivo, "        }");

                produtoAtual = produtoAtual->prox_produto;
                if (produtoAtual != NULL) {
                    fprintf(arquivo, ",");
                }
                fprintf(arquivo, "\n");
            }
            fprintf(arquivo, "      ]\n");
        }
        fprintf(arquivo, "    }");

        linhaAtual = linhaAtual->prox_linha;
        if (linhaAtual != NULL) {
            fprintf(arquivo, ",");
        }
        fprintf(arquivo, "\n");
    }

    fprintf(arquivo, "  ]\n");
    fprintf(arquivo, "}\n");

    fclose(arquivo);
    printf("Stock guardado\n");
}

void removeNewlineJSON(const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo %s\n", nome_arquivo);
        return;
    }

    FILE* arquivo_temp = fopen("temp.json", "w");
    if (arquivo_temp == NULL) {
        printf("Não foi possível abrir o arquivo temp.json\n");
        return;
    }

    char c;
    int inside_string = 0;
    while ((c = (char) fgetc(arquivo)) != EOF) {
        if (c == '\"') {
            inside_string = !inside_string;
        }
        if (c != '\n' && (inside_string || c != ' ')) {
            fputc(c, arquivo_temp);
        }
    }

    fclose(arquivo);
    fclose(arquivo_temp);

    remove(nome_arquivo);
    rename("temp.json", nome_arquivo);
}

void carregarStock(StockLoja* stock, const char* nome_arquivo) {
    removeNewlineJSON(nome_arquivo);

    char* json = read_file(nome_arquivo);
    if (json == NULL) {
        printf("Não foi possível ler o arquivo %s\n", nome_arquivo);
        return;
    }

    cJSON* root = cJSON_Parse(json);
    if (root == NULL) {
        printf("Falha ao parsear JSON\n");
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("Erro antes de: %s\n", error_ptr);
        }
        free(json);
        return;
    }

    cJSON* nome = cJSON_GetObjectItem(root, "nome");
    cJSON* num_linhas = cJSON_GetObjectItem(root, "num_linhas");
    cJSON* lista_linhas = cJSON_GetObjectItem(root, "lista_linhas");

    if (cJSON_IsString(nome) && (nome->valuestring != NULL)) {
        strncpy(stock->nome, nome->valuestring, sizeof(stock->nome));
        stock->nome[sizeof(stock->nome) - 1] = '\0';
    }

    if (cJSON_IsNumber(num_linhas)) {
        stock->num_linhas = num_linhas->valueint;
    }

    if (cJSON_IsArray(lista_linhas)) {
        parse_linhas(stock, lista_linhas);
    }

    cJSON_Delete(root);
    free(json);
    printf("Stock carregado\n");
}

static char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* content = malloc(length + 1);
    if (content) {
        fread(content, 1, length, file);
        content[length] = '\0';
    }
    fclose(file);
    return content;
}

static void parse_parametros(Produto* produto, cJSON* parametros_json) {
    int param_count = cJSON_GetArraySize(parametros_json);
    ListaParamAdicionalProduto* last_param = NULL;

    for (int i = 0; i < param_count; i++) {
        cJSON* param_json = cJSON_GetArrayItem(parametros_json, i);
        if (!param_json) continue;

        ParamAdicionalProduto* param = malloc(sizeof(ParamAdicionalProduto));
        ListaParamAdicionalProduto* param_node = malloc(sizeof(ListaParamAdicionalProduto));
        param_node->parametro = param;
        param_node->prox_parametro = NULL;

        if (last_param == NULL) {
            produto->parametros = param_node;
        } else {
            last_param->prox_parametro = param_node;
        }
        last_param = param_node;

        cJSON* id = cJSON_GetObjectItem(param_json, "id");
        cJSON* nome = cJSON_GetObjectItem(param_json, "nome");
        cJSON* valor = cJSON_GetObjectItem(param_json, "valor");

        if (cJSON_IsNumber(id)) {
            param->id = id->valueint;
        }
        if (cJSON_IsString(nome) && (nome->valuestring != NULL)) {
            strncpy(param->nome, nome->valuestring, sizeof(param->nome));
            param->nome[sizeof(param->nome) - 1] = '\0';
        }
        if (cJSON_IsString(valor) && (valor->valuestring != NULL)) {
            strncpy(param->valor, valor->valuestring, sizeof(param->valor));
            param->valor[sizeof(param->valor) - 1] = '\0';
        }
    }
}

static void parse_produtos(LinhaProdutos* linha, cJSON* produtos_json) {
    int prod_count = cJSON_GetArraySize(produtos_json);
    ListaProdutos* last_prod = NULL;

    for (int i = 0; i < prod_count; i++) {
        cJSON* prod_json = cJSON_GetArrayItem(produtos_json, i);
        if (!prod_json) continue;

        Produto* produto = malloc(sizeof(Produto));
        ListaProdutos* prod_node = malloc(sizeof(ListaProdutos));
        prod_node->produto = produto;
        prod_node->prox_produto = NULL;

        if (last_prod == NULL) {
            linha->lista_produtos = prod_node;
        } else {
            last_prod->prox_produto = prod_node;
        }
        last_prod = prod_node;

        cJSON* linhaID = cJSON_GetObjectItem(prod_json, "linhaID");
        cJSON* listaID = cJSON_GetObjectItem(prod_json, "listaID");
        cJSON* produtoID = cJSON_GetObjectItem(prod_json, "produtoID");
        cJSON* nome = cJSON_GetObjectItem(prod_json, "nome");
        cJSON* item = cJSON_GetObjectItem(prod_json, "item");
        cJSON* modelo = cJSON_GetObjectItem(prod_json, "modelo");
        cJSON* quantidade = cJSON_GetObjectItem(prod_json, "quantidade");
        cJSON* preco = cJSON_GetObjectItem(prod_json, "preco");
        cJSON* num_parametros = cJSON_GetObjectItem(prod_json, "num_parametros");
        cJSON* parametros = cJSON_GetObjectItem(prod_json, "parametros");

        if (cJSON_IsNumber(linhaID)) {
            produto->linhaID = linhaID->valueint;
        }
        if (cJSON_IsNumber(listaID)) {
            produto->listaID = listaID->valueint;
        }
        if (cJSON_IsNumber(produtoID)) {
            produto->produtoID = produtoID->valueint;
        }
        if (cJSON_IsString(nome) && (nome->valuestring != NULL)) {
            strncpy(produto->nome, nome->valuestring, sizeof(produto->nome));
            produto->nome[sizeof(produto->nome) - 1] = '\0';
        }
        if (cJSON_IsString(item) && (item->valuestring != NULL)) {
            strncpy(produto->item, item->valuestring, sizeof(produto->item));
            produto->item[sizeof(produto->item) - 1] = '\0';
        }
        if (cJSON_IsString(modelo) && (modelo->valuestring != NULL)) {
            strncpy(produto->modelo, modelo->valuestring, sizeof(produto->modelo));
            produto->modelo[sizeof(produto->modelo) - 1] = '\0';
        }
        if (cJSON_IsNumber(quantidade)) {
            produto->quantidade = quantidade->valueint;
        }
        if (cJSON_IsNumber(preco)) {
            produto->preco = preco->valuedouble;
        }
        if (cJSON_IsNumber(num_parametros)) {
            produto->num_parametros = num_parametros->valueint;
        }
        if (cJSON_IsArray(parametros)) {
            parse_parametros(produto, parametros);
        }
    }
}

static void parse_linhas(StockLoja* stock, cJSON* linhas_json) {
    int linha_count = cJSON_GetArraySize(linhas_json);
    ListaLinhaProdutos* last_linha = NULL;

    for (int i = 0; i < linha_count; i++) {
        cJSON* linha_json = cJSON_GetArrayItem(linhas_json, i);
        if (!linha_json) continue;

        LinhaProdutos* linha = malloc(sizeof(LinhaProdutos));
        ListaLinhaProdutos* linha_node = malloc(sizeof(ListaLinhaProdutos));
        linha_node->linha = linha;
        linha_node->prox_linha = NULL;

        if (last_linha == NULL) {
            stock->lista_linhas = linha_node;
        } else {
            last_linha->prox_linha = linha_node;
        }
        last_linha = linha_node;

        cJSON* linhaID = cJSON_GetObjectItem(linha_json, "linhaID");
        cJSON* nome = cJSON_GetObjectItem(linha_json, "nome");
        cJSON* num_produtos = cJSON_GetObjectItem(linha_json, "num_produtos");
        cJSON* lista_produtos = cJSON_GetObjectItem(linha_json, "lista_produtos");

        if (cJSON_IsNumber(linhaID)) {
            linha->linhaID = linhaID->valueint;
        }
        if (cJSON_IsString(nome) && (nome->valuestring != NULL)) {
            strncpy(linha->nome, nome->valuestring, sizeof(linha->nome));
            linha->nome[sizeof(linha->nome) - 1] = '\0';
        }
        if (cJSON_IsNumber(num_produtos)) {
            linha->num_produtos = num_produtos->valueint;
        }
        if (cJSON_IsArray(lista_produtos)) {
            parse_produtos(linha, lista_produtos);
        }
    }
}
