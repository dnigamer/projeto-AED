#include <QMessageBox>
#include <QFile>
#include <QStringListModel>
#include "mainmenu.h"
#include "ui_MainMenu.h"

StockLoja *stock;
int selLinha = 0;
int selProduto = 0;
int selModelo = 0;

// Função para abrir a base de dados (menu Ficheiro -> Abrir)
void MainMenu::open()
{
    statusBar()->showMessage("Ação abrir chamada");
}

// Função para salvar a base de dados (menu Ficheiro -> Salvar)
void MainMenu::save()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("As alterações não foram guardadas.");
    msgBox.setInformativeText("Deseja guardar as alterações?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setStyleSheet("QMessageBox {icon-size: 64px;}");

    int ret = msgBox.exec();
    if (ret == QMessageBox::Save) {
        statusBar()->showMessage("Save was clicked");
    } else {
        statusBar()->showMessage("No was clicked");
    }

}

// Função para sair da aplicação (menu Ficheiro -> Sair)
void MainMenu::quit()
{
    statusBar()->showMessage("Ação sair");

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Tem a certeza que deseja sair?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setStyleSheet("QMessageBox {icon-size: 64px;}");

    int ret = msgBox.exec();

    switch (ret) {
        case QMessageBox::Yes:
            QMainWindow::close();
            break;
        case QMessageBox::No:
            statusBar()->showMessage("Não foi clicado");
            break;
        default:
            break;
    }
}

MainMenu::MainMenu(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainMenu) {
    bool problemas = false;
    ui->setupUi(this); // Set up the UI

    #ifdef Q_OS_MACOS
        createMacMenu();
    #endif

    // Ações para abrir, salvar e sair no menu de title "Ficheiro"
    auto *actionOpen = new QAction(tr("Abrir base de dados"), this);
    actionOpen->setShortcut(QKeySequence::Open);
    auto *actionSave = new QAction(tr("Salvar para base de dados"), this);
    actionSave->setShortcut(QKeySequence::Save);
    auto *actionExit = new QAction(tr("Sair"), this);
    actionExit->setShortcut(QKeySequence::Quit);

    // Conectar funções às ações (Ficheiro -> Abrir, Ficheiro -> Salvar, Ficheiro -> Sair)
    connect(actionOpen, &QAction::triggered, this, &MainMenu::open);
    connect(actionSave, &QAction::triggered, this, &MainMenu::save);
    connect(actionExit, &QAction::triggered, this, &MainMenu::quit);

    // Assinar ações aos itens de menu (Ficheiro -> Abrir, Ficheiro -> Salvar, Ficheiro -> Sair)
    ui->menuFicheiro->addAction(actionOpen);
    ui->menuFicheiro->addAction(actionSave);
    ui->menuFicheiro->addSeparator();
    ui->menuFicheiro->addAction(actionExit);

    // operações sobre ListViews - não permitir edição
    ui->linhasLV->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->produtosLV->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->modelosLV->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Listeners para ListViews - deteção de clique
    connect(ui->linhasLV, &QListView::clicked, this, &MainMenu::onLinhasLVClicked);
    connect(ui->produtosLV, &QListView::clicked, this, &MainMenu::onProdutosLVClicked);
    connect(ui->modelosLV, &QListView::clicked, this, &MainMenu::onModelosLVClicked);

    // Listeners para botoes
    // Tab Stock
    //connect(ui->stockAtualizarBtn, &QPushButton::clicked, this, &MainMenu::setLinhas)
    connect(ui->adModeloBtn, &QPushButton::clicked, this, &MainMenu::onAdicionarProdutoBtnClicked);
    connect(ui->delModeloBtn, &QPushButton::clicked, this, &MainMenu::onRemoverProdutoBtnClicked);
    connect(ui->edModeloBtn, &QPushButton::clicked, this, &MainMenu::onAtualizarProdutoBtnClicked);


    // Tab Definições
    connect(ui->nomeLojaModBtn, &QPushButton::clicked, this, &MainMenu::onNomeLojaModBtnClicked);
    connect(ui->atualizarStockInfoBtn, &QPushButton::clicked, this, &MainMenu::onAtualizarStockInfoBtnClicked);
    connect(ui->apagaLinhasStockBtn, &QPushButton::clicked, this, &MainMenu::onApagaLinhasStockBtnClicked);
    connect(ui->apagaProdutosStockBtn, &QPushButton::clicked, this, &MainMenu::onApagaProdutosStockBtnClicked);

    // Listeners para tabgroups
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainMenu::onTabChanged); // mudança de tab

    // Adicionar logo no sitio logoLoja
    if (!QFile::exists("logo.png")) {
        statusBar()->showMessage("Logo não encontrado. Aplicação funcionará sem o logo.");
        ui->logoLoja->setText("N/D");
        problemas = true;
    } else {
        QPixmap logo("logo.png");
        ui->logoLoja->setPixmap(logo);
        ui->logoLoja->setScaledContents(true);
    }

    // Mostra informação sobre a aplicação na barra de estado
    if (!problemas) statusBar()->showMessage("Pronto");
}

