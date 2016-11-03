#include "AddProviderDialog.h"
#include "ui_AddProviderDialog.h"
#include <QMessageBox>
#include <QSqlQuery>


AddProviderDialog::AddProviderDialog(QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::AddProviderDialog)
{
    ui->setupUi(this);
}

AddProviderDialog::~AddProviderDialog()
{
    delete ui;
}

void AddProviderDialog::on_OKButton_clicked()
{
    query->prepare("INSERT INTO `providers` (`name`, `city`, `phone`) "
                   "VALUES (:name ,:city ,:phone )");
    query->bindValue(":city", ui->cityEdit->text());
    query->bindValue(":name", ui->nameEdit->text());
    query->bindValue(":phone", ui->phoneEdit->text());
    if(query->exec())
    {
        close();
    }
    else
    {
        QMessageBox errorMsg;
        errorMsg.setWindowTitle("Помилка");
        errorMsg.setText("Помилка додавання постачальника");
        errorMsg.exec();
    }
}

void AddProviderDialog::on_cancelButton_clicked()
{
    close();
}
