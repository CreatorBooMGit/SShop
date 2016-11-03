#ifndef ADDWORKERDIALOG_H
#define ADDWORKERDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class AddWorkerDialog;
}

class AddWorkerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddWorkerDialog(QSqlQuery *q, QWidget *parent = 0);
    ~AddWorkerDialog();

private slots:
    void on_OKButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::AddWorkerDialog *ui;
    QSqlQuery *query;
    QVector <int> positionID;
};

#endif // ADDWORKERDIALOG_H
