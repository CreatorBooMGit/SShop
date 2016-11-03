#include "addpurchasedialog.h"
#include "ui_addpurchasedialog.h"
#include <QDebug>
#include <QMessageBox>

AddPurchaseDialog::AddPurchaseDialog(QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::AddPurchaseDialog)
{
    ui->setupUi(this);
    query->prepare("SELECT goods.idgood, goods.code, providers.name AS nameP, goods.name AS nameG, goods.price, goods.pricePDV "
                   "FROM goods, providers "
                   "WHERE goods.idprovider = providers.idprovider");
    bool exec = query->exec();
    qDebug() << "addPurchase -> query:" << exec;
    while(query->next())
    {
        good tmp;
        tmp.idgood = query->value("idgood").toInt();
        tmp.code = query->value("code").toInt();
        tmp.goodName = query->value("nameG").toString();
        tmp.providerName = query->value("nameP").toString();
        tmp.price = query->value("price").toDouble();
        tmp.pricePDV = query->value("pricePDV").toDouble();
        goods.push_back(tmp);
        ui->nameBox->addItem(tmp.goodName);
    }
}

AddPurchaseDialog::~AddPurchaseDialog()
{
//    QMessageBox msg;
//    msg.setText("А я тебе верил =(");
//    msg.exec();
    delete ui;
}

void AddPurchaseDialog::on_nameBox_currentIndexChanged(int index)
{
    ui->providerEdit->setText(goods[index].providerName);
    ui->codEdit->setText(QString::number(goods[index].code));
    ui->priceSpinBox->setValue(goods[index].price);
    ui->pricePDVSpinBox->setValue(goods[index].pricePDV);
}

void AddPurchaseDialog::on_countSpin_valueChanged(int arg1)
{
    ui->sumSpin->setValue(arg1 * ui->pricePDVSpinBox->value());
}

void AddPurchaseDialog::on_pushButton_clicked()
{
    if(ui->countSpin->value() <= 0)
    {
        QMessageBox msg;
        msg.setWindowTitle("Помилка");
        msg.setText("Кількість = 0");
        msg.exec();
        return;
    }

    purchase tmp;
    tmp.goodId = goods[ui->nameBox->currentIndex()].idgood;
    tmp.goodCode = ui->codEdit->text().toInt();
    tmp.goodName = ui->nameBox->currentText();
    tmp.goodProvider = ui->providerEdit->text();
    tmp.goodPrice = ui->priceSpinBox->value();
    tmp.goodPricePDV = ui->pricePDVSpinBox->value();
    tmp.goodCount = ui->countSpin->value();
    tmp.sum = ui->sumSpin->value();
    emit sendPurchase(tmp);
    close();
}

void AddPurchaseDialog::on_canselButton_clicked()
{
    close();
}
