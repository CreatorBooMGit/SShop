#include "ServerSettingDialog.h"
#include "ui_ServerSettingDialog.h"

ServerSettingDialog::ServerSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerSettingDialog)
{
    ui->setupUi(this);
}

ServerSettingDialog::~ServerSettingDialog()
{
    delete ui;
}
