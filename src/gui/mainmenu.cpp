#include <QMessageBox>
#include <QFile>
#include <QStringListModel>
#include <QFileDialog>
#include "mainmenu.h"
#include "ui_MainMenu.h"

StockLoja *stock;
int stockSaved = 0;
int selLinha = 0, selProduto = 0, selModelo = 0;

void MainMenu::newDB() {
    char *stockName = static_cast<char *>(malloc(1));
    stock = criarStockLoja(stockName, 1);

    setStock(stock);
    reloadTabs();
    ui->statusbar->showMessage("Nova base de dados criada com sucesso!");
}

void MainMenu::openDB() {
    QString filename = QFileDialog::getOpenFileName(this, "Abrir base de dados", "", "JSON Files (*.json)");
    if (filename.isEmpty()) return;

    char *stockName = static_cast<char *>(malloc(1));
    stock = criarStockLoja(stockName, 1);

    int resultado = carregarStock(stock, filename.toStdString().c_str());
    if (resultado == 1) {
        criarWarningMessageBox("ERRO!!", "Erro ao abrir a base de dados.", 0);
        return;
    } else if (resultado == -1) {
        criarWarningMessageBox("ERRO!!", "Não foi possivel ler a base de dados.", 0);
        return;
    }

    setWindowTitle("Gestão de Stock - " + QString(stock->nome));
    setStock(stock);
    reloadTabs();

    ui->statusbar->showMessage("Base de dados aberta com sucesso!");
}

void MainMenu::saveDB() {
    int res = criarWarningMessageBox("Atenção", "Deseja guardar as alterações?", 1);
    if (res == QMessageBox::Yes) {
        QString filename = QFileDialog::getSaveFileName(this, "Guardar base de dados", "", "JSON Files (*.json)");
        if (filename.isEmpty()) return;

        int resultado = guardarStock(stock, filename.toStdString().c_str());
        if (resultado == 1) {
            criarWarningMessageBox("ERRO!!", "Não foi possivel guardar a base de dados.", 0);
            return;
        }

        stockSaved = 1;
        statusBar()->showMessage("Stock guardado com sucesso!");
    }
}

void MainMenu::closeDB() {
    int res = criarWarningMessageBox("Atenção", "Deseja fechar a base de dados?", 1);
    if (res == QMessageBox::Yes) {
        stock = nullptr;
        setWindowTitle("Gestão de Stock");
        ui->nomeLojaLab->setText("Loja sem nome");
        setLinhas(nullptr);
        setProdutos(nullptr);
        setModelos(nullptr);
        setModeloInfo(nullptr);
        reloadTabs();
        ui->statusbar->showMessage("Base de dados fechada com sucesso!");
    }
}

void MainMenu::quit() {
    int res = criarWarningMessageBox("Atenção", "Deseja sair da aplicação?", 1);
    if (res == QMessageBox::Yes) {
        if (stock && !stockSaved) {
            int res2 = criarWarningMessageBox("Atenção", "Deseja guardar as alterações antes de sair?", 1);
            if (res2 == QMessageBox::Yes) saveDB();
        }
        QApplication::quit();
    }
}

MainMenu::MainMenu(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainMenu) {
    ui->setupUi(this);

    QAction* actions[] = {
            new QAction(tr("Criar nova base de dados"), this),
            new QAction(tr("Abrir base de dados"), this),
            new QAction(tr("Salvar para base de dados"), this),
            new QAction(tr("Fechar base de dados"), this),
            new QAction(tr("Sair"), this)
    };
    actions[0]->setShortcut(QKeySequence::New);
    actions[1]->setShortcut(QKeySequence::Open);
    actions[2]->setShortcut(QKeySequence::Save);
    actions[3]->setShortcut(QKeySequence::Close);
    actions[4]->setShortcut(QKeySequence::Quit);

    connect(actions[0], &QAction::triggered, this, &MainMenu::newDB);
    connect(actions[1], &QAction::triggered, this, &MainMenu::openDB);
    connect(actions[2], &QAction::triggered, this, &MainMenu::saveDB);
    connect(actions[3], &QAction::triggered, this, &MainMenu::closeDB);
    connect(actions[4], &QAction::triggered, this, &MainMenu::quit);

    ui->menuFicheiro->addActions({actions[0], actions[1], actions[2], actions[3]});
    ui->menuFicheiro->addSeparator();
    ui->menuFicheiro->addAction(actions[4]);

    auto listViews = {ui->linhasLV, ui->produtosLV, ui->modelosLV};
    for (auto lv : listViews) lv->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->linhasLV, &QListView::clicked, this, &MainMenu::onLinhasLVClicked);
    connect(ui->produtosLV, &QListView::clicked, this, &MainMenu::onProdutosLVClicked);
    connect(ui->modelosLV, &QListView::clicked, this, &MainMenu::onModelosLVClicked);

    auto buttons = {
        std::make_pair(ui->adMarcaBtn, &MainMenu::onAdicionarLinhaBtnClicked),
        std::make_pair(ui->delMarcaBtn, &MainMenu::onRemoverLinhaBtnClicked),
        std::make_pair(ui->edMarcaBtn, &MainMenu::onAtualizarLinhaBtnClicked),
        std::make_pair(ui->adModeloBtn, &MainMenu::onAdicionarProdutoBtnClicked),
        std::make_pair(ui->delModeloBtn, &MainMenu::onRemoverProdutoBtnClicked),
        std::make_pair(ui->edModeloBtn, &MainMenu::onAtualizarProdutoBtnClicked),
        std::make_pair(ui->nomeLojaModBtn, &MainMenu::onNomeLojaModBtnClicked),
        std::make_pair(ui->atualizarStockInfoBtn, &MainMenu::onAtualizarStockInfoBtnClicked),
        std::make_pair(ui->apagaLinhasStockBtn, &MainMenu::onApagaLinhasStockBtnClicked),
        std::make_pair(ui->apagaProdutosStockBtn, &MainMenu::onApagaProdutosStockBtnClicked),
        std::make_pair(ui->procuraBtn, &MainMenu::onProcuraBtnClicked)
    };
    for (auto [button, slot] : buttons) connect(button, &QPushButton::clicked, this, slot);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainMenu::onTabChanged);

    if (!QFile::exists("logo.png")) {
        statusBar()->showMessage("Logo não encontrado. Aplicação funcionará sem o logo.");
        ui->logoLoja->setText("N/D");
    } else {
        QPixmap logo("logo.png");
        ui->logoLoja->setPixmap(logo);
        ui->logoLoja->setScaledContents(true);
    }

    statusBar()->showMessage("Pronto");
}

MainMenu::~MainMenu() {
    delete ui;
}

