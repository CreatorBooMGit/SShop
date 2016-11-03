#include "EditWorkerDialog.h"
#include "ui_EditWorkerDialog.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QMessageBox>

EditWorkerDialog::EditWorkerDialog(int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q), workerId(id),
    ui(new Ui::EditWorkerDialog)
{
    ui->setupUi(this);

//    query = q;
//    workerId = id;
    query->prepare("SELECT positions.idposition, positions.position "
                   "FROM positions");
    query->exec();
    while(query->next())
    {
        positionId.push_back(query->value(0).toInt());
        ui->positionCombo->addItem(query->value(1).toString());
    }

    query->prepare( "SELECT workers.surname, workers.name, workers.patronymic, workers.phone, positions.position, workers.login, workers.password "
                    "FROM workers, positions "
                    "where workers.idposition = positions.idposition and workers.idworker = :id");
    query->bindValue(":id", workerId);
    query->exec();
    query->next();
    ui->surnameEdit->setText(query->value("surname").toString());
    ui->nameEdit->setText(query->value("name").toString());
    ui->patronymicEdit->setText(query->value("patronymic").toString());
    ui->phoneEdit->setText(query->value("phone").toString());
    ui->positionCombo->setCurrentText(query->value("position").toString());
    ui->loginEdit->setText(query->value("login").toString());
}

EditWorkerDialog::~EditWorkerDialog()
{
    qDebug() << "Скрылся";
    delete ui;
}

void EditWorkerDialog::on_OKButton_clicked()
{
    QString password = ui->passwordEdit->text();
    password.append("AUidhsuairw3hj89ruu8s9duj42");
//    qDebug() << QCryptographicHash::hash(ui->passwordEdit->text().toLatin1(), QCryptographicHash::Sha3_512).toHex();
    query->prepare("UPDATE `workers` "
                   "SET `idposition` = :idposition, `login` = :login, `password` = :password, `name` = :name, `surname` = :surname, `patronymic` = :patronymic, `phone` = :phone "
                    "WHERE `idworker` = :id");
    query->bindValue(":idposition", positionId[ui->positionCombo->currentIndex()]);
    query->bindValue(":name", ui->nameEdit->text());
    query->bindValue(":login", ui->loginEdit->text());
    query->bindValue(":password", QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Sha3_512).toHex());
    query->bindValue(":surname", ui->surnameEdit->text());
    query->bindValue(":patronymic", ui->patronymicEdit->text());
    query->bindValue(":phone", ui->phoneEdit->text());
    query->bindValue(":id", workerId);
    if(query->exec())
    {
        close();
    }
    else
    {
        QMessageBox errorMsg;
        errorMsg.setWindowTitle("Помилка");
        errorMsg.setText("Помилка оновлення даних про працівника");
        errorMsg.exec();
    }
}


void EditWorkerDialog::on_cancelButton_clicked()
{
    close(); // Скройся
}
