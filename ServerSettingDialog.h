#ifndef SERVERSETTINGDIALOG_H
#define SERVERSETTINGDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class ServerSettingDialog;
}

class ServerSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServerSettingDialog(QWidget *parent = 0);
    ~ServerSettingDialog();

private slots:
    void on_editButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::ServerSettingDialog *ui;
    QSettings *settings;
};

#endif // SERVERSETTINGDIALOG_H