void MainMenu::setStock(StockLoja *ptr) {
    stock = ptr;
    if (strlen(stock->nome) == 0) {
        ui->nomeLojaLab->setText("Loja sem nome");
        return;
    } else {
        ui->nomeLojaLab->setText(stock->nome);
    }
}

void MainMenu::setLinhas(StockLoja *ptr) {
    if (!ptr) {
        ui->linhasLV->setModel(nullptr);
        setProdutos(nullptr);
        setModelos(nullptr);
        setModeloInfo(nullptr);
        return;
    }

    QStringList list;
    for (ListaLinhaProdutos *current = ptr->lista_linhas; current; current = current->prox_linha)
        list << current->linha->nome;
    std::reverse(list.begin(), list.end());

    auto *model = new QStringListModel(this);
    model->setStringList(list);
    ui->linhasLV->setModel(model);

    if (!list.empty() && ui->linhasLV->selectionModel())
        connect(ui->linhasLV->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainMenu::onLinhasLVClicked);
}

void MainMenu::setProdutos(LinhaProdutos *ptr) {
    if (ptr == nullptr) {
        ui->produtosLV->setModel(nullptr);
        setModelos(nullptr);
        setModeloInfo(nullptr);
        return;
    }

    auto *model = new QStringListModel(this);
    QStringList list;
    QSet<QString> uniqueNames;

    ListaProdutos *current = ptr->lista_produtos;
    while (current != nullptr) {
        Produto produto = *current->produto;
        QString produtoStr = produto.nome;
        if (!uniqueNames.contains(produtoStr)) {
            list << produtoStr;
            uniqueNames.insert(produtoStr);
        }
        current = (ListaProdutos *) current->prox_produto;
    }
    std::sort(list.begin(), list.end());
    model->setStringList(list);
    ui->produtosLV->setModel(model);

    if (list.empty()) return;
    if (ui->modelosLV->selectionModel() == nullptr) return;

    connect(ui->modelosLV->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainMenu::onProdutosLVClicked);
}


void MainMenu::setModelos(ListaProdutos *ptr) {
    if (!ptr) {
        ui->modelosLV->setModel(nullptr);
        setModeloInfo(nullptr);
        return;
    }

    QStringList list;
    for (ListaProdutos *current = ptr; current; current = current->prox_produto)
        list << current->produto->modelo;

    std::sort(list.begin(), list.end());
    auto *model = new QStringListModel(this);
    model->setStringList(list);
    ui->modelosLV->setModel(model);

    if (!list.empty() && ui->modelosLV->selectionModel())
        connect(ui->modelosLV->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainMenu::onModelosLVClicked);
}

void MainMenu::setModeloInfo(Produto *ptr) {
    auto clearModelInfo = [this]() {
        ui->codigoModeloText->clear();
        ui->nomeModeloText->clear();
        ui->quantModeloText->clear();
        ui->precoModeloText->clear();
        ui->outrosParamTV->clearContents();
        ui->outrosParamTV->setRowCount(0);
        ui->outrosParamTV->setColumnCount(2);
        ui->outrosParamTV->setHorizontalHeaderLabels(QStringList() << "Nome" << "Valor");
    };

    if (!ptr) {
        clearModelInfo();
        return;
    }

    QString fullNome = QString::fromStdString(ptr->nome) + " " + QString::fromStdString(ptr->modelo);
    ui->codigoModeloText->setText(QString::number(ptr->linhaID) + "." + QString::number(ptr->produtoID));
    ui->nomeModeloText->setText(fullNome);
    ui->quantModeloText->setText(QString::number(ptr->quantidade));
    ui->precoModeloText->setText(QString::number(ptr->preco).replace('.',',') + " €");
    ui->outrosParamTV->clearContents();
    ui->outrosParamTV->setRowCount(static_cast<int>(ptr->num_parametros));
    ui->outrosParamTV->setColumnCount(2);
    ui->outrosParamTV->setHorizontalHeaderLabels(QStringList() << "Nome" << "Valor");

    ListaParamAdicionalProduto* current = ptr->parametros;
    int numparametros = static_cast<int>(getNumeroParametrosAdicionais(current));

    // do parameters backwards
    for (int i = numparametros - 1; i >= 0; i--) {
        ParamAdicionalProduto param = *current->parametro;
        ui->outrosParamTV->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(param.nome)));
        ui->outrosParamTV->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(param.valor)));
        current = current->prox_parametro;
    }
}

void MainMenu::tabDefinicoes() {
    if (stock == nullptr) {
        ui->nomeLojaModText->clear();
        ui->numLinhasStockText->clear();
        ui->numTiposStockText->clear();
        ui->numProdutosStockText->clear();
        return;
    }
    ui->nomeLojaModText->setText(ui->nomeLojaLab->text());
    ui->numLinhasStockText->setText(QString::number(stock->num_linhas));
    ui->numTiposStockText->setText(QString::number(getNumeroTipoProdutosStock(stock)));
    ui->numProdutosStockText->setText(QString::number(getNumeroProdutosStock(stock)));
}

// Função para recarregar as tabs (dados estáticos) - para quando se muda de tab
void MainMenu::reloadTabs() {
    if (ui->tabWidget->currentIndex() == 0) { // stock
        setLinhas(stock);
        setProdutos(nullptr);
        setModelos(nullptr);
        setModeloInfo(nullptr);
    } else if (ui->tabWidget->currentIndex() == 1) { // pesquisa
        ui->procuraTE->clear();
        ui->resultPesqTV->clear();
        ui->resultPesqTV->setRowCount(0);
        ui->resultPesqTV->setColumnCount(8);
        ui->resultPesqTV->setHorizontalHeaderLabels(QStringList() << "Código Interno" << "Linha" << "Nome" << "Modelo" << "Categoria" << "Quantidade" << "Preço" << "Número de parâmetros");
        for (int j = 0; j < 8; j++) {
            ui->resultPesqTV->resizeColumnToContents(j);
        }
    } else if (ui->tabWidget->currentIndex() == 2) { // definições
        tabDefinicoes();
    }
}

// Função para mostrar os produtos na segunda ListView baseado na linha de produtos selecionada na primeira ListView
void MainMenu::onLinhasLVClicked(const QModelIndex &index) {
    selLinha = index.row();
    auto linha = (LinhaProdutos *) obterLinhaProdutosPorID(stock, selLinha + 1);
    setProdutos(linha);
    setModelos(nullptr);
    setModeloInfo(nullptr);
}

