#include <QMessageBox>
#include <QFile>
#include <QStringListModel>
#include "mainmenu.h"
#include "ui_MainMenu.h"

StockLoja *stock;
int selLinha = 0, selProduto = 0, selModelo = 0;

void MainMenu::open() {
    statusBar()->showMessage("Ação abrir chamada");
}

void MainMenu::save() {
    QMessageBox msgBox(QMessageBox::Warning, "Atenção", "As alterações não foram guardadas. Deseja guardar as alterações?", QMessageBox::Save | QMessageBox::No, this);
    msgBox.setStyleSheet("QMessageBox {icon-size: 64px;}");
    statusBar()->showMessage((msgBox.exec() == QMessageBox::Save) ? "Save was clicked" : "No was clicked");
}

void MainMenu::quit() {
    QMessageBox msgBox(QMessageBox::Warning, "Atenção", "Tem a certeza que deseja sair?",
                        QMessageBox::Yes | QMessageBox::No, this);
    msgBox.setStyleSheet("QMessageBox {icon-size: 64px;}");
    if (msgBox.exec() == QMessageBox::Yes) QMainWindow::close();
    else statusBar()->showMessage("Não foi clicado");
}

MainMenu::MainMenu(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainMenu) {
    ui->setupUi(this);

    QAction* actions[] = {
        new QAction(tr("Abrir base de dados"), this),
        new QAction(tr("Salvar para base de dados"), this),
        new QAction(tr("Sair"), this)
    };
    actions[0]->setShortcut(QKeySequence::Open);
    actions[1]->setShortcut(QKeySequence::Save);
    actions[2]->setShortcut(QKeySequence::Quit);

    connect(actions[0], &QAction::triggered, this, &MainMenu::open);
    connect(actions[1], &QAction::triggered, this, &MainMenu::save);
    connect(actions[2], &QAction::triggered, this, &MainMenu::quit);

    ui->menuFicheiro->addActions({actions[0], actions[1]});
    ui->menuFicheiro->addSeparator();
    ui->menuFicheiro->addAction(actions[2]);

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
        std::make_pair(ui->apagaProdutosStockBtn, &MainMenu::onApagaProdutosStockBtnClicked)
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
    if (!ptr) return;
    stock = ptr;
    ui->nomeLojaLab->setText(ptr->nome);
}

void MainMenu::setLinhas(StockLoja *ptr) {
    if (!ptr) return;

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
        ui->modelosLV->setModel(nullptr);
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

    // Clear any existing items and set the row count
    ui->outrosParamTV->clearContents();
    ui->outrosParamTV->setRowCount(static_cast<int>(ptr->num_parametros));
    ui->outrosParamTV->setColumnCount(2);
    ui->outrosParamTV->setHorizontalHeaderLabels(QStringList() << "Nome" << "Valor");

    ListaParamAdicionalProduto* current = ptr->parametros;
    int numparametros = static_cast<int>(ptr->num_parametros);

    for (int i = 0; i < numparametros; i++) {
        ParamAdicionalProduto param = *current->parametro;
        ui->outrosParamTV->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(param.nome)));
        ui->outrosParamTV->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(param.valor)));
        current = current->prox_parametro;
    }
}

void MainMenu::tabDefinicoes() {
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
    } else if (ui->tabWidget->currentIndex() == 1) { // pesquisa
        ui->procuraTE->clear();
        ui->resultPesqTV->clear();
    } else if (ui->tabWidget->currentIndex() == 2) { // definições
        MainMenu::tabDefinicoes();
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
    QAbstractItemModel* model = ui->produtosLV->model();
    QString text = model->data(index, Qt::DisplayRole).toString();
    std::string nomeStd = text.toStdString();
    char* nome = const_cast<char*>(nomeStd.c_str());
    auto lista = (ListaProdutos *) procurarStockPorNomeProduto(stock, nome);
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
        std::string nome = dialog.getNome().toStdString();
        char* nomeChar = new char[nome.length() + 1];
        std::strcpy(nomeChar, nome.c_str());

        LinhaProdutos linha = criarLinhaProdutos(nomeChar);
        adicionarLinhaProdutos(stock, linha);
        delete[] nomeChar;

        setLinhas(stock);
        setProdutos(nullptr);
        setModelos(nullptr);
        setModeloInfo(nullptr);
        ui->statusbar->showMessage("Linha de produtos adicionada com sucesso!");
    }
}