// Destruidor da classe MainMenu (janela) - liberta memória
MainMenu::~MainMenu()
{
    delete ui;
}

// Função para definir o stock da loja em C++ para a janela
void MainMenu::setStock(StockLoja *ptr) {
    if (ptr == nullptr) return;
    stock = ptr;
    ui->nomeLojaLab->setText(ptr->nome);
}

// Função para mostrar as linhas de produtos na ListView baseado no stock
void MainMenu::setLinhas(StockLoja *ptr) {
    if (ptr == nullptr) return;

    QStringList list;
    ListaLinhaProdutos *current = ptr->lista_linhas;
    while (current != nullptr) {
        LinhaProdutos linha = *current->linha;
        QString linhaStr = linha.nome;
        list << linhaStr;
        current = (ListaLinhaProdutos *) current->prox_linha;
    }
    std::reverse(list.begin(), list.end());

    auto *model = new QStringListModel(this);
    model->setStringList(list);
    ui->linhasLV->setModel(model);

    if (list.empty()) return;
    if (ui->linhasLV->selectionModel() == nullptr) return;

    // Conectar a função onLinhasLVClicked ao evento de clique na ListView para mostrar os produtos de uma linha
    connect(ui->linhasLV->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainMenu::onLinhasLVClicked);
}

// Função para mostrar os produtos na ListView baseado na linha de produtos selecionada na primeira ListView
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
    // Ordenar a lista de produtos
    std::sort(list.begin(), list.end());
    model->setStringList(list);
    ui->produtosLV->setModel(model);

    if (list.empty()) return;
    if (ui->modelosLV->selectionModel() == nullptr) return;

    // Conectar a função onProdutosLVClicked ao evento de clique na ListView para mostrar os modelos de um produto na linha
    connect(ui->modelosLV->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainMenu::onProdutosLVClicked);
}

// Função para mostrar os modelos na ListView baseado no produto selecionado na segunda ListView
void MainMenu::setModelos(ListaProdutos *ptr) {
    if (ptr == nullptr) {
        ui->modelosLV->setModel(nullptr);
        return;
    }

    auto *model = new QStringListModel(this);
    QStringList list;

    ListaProdutos *current = ptr;
    while (current != nullptr) {
        Produto produto = *current->produto;
        QString produtoStr = produto.modelo;
        list << produtoStr;
        current = (ListaProdutos *) current->prox_produto;
    }

    // Ordenar a lista de modelos
    std::sort(list.begin(), list.end());
    model->setStringList(list);
    ui->modelosLV->setModel(model);

    if (list.empty()) return;
    if (ui->modelosLV->selectionModel() == nullptr) return;

    // Conectar a função onModelosLVClicked ao evento de clique na ListView para mostrar a informação de um modelo
    connect(ui->modelosLV->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainMenu::onModelosLVClicked);
}