// Função para mostrar os modelos na terceira ListView baseado no produto selecionado na segunda ListView
void MainMenu::onProdutosLVClicked(const QModelIndex &index) {
    selProduto = index.row();
    QModelIndex indexLinha = ui->linhasLV->currentIndex();

    QAbstractItemModel* model = ui->produtosLV->model();
    QString produtoText = model->data(index, Qt::DisplayRole).toString();
    std::string produtoStd = produtoText.toStdString();
    char* produto = const_cast<char*>(produtoStd.c_str());

    auto linhaText = ui->linhasLV->model()->data(indexLinha, Qt::DisplayRole).toString();
    std::string linhaTextStd = linhaText.toStdString();
    char* nomeLinha = const_cast<char*>(linhaTextStd.c_str());

    auto linha = (LinhaProdutos *) obterLinhaProdutosPorNome(stock, nomeLinha);
    auto lista = (ListaProdutos *) obterListaProdutosPorItemLinha(linha, produto);

    setModelos(lista);
    setModeloInfo(nullptr);
}

// Função para mostrar a informação de um modelo na secção de informação abaixo da ListView
void MainMenu::onModelosLVClicked(const QModelIndex &index) {
    selModelo = index.row();

    auto nomeLinha = ui->linhasLV->model()->data(ui->linhasLV->currentIndex(), Qt::DisplayRole).toString();
    std::string nomeLinhaStd = nomeLinha.toStdString();
    char* nomeLinhaChar = const_cast<char*>(nomeLinhaStd.c_str());

    QModelIndex indexProduto = ui->produtosLV->model()->index(selProduto, 0);
    auto itemProd = ui->produtosLV->model()->data(indexProduto, Qt::DisplayRole).toString();
    auto modeloProd = ui->modelosLV->model()->data(index, Qt::DisplayRole).toString();
    std::string nomeStd = itemProd.toStdString();
    std::string modeloStd = modeloProd.toStdString();
    char* nome = const_cast<char*>(nomeStd.c_str());
    char* modelo = const_cast<char*>(modeloStd.c_str());

    auto produto = (Produto *) obterProdutoPorNome(obterLinhaProdutosPorNome(stock, nomeLinhaChar), nome, modelo);
    setModeloInfo(produto);

    if (produto == nullptr) ui->statusbar->showMessage("ERRO!! - Produto não encontrado!");
}

void MainMenu::onAdicionarLinhaBtnClicked() {
    LinhaDialog dialog;
    dialog.setNomeJanela(0,"");
    dialog.show();
    dialog.exec();

    if (dialog.result() == QDialog::Accepted) {
        // check for all strings length < 50
        if (dialog.getNome().length() > 50) {
            criarWarningMessageBox("ERRO!!", "Nome da linha de produtos não pode ter mais de 50 caracteres.", 0);
            return;
        }

        std::string nome = dialog.getNome().toStdString();
        char* nomeChar = new char[nome.length() + 1];
        std::strcpy(nomeChar, nome.c_str());

        LinhaProdutos linha = criarLinhaProdutos(nomeChar);
        int resultado = adicionarLinhaProdutos(stock, linha);
        delete[] nomeChar;

        if (resultado == 2) { // linha já existe
            criarWarningMessageBox("ERRO!!", "Linha de produtos já existe.", 0);
            return;
        } else if (resultado == 1) { // malloc problemas
            criarWarningMessageBox("ERRO!!", "Problemas ao adicionar linha de produtos.", 0);
            return;
        } else if (resultado == -1) { // nome vazio
            criarWarningMessageBox("ERRO!!", "Nome da linha não pode ser vazio.", 0);
            return;
        }

        setLinhas(stock);
        setProdutos(nullptr);
        setModelos(nullptr);
        setModeloInfo(nullptr);
        ui->statusbar->showMessage("Sucesso - Linha de produtos adicionada com sucesso!");
    }
}

// Função para remover uma linha de produtos
void MainMenu::onRemoverLinhaBtnClicked() {
    if (ui->linhasLV->currentIndex().row() == -1) {
        criarWarningMessageBox("ERRO!!", "Selecione uma linha de produtos para remover.", 0);
        return;
    }

    auto nomeLinha = ui->linhasLV->model()->data(ui->linhasLV->currentIndex(), Qt::DisplayRole).toString();
    std::string nomeStd = nomeLinha.toStdString();
    char* nome = const_cast<char*>(nomeStd.c_str());

    // procurar por linha se existe
    LinhaProdutos *linha = obterLinhaProdutosPorNome(stock, nome);
    if (linha == nullptr) {
        criarWarningMessageBox("ERRO!!", "Linha de produtos não encontrada.", 0);
        return;
    }

    int ret = criarWarningMessageBox("Confirmação", "Tem a certeza que deseja remover a linha de produtos \"" + nomeLinha + "\"?", 1);
    if (ret == QMessageBox::No) return;

    int resultado = removerLinhaProdutos(stock, linha->linhaID);
    if (resultado == 2) {
        criarWarningMessageBox("ERRO!!", "Linha de produtos não encontrada.", 0);
        return;
    } else if (resultado == 1) {
        criarWarningMessageBox("ERRO!!", "Linha de produtos não foi removida porque o código da linha é inválido.", 0);
        return;
    }

    setLinhas(stock);
    setProdutos(nullptr);
    setModelos(nullptr);
    setModeloInfo(nullptr);
    ui->statusbar->showMessage("Linha de produtos removida com sucesso!");
}

// Função para atualizar uma linha de produtos
void MainMenu::onAtualizarLinhaBtnClicked() {
    if (ui->linhasLV->currentIndex().row() == -1) {
        criarWarningMessageBox("ERRO!!", "Selecione uma linha de produtos para atualizar.", 0);
        return;
    }

    auto nomeLinha = ui->linhasLV->model()->data(ui->linhasLV->currentIndex(), Qt::DisplayRole).toString();
    std::string nomeStd = nomeLinha.toStdString();
    char* nome = const_cast<char*>(nomeStd.c_str());

    LinhaProdutos *linha = obterLinhaProdutosPorNome(stock, nome);
    if (linha == nullptr) {
        criarWarningMessageBox("ERRO!!", "Linha de produtos não encontrada.", 0);
        return;
    }

    LinhaDialog dialog;
    dialog.setNomeJanela(1, nomeLinha);
    dialog.setID(QString::number(linha->linhaID));
    dialog.setNome(nomeLinha);
    dialog.show();
    dialog.exec();

    if (dialog.result() == QDialog::Accepted) {
        if (dialog.getNome().length() > 50) {
            criarWarningMessageBox("ERRO!!", "Nome da linha de produtos não pode ter mais de 50 caracteres.", 0);
            return;
        }

        std::string novoNome = dialog.getNome().toStdString();
        char* novoNomeChar = new char[novoNome.length() + 1];
        std::strcpy(novoNomeChar, novoNome.c_str());

        LinhaProdutos novaLinha = criarLinhaProdutos(novoNomeChar);
        novaLinha.linhaID = linha->linhaID;

        int resultado = atualizarLinhaProdutos(stock, &novaLinha);
        if (resultado == 1) {
            criarWarningMessageBox("ERRO!!", "Linha de produtos não foi encontrada.", 0);
            return;
        } else if (resultado == 2) {
            criarWarningMessageBox("ERRO!!", "Linha de produtos não foi atualizada corretamente.", 0);
            return;
        } else if (resultado == -1) {
            criarWarningMessageBox("ERRO!!", "Nome da linha de produtos não pode ser vazio.", 0);
            return;
        }

        setLinhas(stock);
        setProdutos(nullptr);
        setModelos(nullptr);
        setModeloInfo(nullptr);
        ui->statusbar->showMessage("Linha de produtos atualizada com sucesso!");
    }

}