// Função para remover uma linha de produtos
void MainMenu::onRemoverLinhaBtnClicked() {
    if (ui->linhasLV->currentIndex().row() == -1) {
        ui->statusbar->showMessage("ERRO!! - Selecione uma linha de produtos para remover.");
        return;
    }

    auto nomeLinha = ui->linhasLV->model()->data(ui->linhasLV->currentIndex(), Qt::DisplayRole).toString();
    std::string nomeStd = nomeLinha.toStdString();
    char* nome = const_cast<char*>(nomeStd.c_str());

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Tem a certeza que deseja remover a linha de produtos \"" + nomeLinha + "\"?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setStyleSheet("QMessageBox {icon-size: 64px;}");
    int ret = msgBox.exec();
    if (ret == QMessageBox::No) return;

    LinhaProdutos *linha = obterLinhaProdutosPorNome(stock, nome);
    int resultado = removerLinhaProdutos(stock, linha->linhaID);
    if (resultado == 1) {
        ui->statusbar->showMessage("FATAL!! - Linha de produtos não encontrada!");
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
        ui->statusbar->showMessage("ERRO!! - Selecione uma linha de produtos para atualizar.");
        return;
    }

    auto nomeLinha = ui->linhasLV->model()->data(ui->linhasLV->currentIndex(), Qt::DisplayRole).toString();
    std::string nomeStd = nomeLinha.toStdString();
    char* nome = const_cast<char*>(nomeStd.c_str());

    LinhaProdutos *linha = obterLinhaProdutosPorNome(stock, nome);
    LinhaDialog dialog;
    dialog.setNomeJanela(1, nomeLinha);
    dialog.setID(QString::number(linha->linhaID));
    dialog.setNome(nomeLinha);
    dialog.show();
    dialog.exec();

    if (dialog.result() == QDialog::Accepted) {
        std::string novoNome = dialog.getNome().toStdString();
        char* novoNomeChar = new char[novoNome.length() + 1];
        std::strcpy(novoNomeChar, novoNome.c_str());

        LinhaProdutos novaLinha = criarLinhaProdutos(novoNomeChar);
        novaLinha.linhaID = linha->linhaID;

        int resultado = atualizarLinhaProdutos(stock, &novaLinha);
        if (resultado == 1) {
            ui->statusbar->showMessage("FATAL!! - Linha de produtos não encontrada!");
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
        ui->statusbar->showMessage("ERRO!! - Selecione uma linha de produtos para adicionar um produto.");
        return;
    }

    dialog.setNomeJanela(1, ui->linhasLV->model()->data(ui->linhasLV->currentIndex(), Qt::DisplayRole).toString());
    dialog.show();
    dialog.exec();

    if (dialog.result() == QDialog::Accepted) {
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
            std::string nomeParam = dialog.getNomeParametro(i).toStdString();
            std::string valorParam = dialog.getValorParametro(i).toStdString();

            char* nomeParamChar = new char[nomeParam.length() + 1];
            std::strcpy(nomeParamChar, nomeParam.c_str());
            char* valorParamChar = new char[valorParam.length() + 1];
            std::strcpy(valorParamChar, valorParam.c_str());

            ParamAdicionalProduto* parametro = criarParametroAdicionalProduto(nomeParamChar, valorParamChar);
            adicionarParametroAdicionalLista(&parametros, parametro);

            delete[] nomeParamChar;
            delete[] valorParamChar;
        }

        Produto produto = criarProduto(nomeChar, categoriaChar, modeloChar, dialog.getQuantidade(), dialog.getPreco(), parametros);

        delete[] nomeChar;
        delete[] categoriaChar;
        delete[] modeloChar;

        adicionarProduto(obterLinhaProdutosPorID(stock, selLinha + 1), &produto);
        setProdutos(obterLinhaProdutosPorID(stock, selLinha + 1));

        ui->statusbar->showMessage("Produto adicionado com sucesso!");
    }
}

