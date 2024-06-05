//
// Created by Gonçalo Miranda on 29/04/2024.
//

#ifndef QTTESTS_MAINMENU_H
#define QTTESTS_MAINMENU_H

#include <QWidget>
#include <QMainWindow>
#include <QStandardItemModel>
#include "MenuDialog.h"
#include "LinhaDialog.h"

#include "../operacoes.h"
#include "../storage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

class MainMenu : public QMainWindow {
    Q_OBJECT

public slots:
    void onLinhasLVClicked(const QModelIndex &index);
    void onProdutosLVClicked(const QModelIndex &index);
    void onModelosLVClicked(const QModelIndex &index);

    void onAdicionarLinhaBtnClicked();
    void onAdicionarProdutoBtnClicked();
    void onRemoverLinhaBtnClicked();
    void onRemoverProdutoBtnClicked();
    void onAtualizarLinhaBtnClicked();
    void onAtualizarProdutoBtnClicked();

    void onAtualizarStockInfoBtnClicked();
    void onNomeLojaModBtnClicked();
    void onApagaLinhasStockBtnClicked();
    void onApagaProdutosStockBtnClicked();

    void onProcuraBtnClicked();


public:
    explicit MainMenu(QWidget *parent = nullptr);

    void setStock(StockLoja *stock);
    void setLinhas(StockLoja *ptr);
    void setProdutos(LinhaProdutos *ptr);
    void setModelos(ListaProdutos *ptr);
    void setModeloInfo(Produto *ptr);

    void tabDefinicoes();
    void reloadTabs();

    static int criarWarningMessageBox(const QString &title, const QString &text, int mode);

    ~MainMenu() override;

private:
    Ui::MainMenu *ui;

    void openDB();
    void saveDB();
    void closeDB();
    void newDB();

#ifdef Q_OS_MACOS
    void createMacMenu();

    #endif

    void quit();

    void onTabChanged(int index);
};


#endif //QTTESTS_MAINMENU_H
