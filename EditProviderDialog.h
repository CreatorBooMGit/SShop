#ifndef EDITPROVIDERDIALOG_H
#define EDITPROVIDERDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class EditProviderDialog;
}

class EditProviderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditProviderDialog(int id, QSqlQuery *q, QWidget *parent = 0);
    ~EditProviderDialog();

private slots:
    void on_OKButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::EditProviderDialog *ui;
    QSqlQuery *query;
    int providerID;
};

#endif // EDITPROVIDERDIALOG_H
