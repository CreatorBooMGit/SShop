#ifndef ADDPURCHASEDIALOG_H
#define ADDPURCHASEDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QVector>
#include "usertypes.h"

namespace Ui {
class AddPurchaseDialog;
}

class AddPurchaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPurchaseDialog(QSqlQuery *q, QWidget *parent = 0);
    ~AddPurchaseDialog();

signals:
    void sendPurchase(purchase tmp);

private slots:
    void on_nameBox_currentIndexChanged(int index);
    void on_countSpin_valueChanged(int arg1);
    void on_pushButton_clicked();

    void on_canselButton_clicked();

private:
    Ui::AddPurchaseDialog *ui;
    QSqlQuery *query;

    struct good {
        int idgood;
        int code;
        QString providerName;
        QString goodName;
        double price;
        double pricePDV;
    };

    QVector <good> goods;
};

#endif // ADDPURCHASEDIALOG_H
