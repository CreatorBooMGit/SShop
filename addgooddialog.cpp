#include "addgooddialog.h"
#include "ui_addgooddialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
AddGoodDialog::AddGoodDialog(QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::AddGoodDialog)
{
    ui->setupUi(this);

    query->prepare("SELECT providers.idprovider, providers.name "
                   "FROM providers");
    query->exec();
    while(query->next())
    {
        providersId.push_back(query->value(0).toInt());
        ui->providerCombo->addItem(query->value(1).toString());
    }
}

AddGoodDialog::~AddGoodDialog()
{
    delete ui;
}

void AddGoodDialog::on_addButton_clicked()
{
    query->prepare("INSERT INTO `goods` (`code`, `name`, `idprovider`, `price`, `pricePDV`, `count`) "
                    "VALUES (:code, :name, :idprovider, :price, :pricePDV, :count)");
    query->bindValue(":code", ui->codEdit->text());
    query->bindValue(":name", ui->nameEdit->text());
    query->bindValue(":idprovider", providersId[ui->providerCombo->currentIndex()]);
    query->bindValue(":price", ui->priceSpinBox->value());
    query->bindValue(":pricePDV", ui->pricePDVSpinBox->value());
    query->bindValue(":count", ui->countSpinBox->value());
    if(query->exec())
    {
        close();
    }
    else
    {
        QMessageBox errorMsg;
        errorMsg.setWindowTitle("Помилка");
        errorMsg.setText("Помилка додавання товару");
        errorMsg.exec();
    }

}

void AddGoodDialog::on_cancelButton_clicked()
{
    close();
}

void AddGoodDialog::on_priceSpinBox_valueChanged(double arg1)
{
    ui->pricePDVSpinBox->setValue(arg1 * 0.2 + arg1);
}
