#include <QApplication>
#include <QFile>
#include "mainmenu.h"
#include "gui.h"

void startGui(int argc, char **argv, StockLoja *stockGeral) {
    QApplication app(argc, argv);
    MainMenu mainMenu;
    mainMenu.setWindowTitle("Gestão de Stock - " + QString(stockGeral->nome));
    mainMenu.setLinhas(stockGeral);
    mainMenu.setStock(stockGeral);
    mainMenu.reloadTabs();

    //QFile file("MacOS.qss");
    //file.open(QFile::ReadOnly);
    //QString style( file.readAll() );
    //app.setStyleSheet( style );

    mainMenu.show();
    QApplication::exec();
}