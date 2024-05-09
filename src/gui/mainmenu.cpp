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

    // Conectar funções às ações
    connect(actionOpen, &QAction::triggered, this, &MainMenu::open);
    connect(actionSave, &QAction::triggered, this, &MainMenu::save);
    connect(actionExit, &QAction::triggered, this, &MainMenu::quit);

    // Assinar ações aos itens de menu
    ui->menuFicheiro->addAction(actionOpen);
    ui->menuFicheiro->addAction(actionSave);
    ui->menuFicheiro->addSeparator();
    ui->menuFicheiro->addAction(actionExit);

    // operações sobre ListViews
    ui->linhasLV->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->produtosLV->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->modelosLV->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Listeners para ListViews
    connect(ui->linhasLV, &QListView::clicked, this, &MainMenu::onLinhasLVClicked);
    connect(ui->produtosLV, &QListView::clicked, this, &MainMenu::onProdutosLVClicked);
    //connect(ui->modelosLV, &QListView::clicked, this, &MainMenu::onModelosLVClicked);

    // Listeners para botoes
    connect(ui->nomeLojaModBtn, &QPushButton::clicked, this, &MainMenu::onNomeLojaModBtnClicked);
    connect(ui->atualizarStockInfoBtn, &QPushButton::clicked, this, &MainMenu::onAtualizarStockInfoBtnClicked);

    // Listeners para tabgroups
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainMenu::onTabChanged);

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

    if (!problemas) statusBar()->showMessage("Pronto");
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::setStock(StockLoja *ptr) {
    if (ptr == nullptr) return;
    stock = ptr;
    ui->nomeLojaLab->setText(ptr->nome);
}

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
    connect(ui->linhasLV->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainMenu::onLinhasLVClicked);
}

void MainMenu::setProdutos(LinhaProdutos *ptr) {
    if (ptr == nullptr) return;

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

    model->setStringList(list);
    ui->produtosLV->setModel(model);

    if (list.empty()) return;
    if (ui->modelosLV->selectionModel() == nullptr) return;
    connect(ui->modelosLV->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainMenu::onProdutosLVClicked);
}

void MainMenu::setModelos(ListaProdutos *ptr) {
    if (ptr == nullptr) ui->modelosLV->setModel(nullptr);

    auto *model = new QStringListModel(this);
    QStringList list;

    ListaProdutos *current = ptr;
    while (current != nullptr) {
        Produto produto = *current->produto;
        QString produtoStr = produto.modelo;
        list << produtoStr;
        current = (ListaProdutos *) current->prox_produto;
    }

    model->setStringList(list);
    ui->modelosLV->setModel(model);

    if (list.empty()) return;
    //if (ui->modelosLV->selectionModel() == nullptr) return;
    //connect(ui->modelosLV->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainMenu::onProdutosLVClicked);
}

void MainMenu::tabDefinicoes() {
    ui->nomeLojaModText->setText(ui->nomeLojaLab->text());
    ui->numLinhasStockText->setText(QString::number(stock->num_linhas));
    ui->numTiposStockText->setText(QString::number(getNumeroTipoProdutosStock(stock)));
    ui->numProdutosStockText->setText(QString::number(getNumeroProdutosStock(stock)));
}

void MainMenu::reloadTabs() {
    tabDefinicoes();
}

void MainMenu::onLinhasLVClicked(const QModelIndex &index) {
    selLinha = index.row();
    auto linha = (LinhaProdutos *) obterLinhaProdutosPorID(stock, selLinha + 1);
    setProdutos(linha);
    setModelos(nullptr);
}

void MainMenu::onProdutosLVClicked(const QModelIndex &index) {
    QAbstractItemModel* model = ui->produtosLV->model();
    QString text = model->data(index, Qt::DisplayRole).toString();
    std::string nomeStd = text.toStdString();
    char* nome = const_cast<char*>(nomeStd.c_str());
    auto lista = (ListaProdutos *) procurarStockPorNomeProduto(stock, nome);
    setModelos(lista);
}

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


void MainMenu::onAtualizarStockInfoBtnClicked() {
    tabDefinicoes();
}

void MainMenu::onTabChanged(int index) {
    if (index == 2) { // definições
        tabDefinicoes();
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
