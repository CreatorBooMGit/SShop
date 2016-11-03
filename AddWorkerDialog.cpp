#include "AddWorkerDialog.h"
#include "ui_AddWorkerDialog.h"
#include <QMessageBox>

AddWorkerDialog::AddWorkerDialog(QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::AddWorkerDialog)
{
    ui->setupUi(this);

    query->prepare("SELECT positions.idposition, positions.position "
                   "FROM positions");
    query->exec();
    while(query->next())
    {
        positionID.push_back(query->value(0).toInt());
        ui->positionCombo->addItem(query->value(1).toString());
    }
}

AddWorkerDialog::~AddWorkerDialog()
{
    delete ui;
}

void AddWorkerDialog::on_OKButton_clicked()
{
    query->prepare("INSERT INTO `kursach`.`workers` (`idposition`, `login`, `password`, `name`, `surname`, `patronymic`, `phone`) "
                   "VALUES (:idposition, :login, :password, :name, :surname, :patronymic, :phone)");
    query->bindValue(":idposition", positionID[ui->positionCombo->currentIndex()]);
    query->bindValue(":name", ui->nameEdit->text());
    query->bindValue(":login", ui->loginEdit->text());
    query->bindValue(":password", ui->passwordEdit->text());
    query->bindValue(":surname", ui->surnameEdit->text());
    query->bindValue(":patronymic", ui->patronymicEdit->text());
    query->bindValue(":phone", ui->phoneEdit->text());
    if(query->exec())
    {
        close();
    }
    else
    {
        QMessageBox errorMsg;
        errorMsg.setWindowTitle("Помилка");
        errorMsg.setText("Помилка додавання працівника");
        errorMsg.exec();
    }

}

void AddWorkerDialog::on_cancelButton_clicked()
{
    close();
}
