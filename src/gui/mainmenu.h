//
// Created by Gonçalo Miranda on 29/04/2024.
//

#ifndef QTTESTS_MAINMENU_H
#define QTTESTS_MAINMENU_H

#include <QWidget>
#include <QMainWindow>
#include "../produtos.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

class MainMenu : public QMainWindow {
    Q_OBJECT

public slots:

    void onLinhasLVClicked(const QModelIndex &index);
    void onProdutosLVClicked(const QModelIndex &index);

public:
    explicit MainMenu(QWidget *parent = nullptr);

    void setLinhas(StockLoja *ptr);
    void setProdutos(LinhaProdutos *ptr);
    void setModelos(ListaProdutos *ptr);

    ~MainMenu() override;

private:
    Ui::MainMenu *ui;

    void open();
    void save();

    #ifdef Q_OS_MACOS
    void createMacMenu();
    #endif

    void quit();

};


#endif //QTTESTS_MAINMENU_H
