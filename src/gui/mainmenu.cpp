#include <QMessageBox>
#include "mainmenu.h"
#include "ui_MainMenu.h"
#include "../produtos.h"

void MainMenu::open()
{
    statusBar()->showMessage("Open action triggered");
}

void MainMenu::save()
{
    statusBar()->showMessage("Save action triggered");
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setStyleSheet("QMessageBox {icon-size: 64px;}");
    int ret = msgBox.exec();
    switch (ret) {
        case QMessageBox::Save:
            statusBar()->showMessage("Save was clicked");
            break;
        case QMessageBox::Discard:
            statusBar()->showMessage("Discard was clicked");
            break;
        case QMessageBox::Cancel:
            statusBar()->showMessage("Cancel was clicked");
            break;
        default:
            break;
    }

}

void MainMenu::quit()
{
    statusBar()->showMessage("Quit action triggered");
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Are you sure you want to quit?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setStyleSheet("QMessageBox {icon-size: 64px;}");
    int ret = msgBox.exec();
    switch (ret) {
        case QMessageBox::Yes:
            QMainWindow::close();
            break;
        case QMessageBox::No:
            statusBar()->showMessage("No was clicked");
            break;
        default:
            break;
    }
}

MainMenu::MainMenu(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainMenu) {
    ui->setupUi(this); // Set up the UI

    #ifdef Q_OS_MACOS
        createMacMenu();
    #endif

    // Create actions
    auto *actionOpen = new QAction(tr("Abrir base de dados"), this);
    actionOpen->setShortcut(QKeySequence::Open);
    auto *actionSave = new QAction(tr("Salvar para base de dados"), this);
    actionSave->setShortcut(QKeySequence::Save);
    auto *actionExit = new QAction(tr("Sair"), this);
    actionExit->setShortcut(QKeySequence::Quit);

    // Connect actions to slots
    connect(actionOpen, &QAction::triggered, this, &MainMenu::open);
    connect(actionSave, &QAction::triggered, this, &MainMenu::save);
    connect(actionExit, &QAction::triggered, this, &MainMenu::quit);

    // Add actions to the toolbar
    ui->menuFicheiro->addAction(actionOpen);
    ui->menuFicheiro->addAction(actionSave);
    ui->menuFicheiro->addSeparator(); // Add a separator between actions
    ui->menuFicheiro->addAction(actionExit);

    statusBar()->showMessage("Ready"); // Display initial message on the status bar
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::setStock(StockLoja *ptr) {
}

#ifdef Q_OS_MACOS
void MainMenu::createMacMenu()
{
    QMenuBar* menuBar = ui->menubar;
    menuBar->setNativeMenuBar(true);
}

#endif