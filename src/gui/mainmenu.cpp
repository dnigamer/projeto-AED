#include <QMessageBox>
#include <QFile>
#include "mainmenu.h"
#include "ui_MainMenu.h"

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

    // Mostar mensagem de "Ready" na barra de estado
    if (!problemas)
        statusBar()->showMessage("Pronto");
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::setStock(StockLoja *ptr) {
    // TODO: Implementar a função para mostrar os produtos
    ui->nomeLojaLab->setText(ptr->nome);
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