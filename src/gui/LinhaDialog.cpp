//
// Created by Gonçalo Miranda on 02/06/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LinhaDialog.h" resolved

#include "LinhaDialog.h"
#include "ui_LinhaDialog.h"


LinhaDialog::LinhaDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::LinhaDialog) {
    ui->setupUi(this);
}

void LinhaDialog::setNomeJanela(int type, const QString &dados) {
    if (type == 0) {
        this->setWindowTitle("Adicionar Linha de Produtos");
        ui->dialogTitle->setText("Adicionar Linha de Produtos");
    } else if (type == 1) {
        this->setWindowTitle("Atualizar Linha de Produtos");
        ui->dialogTitle->setText("Atualizar Linha de Produtos: " + dados);
    }
}

void LinhaDialog::setID(const QString &dados) {
    ui->IDText->setText(dados);
}

void LinhaDialog::setNome(const QString &dados) {
    ui->nomeText->setText(dados);
}

QString LinhaDialog::getNome() {
    return ui->nomeText->text();
}

LinhaDialog::~LinhaDialog() {
    delete ui;
}
