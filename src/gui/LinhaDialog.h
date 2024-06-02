//
// Created by Gonçalo Miranda on 02/06/2024.
//

#ifndef PROJETO_LINHADIALOG_H
#define PROJETO_LINHADIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class LinhaDialog; }
QT_END_NAMESPACE

class LinhaDialog : public QDialog {
    Q_OBJECT

public:
    explicit LinhaDialog(QWidget *parent = nullptr);

    void setNomeJanela(int type, const QString &dados);
    void setID(const QString &dados);
    void setNome(const QString &dados);

    QString getNome();

    ~LinhaDialog() override;

private:
    Ui::LinhaDialog *ui;
};


#endif //PROJETO_LINHADIALOG_H
