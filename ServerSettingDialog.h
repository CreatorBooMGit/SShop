#ifndef SERVERSETTINGDIALOG_H
#define SERVERSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class ServerSettingDialog;
}

class ServerSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServerSettingDialog(QWidget *parent = 0);
    ~ServerSettingDialog();

private:
    Ui::ServerSettingDialog *ui;
};

#endif // SERVERSETTINGDIALOG_H
