//
// Created by Gonçalo Miranda on 29/04/2024.
//

#ifndef QTTESTS_MAINMENU_H
#define QTTESTS_MAINMENU_H

#include <QWidget>
#include <QMainWindow>
#include "../operacoes.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

class MainMenu : public QMainWindow {
    Q_OBJECT

public slots:

    void onLinhasLVClicked(const QModelIndex &index);
    void onProdutosLVClicked(const QModelIndex &index);

    void onAtualizarStockInfoBtnClicked();
    void onNomeLojaModBtnClicked();
    void onApagaLinhasStockBtnClicked();
    void onApagaProdutosStockBtnClicked();


public:
    explicit MainMenu(QWidget *parent = nullptr);

    void setStock(StockLoja *stock);
    void setLinhas(StockLoja *ptr);
    void setProdutos(LinhaProdutos *ptr);
    void setModelos(ListaProdutos *ptr);

    void tabDefinicoes();
    void reloadTabs();

    ~MainMenu() override;

private:
    Ui::MainMenu *ui;

    void open();
    void save();

    #ifdef Q_OS_MACOS
    void createMacMenu();
    #endif

    void quit();

    void onTabChanged(int index);
};


#endif //QTTESTS_MAINMENU_H