// Função para adicionar um produto a uma linha de produtos
void MainMenu::onAdicionarProdutoBtnClicked() {
    MenuDialog dialog;
    if (ui->linhasLV->currentIndex().row() == -1) {
        criarWarningMessageBox("ERRO!!", "Selecione uma linha de produtos para adicionar um produto.", 0);
        return;
    }

    dialog.setNomeJanela(1, ui->linhasLV->model()->data(ui->linhasLV->currentIndex(), Qt::DisplayRole).toString());
    dialog.show();
    dialog.exec();

    if (dialog.result() == QDialog::Accepted) {
        if (dialog.getNome().length() > 50 || dialog.getModelo().length() > 50 || dialog.getCategoria().length() > 50) {
            criarWarningMessageBox("ERRO!!", "Nome, modelo e tipo do produto não pode ter mais de 50 caracteres.", 0);
            return;
        }
        if (dialog.getNome().length() == 0 || dialog.getModelo().length() == 0 || dialog.getCategoria().length() == 0) {
            criarWarningMessageBox("ERRO!!", "Nome, modelo e tipo do produto não podem ser vazios.", 0);
            return;
        }
        if (dialog.getQuantidade() < 0) {
            criarWarningMessageBox("ERRO!!", "Quantidade do produto não pode ser negativa.", 0);
            return;
        }
        if (dialog.getPreco() < 0) {
            criarWarningMessageBox("ERRO!!", "Preço do produto não pode ser negativo.", 0);
            return;
        }

        std::string nome = dialog.getNome().toStdString();
        std::string categoria = dialog.getCategoria().toStdString();
        std::string modelo = dialog.getModelo().toStdString();

        char* nomeChar = new char[nome.length() + 1];
        std::strcpy(nomeChar, nome.c_str());
        char* categoriaChar = new char[categoria.length() + 1];
        std::strcpy(categoriaChar, categoria.c_str());
        char* modeloChar = new char[modelo.length() + 1];
        std::strcpy(modeloChar, modelo.c_str());

        ListaParamAdicionalProduto* parametros = nullptr;
        for (int i = 0; i < dialog.getNumeroParametros(); i++) {
            if (dialog.getNomeParametro(i).length() > 50 || dialog.getValorParametro(i).length() > 50) {
                criarWarningMessageBox("ERRO!!", "Nome e valor do parâmetro não podem ter mais de 50 caracteres.", 0);
                return;
            }
            if (dialog.getNomeParametro(i) == "" || dialog.getValorParametro(i) == "") {
                criarWarningMessageBox("ERRO!!", "Nome e valor do parâmetro não podem ser vazios.", 0);
                return;
            }
            std::string nomeParam = dialog.getNomeParametro(i).toStdString();
            std::string valorParam = dialog.getValorParametro(i).toStdString();

            char* nomeParamChar = new char[nomeParam.length() + 1];
            std::strcpy(nomeParamChar, nomeParam.c_str());
            char* valorParamChar = new char[valorParam.length() + 1];
            std::strcpy(valorParamChar, valorParam.c_str());

            ParamAdicionalProduto* parametro = criarParametroAdicionalProduto(nomeParamChar, valorParamChar);
            if (parametro == nullptr) {
                criarWarningMessageBox("ERRO!!", "Problemas ao adicionar parâmetro adicional! Verifique por valores em falta.", 0);
                return;
            }
            adicionarParametroAdicionalLista(&parametros, parametro);

            delete[] nomeParamChar;
            delete[] valorParamChar;
        }

        Produto produto = criarProduto(nomeChar, categoriaChar, modeloChar, dialog.getQuantidade(), dialog.getPreco(), parametros);

        delete[] nomeChar;
        delete[] categoriaChar;
        delete[] modeloChar;

        auto nomeQt = ui->linhasLV->model()->data(ui->linhasLV->currentIndex(), Qt::DisplayRole).toString();
        std::string nomeLinhaStd = nomeQt.toStdString();
        char* nomeLinha = const_cast<char*>(nomeLinhaStd.c_str());

        int resultado = adicionarProduto(obterLinhaProdutosPorNome(stock, nomeLinha), &produto);
        if (resultado == 1) {
            criarWarningMessageBox("ERRO!!", "Ocorreu um problema ao adicionar o produto.", 0);
            return;
        } else if (resultado == 2) {
            criarWarningMessageBox("ERRO!!", "A linha do produto não foi encontrada.", 0);
            return;
        } else if (resultado == 3) {
            criarWarningMessageBox("ERRO!!", "O produto com este nome e modelo já existe.", 0);
            return;
        } else if (resultado == 4) {
            criarWarningMessageBox("ERRO!!", "O nome do produto não pode ser vazio.", 0);
            return;
        } else if (resultado == 5) {
            criarWarningMessageBox("ERRO!!", "O modelo do produto não pode ser vazio.", 0);
            return;
        }

        setProdutos(obterLinhaProdutosPorID(stock, selLinha + 1));
        setModelos(obterListaProdutosPorIDLinha(stock, selLinha + 1));
        setModeloInfo(nullptr);

        ui->statusbar->showMessage("Produto adicionado com sucesso!");
    }
}

