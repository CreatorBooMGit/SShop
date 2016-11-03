#ifndef ADDGOODDIALOG_H
#define ADDGOODDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class AddGoodDialog;
}

class AddGoodDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddGoodDialog(QSqlQuery *q, QWidget *parent = 0);
    ~AddGoodDialog();

private slots:
    void on_addButton_clicked();
    void on_cancelButton_clicked();
    void on_priceSpinBox_valueChanged(double arg1);

private:
    Ui::AddGoodDialog *ui;
    QSqlQuery *query;
    QVector <int> providersId;
};

#endif // ADDGOODDIALOG_H
