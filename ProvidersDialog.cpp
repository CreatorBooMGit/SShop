#include "AddProviderDialog.h"
#include "EditProviderDialog.h"
#include "ProvidersDialog.h"
#include "ui_ProvidersDialog.h"

#include <QMessageBox>

ProvidersDialog::ProvidersDialog(QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::ProvidersDialog)
{
    ui->setupUi(this);
    updateProviders();
}

ProvidersDialog::~ProvidersDialog()
{
    delete ui;
}

void ProvidersDialog::on_addButton_clicked()
{
    AddProviderDialog *addDialog = new AddProviderDialog(query, this);
    addDialog->exec();
    delete addDialog;
    updateProviders();
}

void ProvidersDialog::updateProviders()
{
    query->prepare("SELECT * "
                   "FROM providers ");
    query->exec();
    ui->tableProviders->setRowCount(query->size());
    Providers.clear();
    while(query->next())
    {
        Providers.push_back(query->value(0).toInt());
        for(int i = 1; i < 4; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(query->value(i).toString());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableProviders->setItem( Providers.size() - 1, i - 1, item);
        }
    }
}

void ProvidersDialog::on_editButton_clicked()
{
    EditProviderDialog *editDialog = new EditProviderDialog(Providers[ui->tableProviders->currentRow()], query, this);
    editDialog->exec();
    delete editDialog;
    updateProviders();
}
void ProvidersDialog::on_removeButton_clicked()
{
    QMessageBox confirmMsg;
    confirmMsg.setWindowTitle(tr("Видалення товару"));
    confirmMsg.setText(tr("Підтвердити видалення?"));
    QAbstractButton *confirmMsgButtonYes = confirmMsg.addButton(tr("Підтвердити"), QMessageBox::YesRole);
    QAbstractButton *confirmMsgButtonNo = confirmMsg.addButton(tr("Відміна"), QMessageBox::NoRole);
    confirmMsg.exec();
    if(confirmMsg.clickedButton() == confirmMsgButtonYes)
    {
        query->prepare("DELETE FROM `kursach`.`providers` "
                       "WHERE `idprovider` = :idprovider");
        query->bindValue(":idprovider", Providers[ui->tableProviders->currentRow()]);
        query->exec();
        updateProviders();
    }
}

void ProvidersDialog::on_tableProviders_itemSelectionChanged()
{
    ui->editButton->setEnabled(true);
    ui->removeButton->setEnabled(true);
}
