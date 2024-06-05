#include <QApplication>
#include <QFile>
#include "mainmenu.h"
#include "gui.h"

void startGui(int argc, char **argv, StockLoja *stockGeral) {
    QApplication app(argc, argv);
    MainMenu mainMenu;

    if (stockGeral->lista_linhas == nullptr) {
        mainMenu.setWindowTitle("Gestão de Stock");
        mainMenu.setStock(stockGeral);
    } else {
        mainMenu.setWindowTitle("Gestão de Stock - " + QString(stockGeral->nome));
        mainMenu.setStock(stockGeral);
        mainMenu.setLinhas(stockGeral);
    }

    mainMenu.reloadTabs();
    mainMenu.show();

    QApplication::exec();
}