void MainMenu::onRemoverProdutoBtnClicked() {
    if (ui->linhasLV->currentIndex().row() == -1) {
        criarWarningMessageBox("ERRO!!", "Selecione uma linha de produtos para remover um produto.", 0);
        return;
    }
    if (ui->produtosLV->currentIndex().row() == -1) {
        criarWarningMessageBox("ERRO!!", "Selecione um produto para remover um modelo.", 0);
        return;
    }
    if (ui->modelosLV->currentIndex().row() == -1) {
        criarWarningMessageBox("ERRO!!", "Selecione um modelo para remover.", 0);
        return;
    }

    auto nomeLinha = ui->linhasLV->model()->data(ui->linhasLV->currentIndex(), Qt::DisplayRole).toString();
    std::string nomeLinhaStd = nomeLinha.toStdString();
    char* nomeLinhaChar = const_cast<char*>(nomeLinhaStd.c_str());

    QModelIndex indexProduto = ui->produtosLV->model()->index(selProduto, 0);
    auto itemProd = ui->produtosLV->model()->data(indexProduto, Qt::DisplayRole).toString();
    std::string nomeStd = itemProd.toStdString();
    auto modeloProd = ui->modelosLV->model()->data(ui->modelosLV->currentIndex(), Qt::DisplayRole).toString();
    std::string modeloStd = modeloProd.toStdString();

    char* nome = const_cast<char*>(nomeStd.c_str());
    char* modelo = const_cast<char*>(modeloStd.c_str());

    // open message box asking for confirmation
    int ret = criarWarningMessageBox("Confirmação", "Tem a certeza que deseja remover o modelo \"" + itemProd + " " + modeloProd + "\"?", 1);
    if (ret == QMessageBox::No) return;

    LinhaProdutos *linha = obterLinhaProdutosPorNome(stock, nomeLinhaChar);
    if (linha == nullptr) {
        criarWarningMessageBox("ERRO!!", "Linha de produtos não encontrada.", 0);
        return;
    }

    Produto *produto = obterProdutoPorNome(linha, nome, modelo);
    if (produto == nullptr) {
        criarWarningMessageBox("ERRO!!", "Produto não encontrado.", 0);
        return;
    }

    int resultado = removerProduto(linha, produto->produtoID);
    if (resultado == 1) {
        criarWarningMessageBox("ERRO!!", "Ocorreu um problema ao eliminar o produto!", 0);
        return;
    }

    setProdutos(obterLinhaProdutosPorNome(stock, nomeLinhaChar));
    setModelos(nullptr);
    setModeloInfo(nullptr);

    ui->statusbar->showMessage("Produto removido com sucesso!");
}

void MainMenu::onAtualizarProdutoBtnClicked() {
    QModelIndex selecProduto = ui->produtosLV->currentIndex();

    if (ui->linhasLV->currentIndex().row() == -1) {
        criarWarningMessageBox("ERRO!!", "Selecione uma linha de produtos para atualizar um produto.", 0);
        return;
    }
    if (ui->produtosLV->currentIndex().row() == -1) {
        criarWarningMessageBox("ERRO!!", "Selecione um produto para atualizar um modelo.", 0);
        return;
    }
    if (ui->modelosLV->currentIndex().row() == -1) {
        criarWarningMessageBox("ERRO!!", "Selecione um modelo para atualizar.", 0);
        return;
    }

    auto nomeLinha = ui->linhasLV->model()->data(ui->linhasLV->currentIndex(), Qt::DisplayRole).toString();
    std::string nomeLinhaStd = nomeLinha.toStdString();
    char* nomeLinhaChar = const_cast<char*>(nomeLinhaStd.c_str());

    QModelIndex indexProduto = ui->produtosLV->model()->index(selProduto, 0);
    auto itemProd = ui->produtosLV->model()->data(indexProduto, Qt::DisplayRole).toString();
    auto modeloProd = ui->modelosLV->model()->data(ui->modelosLV->currentIndex(), Qt::DisplayRole).toString();
    std::string nomeStd = itemProd.toStdString();
    std::string modeloStd = modeloProd.toStdString();
    char* nome = const_cast<char*>(nomeStd.c_str());
    char* modelo = const_cast<char*>(modeloStd.c_str());

    auto produto = (Produto *) obterProdutoPorNome(obterLinhaProdutosPorNome(stock, nomeLinhaChar), nome, modelo);

    MenuDialog dialog;
    dialog.setNomeJanela(2, itemProd + " " + modeloProd);
    dialog.setID(QString::number(produto->produtoID));
    dialog.setNome(produto->nome);
    dialog.setCategoria(produto->item);
    dialog.setModelo(produto->modelo);
    dialog.setQuantidade(static_cast<int>(produto->quantidade));
    dialog.setPreco(QString::number(produto->preco));

    ListaParamAdicionalProduto* current = produto->parametros;
    while (current != nullptr) {
        ParamAdicionalProduto param = *current->parametro;
        dialog.addParametro(param.nome, param.valor);
        current = current->prox_parametro;
    }

    dialog.show();
    dialog.exec();

    if (dialog.result() == QDialog::Accepted) {
        if (dialog.getNome().length() > 50 || dialog.getModelo().length() > 50 || dialog.getCategoria().length() > 50) {
            criarWarningMessageBox("ERRO!!", "Nome, modelo e tipo do produto não pode ter mais de 50 caracteres.", 0);
            return;
        }
        if (dialog.getNome().length() == 0 || dialog.getModelo().length() == 0 || dialog.getCategoria().length() == 0) {
            criarWarningMessageBox("ERRO!!", "Nome, modelo e tipo do produto não podem ser vazios.", 0);
            return;
        }
        if (dialog.getQuantidade() < 0) {
            criarWarningMessageBox("ERRO!!", "Quantidade do produto não pode ser negativa.", 0);
            return;
        }
        if (dialog.getPreco() < 0) {
            criarWarningMessageBox("ERRO!!", "Preço do produto não pode ser negativo.", 0);
            return;
        }

        std::string novoNome = dialog.getNome().toStdString();
        std::string categoria = dialog.getCategoria().toStdString();
        std::string novoModelo = dialog.getModelo().toStdString();

        char* nomeChar = new char[novoNome.length() + 1];
        std::strcpy(nomeChar, novoNome.c_str());
        char* categoriaChar = new char[categoria.length() + 1];
        std::strcpy(categoriaChar, categoria.c_str());
        char* modeloChar = new char[novoModelo.length() + 1];
        std::strcpy(modeloChar, novoModelo.c_str());

        ListaParamAdicionalProduto* parametros = nullptr;
        for (int i = 0; i < dialog.getNumeroParametros(); i++) {
            if (dialog.getNomeParametro(i).length() > 50 || dialog.getValorParametro(i).length() > 50) {
                criarWarningMessageBox("ERRO!!", "Nome e valor do parâmetro não podem ter mais de 50 caracteres.", 0);
                return;
            }
            if (dialog.getNomeParametro(i) == "" || dialog.getValorParametro(i) == "") {
                criarWarningMessageBox("ERRO!!", "Nome e valor do parâmetro não podem ser vazios.", 0);
                return;
            }
            std::string nomeParam = dialog.getNomeParametro(i).toStdString();
            std::string valorParam = dialog.getValorParametro(i).toStdString();

            char* nomeParamChar = new char[nomeParam.length() + 1];
            std::strcpy(nomeParamChar, nomeParam.c_str());
            char* valorParamChar = new char[valorParam.length() + 1];
            std::strcpy(valorParamChar, valorParam.c_str());

            ParamAdicionalProduto* parametro = criarParametroAdicionalProduto(nomeParamChar, valorParamChar);
            if (parametro == nullptr) {
                criarWarningMessageBox("ERRO!!", "Erro ao criar parâmetro adicional.", 0);
                return;
            }
            int resultado = adicionarParametroAdicionalLista(&parametros, parametro);
            if (resultado == 1) {
                criarWarningMessageBox("ERRO!!", "Erro ao adicionar parâmetro adicional à lista. Verifique se preencheu todos os campos.", 0);
                return;
            }

            delete[] nomeParamChar;
            delete[] valorParamChar;
        }

        Produto novoProduto = criarProduto(nomeChar, categoriaChar, modeloChar, dialog.getQuantidade(), dialog.getPreco(), parametros);
        novoProduto.linhaID = produto->linhaID;
        novoProduto.produtoID = produto->produtoID;

        delete[] nomeChar;
        delete[] categoriaChar;
        delete[] modeloChar;

        LinhaProdutos *linha = obterLinhaProdutosPorNome(stock, nomeLinhaChar);
        if (linha == nullptr) {
            criarWarningMessageBox("ERRO!!", "Linha de produtos não encontrada.", 0);
            return;
        }

        int resultado = atualizarProduto(linha, &novoProduto);
        if (resultado == 1) {
            criarWarningMessageBox("ERRO!!", "Produto não encontrado.", 0);
            return;
        }

        setProdutos(obterLinhaProdutosPorNome(stock, nomeLinhaChar));

        auto linhaAnt = (LinhaProdutos *) obterLinhaProdutosPorNome(stock, nomeLinhaChar);
        std::string nomeProduto = ui->produtosLV->model()->data(selecProduto, Qt::DisplayRole).toString().toStdString();
        char* nomeProdutoChar = const_cast<char*>(nomeProduto.c_str());

        auto lista = (ListaProdutos *) obterListaProdutosPorItemLinha(linhaAnt, nomeProdutoChar);
        setModelos(lista);
        ui->produtosLV->setCurrentIndex(selecProduto);
        setModeloInfo(nullptr);

        ui->statusbar->showMessage("Produto atualizado com sucesso!");
    }
}

