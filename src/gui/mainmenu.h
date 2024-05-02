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

    [[maybe_unused]] void onListViewItemClicked(const QModelIndex &index);

public:
    explicit MainMenu(QWidget *parent = nullptr);

    void setStock(StockLoja *ptr);

    ~MainMenu() override;

private:
    Ui::MainMenu *ui;

    void open();
    void save();

    #ifdef Q_OS_MACOS
    void createMacMenu();
    #endif

    void quit();

    void setItens(LinhaProdutos *ptr);
};


#endif //QTTESTS_MAINMENU_H
