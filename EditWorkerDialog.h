#ifndef EDITWORKERDIALOG_H
#define EDITWORKERDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class EditWorkerDialog;
}

class EditWorkerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditWorkerDialog(int id, QSqlQuery *q, QWidget *parent = 0);
    ~EditWorkerDialog();

private slots:
    void on_OKButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::EditWorkerDialog *ui;
    QSqlQuery *query;
    int workerId;

    QVector <int> positionId;
};

#endif // EDITWORKERDIALOG_H
