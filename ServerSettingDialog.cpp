#include "ServerSettingDialog.h"
#include "ui_ServerSettingDialog.h"

#include <QMessageBox>

ServerSettingDialog::ServerSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerSettingDialog)
{
    ui->setupUi(this);

    settings = new QSettings("settingUser.ini", QSettings::IniFormat);

    settings->beginGroup("Database");
    ui->hostEdit->setText(settings->value("host").toString());
    ui->portEdit->setText(settings->value("port").toString());
    ui->databaseEdit->setText(settings->value("database").toString());
    ui->loginEdit->setText(settings->value("username").toString());
    settings->endGroup();
}

ServerSettingDialog::~ServerSettingDialog()
{
    delete ui;
}

void ServerSettingDialog::on_editButton_clicked()
{
    settings->beginGroup("Database");
    settings->setValue("host", ui->hostEdit->text());
    settings->setValue("port", ui->portEdit->text().toInt());
    settings->setValue("database", ui->databaseEdit->text());
    settings->setValue("username", ui->loginEdit->text());
    if(!ui->passwordEdit->text().isEmpty()) settings->setValue("password", ui->passwordEdit->text());
    settings->endGroup();

    QMessageBox msg;
    msg.setWindowTitle(tr("Налаштування серверу"));
    msg.setText("Зміни будуть внесені\nпісля перезапуску програми");
    close();
}

void ServerSettingDialog::on_cancelButton_clicked()
{
    close();
}