/// TAB DEFINIÇÕES
// Função para alterar o nome da loja
void MainMenu::onNomeLojaModBtnClicked() {
    if (ui->nomeLojaModText->text().isEmpty()) {
        criarWarningMessageBox("ERRO!!", "Campo de texto vazio.", 0);
        return;
    }
    if (stock == nullptr) {
        criarWarningMessageBox("ERRO!!", "Stock não inicializado.", 0);
        return;
    }
    if (ui->nomeLojaModText->text().length() > 50) {
        criarWarningMessageBox("ERRO!!", "Nome da loja não pode ter mais de 50 caracteres.", 0);
        return;
    }

    QString newName = ui->nomeLojaModText->text();
    if (newName.isEmpty()) return;
    int resultado = editarStockLoja(stock, newName.toStdString().c_str());
    if (resultado == 1) {
        ui->statusbar->showMessage("ERRO!! - Erro ao alterar o nome da loja");
        return;
    }

    ui->nomeLojaLab->setText(newName);
    ui->nomeLojaModText->clear();
    window()->setWindowTitle("Gestão de Stock - " + newName);
    ui->statusbar->showMessage("Nome da loja alterado para " + newName);
}

// Função para atualizar a informação do stock na tab Definições
void MainMenu::onAtualizarStockInfoBtnClicked() {
    tabDefinicoes();
}

// Função para apagar todas as linhas de produtos do stock
void MainMenu::onApagaLinhasStockBtnClicked() {
    if (stock == nullptr) {
        criarWarningMessageBox("ERRO!!", "Stock não inicializado.", 0);
        return;
    }

    int ret = criarWarningMessageBox("Confirmação", "Tem a certeza que deseja apagar todas as linhas de stock?", 1);
    if (ret == QMessageBox::No) return;
    apagarLinhasProdutos(stock);
    tabDefinicoes();
    ui->statusbar->showMessage("Linhas de stock apagadas. Stock foi limpo.");
}

// Função para apagar todos os produtos do stock
void MainMenu::onApagaProdutosStockBtnClicked() {
    if (stock == nullptr) {
        criarWarningMessageBox("ERRO!!", "Stock não inicializado.", 0);
        return;
    }

    int ret = criarWarningMessageBox("Confirmação", "Tem a certeza que deseja apagar todos os produtos do stock? Linhas serão mantidas.", 1);
    if (ret == QMessageBox::No) return;
    apagarProdutosLinhasStock(stock);
    tabDefinicoes();
    ui->statusbar->showMessage("Produtos de stock apagados. Linhas foram mantidas.");
}

