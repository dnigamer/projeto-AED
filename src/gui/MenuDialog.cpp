//
// Created by Gonçalo Miranda on 31/05/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MenuDialog.h" resolved

#include "MenuDialog.h"
#include "ui_MenuDialog.h"

MenuDialog::MenuDialog(QWidget *parent) : QDialog(parent), ui(new Ui::MenuDialog) {
    ui->setupUi(this);

   connect(ui->addParam, &QPushButton::clicked, [this]() {
        int row = ui->tabParam->rowCount();
        ui->tabParam->setRowCount(row + 1);
        ui->tabParam->setItem(row, 0, new QTableWidgetItem(""));
        ui->tabParam->item(row, 0)->setFlags(ui->tabParam->item(row, 0)->flags() | Qt::ItemIsEditable);
        ui->tabParam->setCurrentCell(row, 0);
        ui->tabParam->editItem(ui->tabParam->item(row, 0));
    });

    connect(ui->delParam, &QPushButton::clicked, [this]() {
        int row = ui->tabParam->currentRow();
        if (row != -1) {
            ui->tabParam->removeRow(row);
        }
    });

}

void MenuDialog::setNomeJanela(int type, const QString &dados) {
    switch (type) {
        case 1:
            ui->dialogTitle->setText("Adicionar produto a linha de produtos: " + dados);
            setWindowTitle("Adicionar produto");
            break;
        case 2:
            ui->dialogTitle->setText("Atualizar produto: " + dados);
            setWindowTitle("Atualizar produto");
            break;
        default:
            ui->dialogTitle->setText("Adicionar produto a linha de produtos: " + dados);
            setWindowTitle("Adicionar produto");
            break;
    }
}

void MenuDialog::setID(const QString &dados) {
    ui->IDText->setText(dados);
}

void MenuDialog::setNome(const QString &dados) {
    ui->nomeText->setText(dados);
}

void MenuDialog::setModelo(const QString &dados) {
    ui->modeloText->setText(dados);
}

void MenuDialog::setCategoria(const QString &dados) {
    ui->categoriaText->setText(dados);
}

void MenuDialog::setQuantidade(const int quantidade) {
    ui->quantidadeSpin->setValue(quantidade);
}

void MenuDialog::setPreco(const QString &dados) {
    ui->precoText->setText(dados + " €");
}

void MenuDialog::setParametros(const QString &dados) {
    QStringList parametros = dados.split(";");
    ui->tabParam->setRowCount(static_cast<int>(parametros.size()));
    ui->tabParam->setColumnCount(1);
    ui->tabParam->setHorizontalHeaderLabels(QStringList() << "Parâmetros");
    for (int i = 0; i < parametros.size(); i++) {
        ui->tabParam->setItem(i, 0, new QTableWidgetItem(parametros.at(i)));
    }
}

void MenuDialog::addParametro(const QString &nome, const QString &valor) {
    int row = ui->tabParam->rowCount();
    ui->tabParam->setRowCount(row + 1);
    ui->tabParam->setItem(row, 0, new QTableWidgetItem(nome));
    ui->tabParam->setItem(row, 1, new QTableWidgetItem(valor));
    ui->tabParam->item(row, 0)->setFlags(ui->tabParam->item(row, 0)->flags() | Qt::ItemIsEditable);
    ui->tabParam->item(row, 1)->setFlags(ui->tabParam->item(row, 1)->flags() | Qt::ItemIsEditable);
}

QString MenuDialog::getNome() {
    return ui->nomeText->text();
}

QString MenuDialog::getModelo() {
    return ui->modeloText->text();
}

QString MenuDialog::getCategoria() {
    return ui->categoriaText->text();
}

int MenuDialog::getQuantidade() {
    return ui->quantidadeSpin->value();
}

float MenuDialog::getPreco() {
    QString priceString = ui->precoText->text().remove(" €");
    bool ok;
    float price = priceString.toFloat(&ok);
    if (ok) {
        return price;
    } else {
        return 0.0f;
    }
}

QString MenuDialog::getParametros() {
    QString parametros;
    for (int i = 0; i < ui->tabParam->rowCount(); i++) {
        parametros += ui->tabParam->item(i, 0)->text() + ";";
    }
    return parametros;
}

unsigned int MenuDialog::getNumeroParametros() {
    return ui->tabParam->rowCount();
}

QString MenuDialog::getNomeParametro(int index) {
    return ui->tabParam->item(index, 0)->text();
}

QString MenuDialog::getValorParametro(int index) {
    return ui->tabParam->item(index, 1)->text();
}

MenuDialog::~MenuDialog() {
    delete ui;
}