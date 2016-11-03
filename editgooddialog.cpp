#include "editgooddialog.h"
#include "ui_editgooddialog.h"
#include <QMessageBox>

EditGoodDialog::EditGoodDialog(int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q), goodId(id),
    ui(new Ui::EditGoodDialog)
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

    query->prepare("SELECT goods.code, goods.name AS nameG, providers.name AS nameP, goods.price, goods.pricePDV, goods.count "
                   "FROM goods, providers "
                   "where goods.idprovider = providers.idprovider AND goods.idgood = :id");
    query->bindValue(":id", goodId);
    query->exec();
    query->next();
    ui->codEdit->setText(query->value("code").toString());
    ui->nameEdit->setText(query->value("nameG").toString());
    ui->providerCombo->setCurrentText(query->value("nameP").toString());
    ui->priceSpinBox->setValue(query->value("price").toDouble());
    ui->pricePDVSpinBox->setValue(query->value("pricePDV").toDouble());
    ui->countSpinBox->setValue(query->value("count").toDouble());
}

EditGoodDialog::~EditGoodDialog()
{
    delete ui;
}

void EditGoodDialog::on_editButton_clicked()
{
    query->prepare("UPDATE `goods` "
                   "SET `code` = :code, `name` = :name, `idprovider` = :idprovider, `price` = :price, `pricePDV` = :pricePDV, `count` = :count "
                   "WHERE `idgood`= :idgood");
    query->bindValue(":code", ui->codEdit->text());
    query->bindValue(":name", ui->nameEdit->text());
    query->bindValue(":idprovider", providersId[ui->providerCombo->currentIndex()]);
    query->bindValue(":price", ui->priceSpinBox->value());
    query->bindValue(":pricePDV", ui->pricePDVSpinBox->value());
    query->bindValue(":count", ui->countSpinBox->value());
    query->bindValue(":idgood", goodId);
    if(query->exec())
    {
        close();
    }
    else
    {
        QMessageBox errorMsg;
        errorMsg.setWindowTitle("Помилка");
        errorMsg.setText("Помилка оновлення товару");
        errorMsg.exec();
    }
}

void EditGoodDialog::on_cancelButton_clicked()
{
    close();
}

void EditGoodDialog::on_priceSpinBox_valueChanged(double arg1)
{
    ui->pricePDVSpinBox->setValue(arg1 + arg1 * 0.2);
}