/// TAB PESQUISA
// Função para pesquisa
void MainMenu::onProcuraBtnClicked() {
    QString procura = ui->procuraTE->text();
    if (procura.isEmpty()) {
        criarWarningMessageBox("ERRO!!", "Campo de pesquisa vazio.", 0);
        return;
    }

    int choice = ui->catPesqDD->currentIndex();
    switch (choice) {
        case 0: {
            QStringList parts = procura.split(".");
            if (parts.size() > 2) {
                criarWarningMessageBox("ERRO!!", "Código inválido.", 0);
                return;
            }

            if (parts.size() == 1 && parts[0].toInt() != 0) {
                LinhaProdutos *linha = obterLinhaProdutosPorID(stock, parts[0].toInt());
                if (linha == nullptr) {
                    criarWarningMessageBox("ERRO!!", "Linha não encontrada.", 0);
                    return;
                }

                ui->resultPesqTV->setRowCount(static_cast<int>(linha->num_produtos));
                ui->resultPesqTV->setColumnCount(8);
                ui->resultPesqTV->setHorizontalHeaderLabels(QStringList() << "Código interno" << "Linha" << "Nome" << "Modelo" << "Categoria" << "Quantidade" << "Preço" << "Número de parâmetros");

                int i = 0;
                for (ListaProdutos *current = linha->lista_produtos; current; current = current->prox_produto) {
                    Produto produto = *current->produto;
                    ui->resultPesqTV->setItem(i, 0, new QTableWidgetItem(QString::number(produto.linhaID) + "." + QString::number(produto.produtoID)));
                    ui->resultPesqTV->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(linha->nome)));
                    ui->resultPesqTV->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(produto.nome)));
                    ui->resultPesqTV->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(produto.modelo)));
                    ui->resultPesqTV->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(produto.item)));
                    ui->resultPesqTV->setItem(i, 5, new QTableWidgetItem(QString::number(produto.quantidade)));
                    ui->resultPesqTV->setItem(i, 6, new QTableWidgetItem(QString::number(produto.preco).replace('.',',') + " €"));
                    ui->resultPesqTV->setItem(i, 7, new QTableWidgetItem(QString::number(produto.num_parametros)));
                    i++;
                }
                for (int j = 0; j < 8; j++) {
                    ui->resultPesqTV->resizeColumnToContents(j);
                }
                return;
            } else if (parts.size() == 2 && parts[1].toInt() != 0 && parts[0].toInt() != 0) {
                LinhaProdutos *linha = obterLinhaProdutosPorID(stock, parts[0].toInt());
                if (linha == nullptr) {
                    QMessageBox msgBox;
                    criarWarningMessageBox("ERRO!!", "Linha não encontrada.", 0);
                    return;
                }

                Produto *produto = obterProdutoPorID(linha, parts[1].toInt());
                if (produto == nullptr) {
                    criarWarningMessageBox("ERRO!!", "Produto não encontrado.", 0);
                    return;
                }

                ui->resultPesqTV->setRowCount(1);
                ui->resultPesqTV->setColumnCount(8);
                ui->resultPesqTV->setHorizontalHeaderLabels(QStringList() << "Código Interno" << "Linha" << "Nome" << "Modelo" << "Categoria" << "Quantidade" << "Preço" << "Número de parâmetros");
                ui->resultPesqTV->setItem(0, 0, new QTableWidgetItem(QString::number(produto->linhaID) + "." + QString::number(produto->produtoID)));
                ui->resultPesqTV->setItem(0, 1, new QTableWidgetItem(QString::fromStdString(linha->nome)));
                ui->resultPesqTV->setItem(0, 2, new QTableWidgetItem(QString::fromStdString(produto->nome)));
                ui->resultPesqTV->setItem(0, 3, new QTableWidgetItem(QString::fromStdString(produto->modelo)));
                ui->resultPesqTV->setItem(0, 4, new QTableWidgetItem(QString::fromStdString(produto->item)));
                ui->resultPesqTV->setItem(0, 5, new QTableWidgetItem(QString::number(produto->quantidade)));
                ui->resultPesqTV->setItem(0, 6, new QTableWidgetItem(QString::number(produto->preco).replace('.',',') + " €"));
                ui->resultPesqTV->setItem(0, 7, new QTableWidgetItem(QString::number(produto->num_parametros)));
                for (int j = 0; j < 8; j++) {
                    ui->resultPesqTV->resizeColumnToContents(j);
                }
                return;
            } else {
                criarWarningMessageBox("ERRO!!", "Código inválido.", 0);
                return;
            }
        }
        case 1: {
            std::string nomeStd = procura.toStdString();
            char* nomeParamChar = const_cast<char*>(nomeStd.c_str());

            if (procura.isEmpty()) {
                criarWarningMessageBox("ERRO!!", "Campo de pesquisa vazio.", 0);
                return;
            }

            ListaProdutos *lista = procurarStockPorNomeProduto(stock, nomeParamChar);
            if (lista == nullptr) {
                criarWarningMessageBox("ERRO!!", "Produto não encontrado.", 0);
                return;
            }

            ui->resultPesqTV->setRowCount(static_cast<int>(getNumeroProdutosLista(lista)));
            ui->resultPesqTV->setColumnCount(8);
            ui->resultPesqTV->setHorizontalHeaderLabels(QStringList() << "Código Interno" << "Linha" << "Nome" << "Modelo" << "Categoria" << "Quantidade" << "Preço" << "Número de parâmetros");
            int i = 0;
            for (ListaProdutos *current = lista; current; current = current->prox_produto) {
                Produto produto = *current->produto;
                LinhaProdutos *linha = obterLinhaProdutosPorID(stock, (int) produto.linhaID);
                ui->resultPesqTV->setItem(i, 0, new QTableWidgetItem(QString::number(produto.linhaID) + "." + QString::number(produto.produtoID)));
                ui->resultPesqTV->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(linha->nome)));
                ui->resultPesqTV->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(produto.nome)));
                ui->resultPesqTV->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(produto.modelo)));
                ui->resultPesqTV->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(produto.item)));
                ui->resultPesqTV->setItem(i, 5, new QTableWidgetItem(QString::number(produto.quantidade)));
                ui->resultPesqTV->setItem(i, 6, new QTableWidgetItem(QString::number(produto.preco).replace('.',',') + " €"));
                ui->resultPesqTV->setItem(i, 7, new QTableWidgetItem(QString::number(produto.num_parametros)));
                i++;
            }
            for (int j = 0; j < 8; j++) {
                ui->resultPesqTV->resizeColumnToContents(j);
            }

            return;
        }
        case 2: {
            std::string modeloStd = procura.toStdString();
            char* modeloParamChar = const_cast<char*>(modeloStd.c_str());

            if (procura.isEmpty()) {
                criarWarningMessageBox("ERRO!!", "Campo de pesquisa vazio.", 0);
                return;
            }

            ListaProdutos *lista = procurarStockPorModeloProduto(stock, modeloParamChar);
            if (lista == nullptr) {
                criarWarningMessageBox("ERRO!!", "Modelo não encontrado.", 0);
                return;
            }

            ui->resultPesqTV->setRowCount(static_cast<int>(getNumeroProdutosLista(lista)));
            ui->resultPesqTV->setColumnCount(8);
            ui->resultPesqTV->setHorizontalHeaderLabels(QStringList() << "Código Interno" << "Linha" << "Nome" << "Modelo" << "Categoria" << "Quantidade" << "Preço" << "Número de parâmetros");
            int i = 0;
            for (ListaProdutos *current = lista; current; current = current->prox_produto) {
                Produto produto = *current->produto;
                LinhaProdutos *linha = obterLinhaProdutosPorID(stock, (int) produto.linhaID);
                ui->resultPesqTV->setItem(i, 0, new QTableWidgetItem(QString::number(produto.linhaID) + "." + QString::number(produto.produtoID)));
                ui->resultPesqTV->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(linha->nome)));
                ui->resultPesqTV->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(produto.nome)));
                ui->resultPesqTV->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(produto.modelo)));
                ui->resultPesqTV->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(produto.item)));
                ui->resultPesqTV->setItem(i, 5, new QTableWidgetItem(QString::number(produto.quantidade)));
                ui->resultPesqTV->setItem(i, 6, new QTableWidgetItem(QString::number(produto.preco).replace('.',',') + " €"));
                ui->resultPesqTV->setItem(i, 7, new QTableWidgetItem(QString::number(produto.num_parametros)));
                i++;
            }
            for (int j = 0; j < 8; j++) {
                ui->resultPesqTV->resizeColumnToContents(j);
            }

            return;
        }
        case 3: {
            std::string categoriaStd = procura.toStdString();
            char* categoriaParamChar = const_cast<char*>(categoriaStd.c_str());

            if (procura.isEmpty()) {
                criarWarningMessageBox("ERRO!!", "Campo de pesquisa vazio.", 0);
                return;
            }

            ListaProdutos *lista = procurarStockPorItemProduto(stock, categoriaParamChar);
            if (lista == nullptr) {
                criarWarningMessageBox("ERRO!!", "Categoria não encontrada.", 0);
                return;
            }

            ui->resultPesqTV->setRowCount(static_cast<int>(getNumeroProdutosLista(lista)));
            ui->resultPesqTV->setColumnCount(8);
            ui->resultPesqTV->setHorizontalHeaderLabels(QStringList() << "Código Interno" << "Linha" << "Nome" << "Modelo" << "Categoria" << "Quantidade" << "Preço" << "Número de parâmetros");
            int i = 0;
            for (ListaProdutos *current = lista; current; current = current->prox_produto) {
                Produto produto = *current->produto;
                LinhaProdutos *linha = obterLinhaProdutosPorID(stock, (int) produto.linhaID);
                ui->resultPesqTV->setItem(i, 0, new QTableWidgetItem(QString::number(produto.linhaID) + "." + QString::number(produto.produtoID)));
                ui->resultPesqTV->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(linha->nome)));
                ui->resultPesqTV->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(produto.nome)));
                ui->resultPesqTV->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(produto.modelo)));
                ui->resultPesqTV->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(produto.item)));
                ui->resultPesqTV->setItem(i, 5, new QTableWidgetItem(QString::number(produto.quantidade)));
                ui->resultPesqTV->setItem(i, 6, new QTableWidgetItem(QString::number(produto.preco).replace('.',',') + " €"));
                ui->resultPesqTV->setItem(i, 7, new QTableWidgetItem(QString::number(produto.num_parametros)));
                i++;
            }
            for (int j = 0; j < 8; j++) {
                ui->resultPesqTV->resizeColumnToContents(j);
            }

            return;
        }
        case 4: {
            std::string parametroStd = procura.toStdString();
            char* parametroChar = const_cast<char*>(parametroStd.c_str());

            if (procura.isEmpty()) {
                criarWarningMessageBox("ERRO!!", "Campo de pesquisa vazio.", 0);
                return;
            }

            ListaProdutos *lista = procurarStockPorParametroProduto(stock, parametroChar);
            if (lista == nullptr) {
                criarWarningMessageBox("ERRO!!", "Parâmetro não encontrado.", 0);
                return;
            }

            ui->resultPesqTV->setRowCount(static_cast<int>(getNumeroProdutosLista(lista)));
            ui->resultPesqTV->setColumnCount(8);
            ui->resultPesqTV->setHorizontalHeaderLabels(QStringList() << "Código Interno" << "Linha" << "Nome" << "Modelo" << "Categoria" << "Quantidade" << "Preço" << "Número de parâmetros");
            int i = 0;
            for (ListaProdutos *current = lista; current; current = current->prox_produto) {
                Produto produto = *current->produto;
                LinhaProdutos *linha = obterLinhaProdutosPorID(stock, (int) produto.linhaID);
                ui->resultPesqTV->setItem(i, 0, new QTableWidgetItem(QString::number(produto.linhaID) + "." + QString::number(produto.produtoID)));
                ui->resultPesqTV->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(linha->nome)));
                ui->resultPesqTV->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(produto.nome)));
                ui->resultPesqTV->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(produto.modelo)));
                ui->resultPesqTV->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(produto.item)));
                ui->resultPesqTV->setItem(i, 5, new QTableWidgetItem(QString::number(produto.quantidade)));
                ui->resultPesqTV->setItem(i, 6, new QTableWidgetItem(QString::number(produto.preco).replace('.',',') + " €"));
                ui->resultPesqTV->setItem(i, 7, new QTableWidgetItem(QString::number(produto.num_parametros)));
                i++;
            }
            for (int j = 0; j < 8; j++) {
                ui->resultPesqTV->resizeColumnToContents(j);
            }

            return;
        }
        case 5: {
            std::string linhaStd = procura.toStdString();
            char* linhaChar = const_cast<char*>(linhaStd.c_str());

            if (procura.isEmpty()) {
                criarWarningMessageBox("ERRO!!", "Campo de pesquisa vazio.", 0);
                return;
            }

            LinhaProdutos *linha = obterLinhaProdutosPorNome(stock, linhaChar);
            if (linha == nullptr) {
                criarWarningMessageBox("ERRO!!", "Linha não encontrada.", 0);
                return;
            }

            ui->resultPesqTV->setRowCount(static_cast<int>(linha->num_produtos));
            ui->resultPesqTV->setColumnCount(8);
            ui->resultPesqTV->setHorizontalHeaderLabels(QStringList() << "Código Interno" << "Linha" << "Nome" << "Modelo" << "Categoria" << "Quantidade" << "Preço" << "Número de parâmetros");
            int i = 0;
            for (ListaProdutos *current = linha->lista_produtos; current; current = current->prox_produto) {
                Produto produto = *current->produto;
                ui->resultPesqTV->setItem(i, 0, new QTableWidgetItem(QString::number(produto.linhaID) + "." + QString::number(produto.produtoID)));
                ui->resultPesqTV->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(linha->nome)));
                ui->resultPesqTV->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(produto.nome)));
                ui->resultPesqTV->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(produto.modelo)));
                ui->resultPesqTV->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(produto.item)));
                ui->resultPesqTV->setItem(i, 5, new QTableWidgetItem(QString::number(produto.quantidade)));
                ui->resultPesqTV->setItem(i, 6, new QTableWidgetItem(QString::number(produto.preco).replace('.',',') + " €"));
                ui->resultPesqTV->setItem(i, 7, new QTableWidgetItem(QString::number(produto.num_parametros)));
                i++;
            }
            for (int j = 0; j < 8; j++) {
                ui->resultPesqTV->resizeColumnToContents(j);
            }

            return;
        }
        default:
            break;
    }
    ui->statusbar->showMessage("Produto encontrado com sucesso!");
}

// Processamento de mudança de tab
void MainMenu::onTabChanged(int index) {
    reloadTabs();
}

int MainMenu::criarWarningMessageBox(const QString &title, const QString &text, int mode) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    if (mode == 1) {
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
    } else {
        msgBox.setStandardButtons(QMessageBox::Ok);
    }
    msgBox.setStyleSheet("QMessageBox {icon-size: 64px;}");
    return msgBox.exec();
}

#ifdef Q_OS_MACOS
void MainMenu::createMacMenu() {
    QMenuBar* menuBar = ui->menubar;
    menuBar->setNativeMenuBar(true);
}
#endif