// Função para mostrar a informação de um modelo na janela
void MainMenu::setModeloInfo(Produto *ptr) {
    if (ptr == nullptr) {
        ui->codigoModeloText->clear();
        ui->nomeModeloText->clear();
        ui->quantModeloText->clear();
        ui->precoModeloText->clear();
        auto *model = new QStandardItemModel(0, 2, this);
        model->setHorizontalHeaderItem(0, new QStandardItem("Nome"));
        model->setHorizontalHeaderItem(1, new QStandardItem("Valor"));
        return;
    }

    QString fullNome = QString::fromStdString(ptr->nome) + " " + QString::fromStdString(ptr->modelo);
    ui->codigoModeloText->setText(QString::number(ptr->linhaID) + "." + QString::number(ptr->produtoID));
    ui->nomeModeloText->setText(fullNome);
    ui->quantModeloText->setText(QString::number(ptr->quantidade));
    ui->precoModeloText->setText(QString::number(ptr->preco).replace('.',',') + " €");

    auto *model = new QStandardItemModel(static_cast<int>(ptr->num_parametros), 2, this);
    model->setHorizontalHeaderItem(0, new QStandardItem("Nome"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Valor"));
    ListaParamAdicionalProduto* current = ptr->parametros;
    int i = 0;
    while (current != nullptr) {
        ParamAdicionalProduto param = *current->parametro;
        model->setItem(i, 0, new QStandardItem(param.nome));
        model->setItem(i, 1, new QStandardItem(param.valor));
        current = current->prox_parametro;
        i++;
    }
}

// Função para mostrar a informação do stock na tab Definições
void MainMenu::tabDefinicoes() {
    ui->nomeLojaModText->setText(ui->nomeLojaLab->text());
    ui->numLinhasStockText->setText(QString::number(stock->num_linhas));
    ui->numTiposStockText->setText(QString::number(getNumeroTipoProdutosStock(stock)));  //implementado em C
    ui->numProdutosStockText->setText(QString::number(getNumeroProdutosStock(stock)));  //implementado em C
}

// Função para recarregar as tabs (dados estáticos) (a implementar para o resto)
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

// Função para mostrar os produtos na ListView baseado na linha de produtos selecionada na primeira ListView
void MainMenu::onLinhasLVClicked(const QModelIndex &index) {
    selLinha = index.row();
    auto linha = (LinhaProdutos *) obterLinhaProdutosPorID(stock, selLinha + 1);
    setProdutos(linha);
    setModelos(nullptr);
    setModeloInfo(nullptr);
}

// Função para mostrar os modelos na ListView baseado no produto selecionado na segunda ListView
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
            adicionarParametroAdicionalProduto(&parametros, parametro);

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

// Função para remover um produto de uma linha de produtos
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
            adicionarParametroAdicionalProduto(&parametros, parametro);

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
    if (resultado == 0) {
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
    ui->statusbar->showMessage("Produtos de stock apagados. Linhas foram mantidas.");
}

// Processamento de mudança de tab
void MainMenu::onTabChanged(int index) {
    if (index == 0) { // stock
        setLinhas(stock);
        setProdutos(nullptr);
        setModelos(nullptr);
    } else if (index == 1) { // pesquisa
        ui->procuraTE->clear();
        ui->resultPesqTV->clear();
    } else if (index == 2) { // definições
        MainMenu::reloadTabs();
    }
}

#ifdef Q_OS_MACOS
// Função para definir o menu nativo do macOS
// Compatibilidade desconhecida abaixo de macOS Mojave!!
void MainMenu::createMacMenu()
{
    QMenuBar* menuBar = ui->menubar;
    menuBar->setNativeMenuBar(true);
}

#endif
