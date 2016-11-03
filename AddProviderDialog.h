#ifndef ADDPROVIDERDIALOG_H
#define ADDPROVIDERDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class AddProviderDialog;
}

class AddProviderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddProviderDialog(QSqlQuery *q, QWidget *parent = 0);
    ~AddProviderDialog();

private slots:
    void on_OKButton_clicked();  

    void on_cancelButton_clicked();

private:
    Ui::AddProviderDialog *ui;
    QSqlQuery *query;
};

#endif // ADDPROVIDERDIALOG_H