void MainMenu::onRemoverProdutoBtnClicked() {
    if (ui->linhasLV->currentIndex().row() == -1) {
        ui->statusbar->showMessage("ERRO!! - Selecione uma linha de produtos para remover um produto.");
        return;
    }
    if (ui->produtosLV->currentIndex().row() == -1) {
        ui->statusbar->showMessage("ERRO!! - Selecione um produto para remover um modelo.");
        return;
    }
    if (ui->modelosLV->currentIndex().row() == -1) {
        ui->statusbar->showMessage("ERRO!! - Selecione um modelo para remover.");
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

    // open message box asking for confirmation
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Tem a certeza que deseja remover o produto?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setStyleSheet("QMessageBox {icon-size: 64px;}");
    int ret = msgBox.exec();
    if (ret == QMessageBox::No) return;

    LinhaProdutos *linha = obterLinhaProdutosPorNome(stock, nomeLinhaChar);
    Produto *produto = obterProdutoPorNome(linha, nome, modelo);
    int resultado = removerProduto(linha, produto->produtoID);
    if (resultado == 0) {
        ui->statusbar->showMessage("FATAL!! - Produto não encontrado!");
        return;
    }

    setProdutos(obterLinhaProdutosPorNome(stock, nomeLinhaChar));
    setModelos(nullptr);
    setModeloInfo(nullptr);
    ui->statusbar->showMessage("Produto removido com sucesso!");
}

void MainMenu::onAtualizarProdutoBtnClicked() {
    if (ui->linhasLV->currentIndex().row() == -1) {
        ui->statusbar->showMessage("ERRO!! - Selecione uma linha de produtos para atualizar um produto.");
        return;
    }
    if (ui->produtosLV->currentIndex().row() == -1) {
        ui->statusbar->showMessage("ERRO!! - Selecione um produto para atualizar um modelo.");
        return;
    }
    if (ui->modelosLV->currentIndex().row() == -1) {
        ui->statusbar->showMessage("ERRO!! - Selecione um modelo para atualizar.");
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
            std::string nomeParam = dialog.getNomeParametro(i).toStdString();
            std::string valorParam = dialog.getValorParametro(i).toStdString();

            char* nomeParamChar = new char[nomeParam.length() + 1];
            std::strcpy(nomeParamChar, nomeParam.c_str());
            char* valorParamChar = new char[valorParam.length() + 1];
            std::strcpy(valorParamChar, valorParam.c_str());

            ParamAdicionalProduto* parametro = criarParametroAdicionalProduto(nomeParamChar, valorParamChar);
            adicionarParametroAdicionalLista(&parametros, parametro);

            delete[] nomeParamChar;
            delete[] valorParamChar;
        }

        Produto novoProduto = criarProduto(nomeChar, categoriaChar, modeloChar, dialog.getQuantidade(), dialog.getPreco(), parametros);
        novoProduto.linhaID = produto->linhaID;
        novoProduto.listaID = produto->listaID;
        novoProduto.produtoID = produto->produtoID;

        delete[] nomeChar;
        delete[] categoriaChar;
        delete[] modeloChar;

        LinhaProdutos *linha = obterLinhaProdutosPorNome(stock, nomeLinhaChar);
        int resultado = atualizarProduto(linha, &novoProduto);
        if (resultado == 1) {
            ui->statusbar->showMessage("FATAL!! - Produto não encontrado!");
            return;
        }

        setProdutos(obterLinhaProdutosPorNome(stock, nomeLinhaChar));
        setModelos(nullptr);
        setModeloInfo(nullptr);
        ui->statusbar->showMessage("Produto atualizado com sucesso!");
    }
}

/// TAB DEFINIÇÕES
// Função para alterar o nome da loja
void MainMenu::onNomeLojaModBtnClicked() {
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
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Tem a certeza que deseja apagar todas as linhas de stock?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setStyleSheet("QMessageBox {icon-size: 64px;}");
    int ret = msgBox.exec();
    if (ret == QMessageBox::No) return;
    apagarLinhasProdutos(stock);
    tabDefinicoes();
    ui->statusbar->showMessage("Linhas de stock apagadas. Stock foi limpo.");
}

// Função para apagar todos os produtos do stock
void MainMenu::onApagaProdutosStockBtnClicked() {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Tem a certeza que deseja apagar todos os produtos do stock?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setStyleSheet("QMessageBox {icon-size: 64px;}");
    int ret = msgBox.exec();
    if (ret == QMessageBox::No) return;
    apagarProdutosLinhasStock(stock);
    tabDefinicoes();
    ui->statusbar->showMessage("Produtos de stock apagados. Linhas foram mantidas.");
}

// Processamento de mudança de tab
void MainMenu::onTabChanged(int index) {
    reloadTabs();
}

#ifdef Q_OS_MACOS
void MainMenu::createMacMenu() {
    QMenuBar* menuBar = ui->menubar;
    menuBar->setNativeMenuBar(true);
}
#endif
