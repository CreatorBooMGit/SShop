#include "authdialog.h"
#include "ui_authdialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QCryptographicHash>
#include <QCloseEvent>

AuthDialog::AuthDialog(user *u, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q), userInfo(u),
    ui(new Ui::AuthDialog)
{
    ui->setupUi(this);
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

void AuthDialog::on_cancelButton_clicked()
{
    exit(0);
}

void AuthDialog::on_confirmButton_clicked()
{
    QString login = ui->loginEdit->text();
    QString password = ui->passwordEdit->text();
    password.append("AUidhsuairw3hj89ruu8s9duj42");

    query->prepare("SELECT * "
                   "FROM workers "
                   "WHERE workers.login = :login");
    query->bindValue(":login", login);
    qDebug() << query->exec();

    if(query->size() <= 0)
    {
        QMessageBox msg;
        msg.setWindowTitle("Помилка");
        msg.setText("Не вірний логін");
        msg.exec();
        ui->loginEdit->clear();
        ui->passwordEdit->clear();
        ui->loginEdit->setFocus();
        return;
    }

    query->next();
//    qDebug() << "passwordHex:" << QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Sha3_512).toHex();
    if(query->value("password").toString().compare(QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Sha3_512).toHex()) == 0)
    {
        userInfo->iduser = query->value(0).toInt();
        userInfo->login = login;
        userInfo->postId = query->value(1).toInt();
//        qDebug() << "Я был здесь";
        reject();
    }
    else
    {
//        qDebug() << "И здесь тоже";
        QMessageBox msg;
        msg.setWindowTitle("Помилка");
        msg.setText("Не вірний пароль");
        msg.exec();
        ui->passwordEdit->clear();
        ui->passwordEdit->setFocus();
    }
}

void AuthDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
}

void AuthDialog::on_loginEdit_returnPressed()
{
    ui->passwordEdit->setFocus();
}

void AuthDialog::on_passwordEdit_returnPressed()
{

}
