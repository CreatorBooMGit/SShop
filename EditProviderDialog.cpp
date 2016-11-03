#include "EditProviderDialog.h"
#include "ui_EditProviderDialog.h"
#include <QMessageBox>

EditProviderDialog::EditProviderDialog(int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q), providerID(id),
    ui(new Ui::EditProviderDialog)
{
    ui->setupUi(this);

    query->prepare("SELECT * FROM providers "
                   "WHERE idprovider = :provider");
    query->bindValue(":provider", providerID);
    if(query->exec())
    {
        query->next();
        ui->nameEdit->setText(query->value("name").toString());
        ui->cityEdit->setText(query->value("city").toString());
        ui->phoneEdit->setText(query->value("phone").toString());
    }
}

EditProviderDialog::~EditProviderDialog()
{
    delete ui;
}
// історія та розвиток HTML, CSS, PHP!!!!
void EditProviderDialog::on_OKButton_clicked()
{
    query->prepare("UPDATE `providers` "
                   "SET `name` = :name , `city` = :city, `phone` = :phone "
                   "WHERE `idprovider` = :idprovider");
    query->bindValue(":name",ui->nameEdit->text());
    query->bindValue(":city", ui->cityEdit->text());
    query->bindValue(":phone", ui->phoneEdit->text());
    query->bindValue(":idprovider", providerID);
    if (query->exec())
    {
        close();
    }
    else
    {
        QMessageBox errorMsg;
        errorMsg.setWindowTitle("Помилка");
        errorMsg.setText("Помилка додавання постачальників");
        errorMsg.exec();
    }
}

void EditProviderDialog::on_cancelButton_clicked()
{
    close();
}
