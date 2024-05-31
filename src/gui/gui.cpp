#include <QApplication>
#include <QFile>
#include "mainmenu.h"
#include "gui.h"

void startGui(int argc, char **argv, StockLoja *stockGeral) {
    QApplication app(argc, argv);
    MainMenu mainMenu;

    // define o nome da janela
    mainMenu.setWindowTitle("Gestão de Stock - " + QString(stockGeral->nome));

    // apresenta as linhas de produtos
    mainMenu.setLinhas(stockGeral);

    // apresenta o stock
    mainMenu.setStock(stockGeral);

    // atualiza as tabs
    mainMenu.reloadTabs();

    // mostra a janela
    mainMenu.show();
    QApplication::exec();
}