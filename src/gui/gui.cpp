#include <QApplication>
#include "mainmenu.h"
#include "gui.h"

void startGui(int argc, char **argv, StockLoja *stockGeral) {
    QApplication app(argc, argv);
    MainMenu mainMenu;
    mainMenu.setWindowTitle("Gestão de Stock - " + QString(stockGeral->nome));
    mainMenu.setLinhas(stockGeral);
    mainMenu.show();
    QApplication::exec();
}