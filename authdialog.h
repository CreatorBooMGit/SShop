#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include "usertypes.h"

namespace Ui {
class AuthDialog;
}

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(user *u, QSqlQuery *q, QWidget *parent = 0);
    ~AuthDialog();

private slots:
    void on_cancelButton_clicked();
    void on_confirmButton_clicked();

    void on_loginEdit_returnPressed();

    void on_passwordEdit_returnPressed();

private:
    Ui::AuthDialog *ui;
    QSqlQuery *query;
    user *userInfo;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // AUTHDIALOG_H
