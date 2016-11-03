#ifndef EDITGOODDIALOG_H
#define EDITGOODDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class EditGoodDialog;
}

class EditGoodDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditGoodDialog(int id, QSqlQuery *q, QWidget *parent = 0);
    ~EditGoodDialog();

private slots:
    void on_editButton_clicked();

    void on_cancelButton_clicked();

    void on_priceSpinBox_valueChanged(double arg1);

private:
    Ui::EditGoodDialog *ui;
    int goodId;
    QVector <int> providersId;
    QSqlQuery *query;
};

#endif // EDITGOODDIALOG_H
