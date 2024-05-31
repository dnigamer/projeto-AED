//
// Created by Gonçalo Miranda on 31/05/2024.
//

#ifndef PROJETO_MENUDIALOG_H
#define PROJETO_MENUDIALOG_H

#include <QDialog>
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MenuDialog; }
QT_END_NAMESPACE

class MenuDialog : public QDialog {
    Q_OBJECT

public:
    explicit MenuDialog(QWidget *parent = nullptr);

    void setNomeJanela(int type, const QString &dados);
    void setID(const QString &dados);
    void setNome(const QString &dados);
    void setModelo(const QString &dados);
    void setCategoria(const QString &dados);
    void setQuantidade(const int quantidade);
    void setPreco(const QString &dados);
    void setParametros(const QString &dados);
    void addParametro(const QString &nome, const QString &valor);

    QString getNome();
    QString getModelo();
    QString getCategoria();
    int getQuantidade();
    float getPreco();
    QString getParametros();
    unsigned int getNumeroParametros();
    QString getNomeParametro(int index);
    QString getValorParametro(int index);

    ~MenuDialog() override;

private:
    Ui::MenuDialog *ui{};
};


#endif //PROJETO_MENUDIALOG_H
