#ifndef WORKERSDIALOG_H
#define WORKERSDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class WorkersDialog;
}

class WorkersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorkersDialog(QSqlQuery *q, QWidget *parent = 0);
    ~WorkersDialog();

private slots:
    void on_addButton_clicked();

    void on_removeButton_clicked();

    void on_editButton_clicked();

    void on_tableWorkers_itemSelectionChanged();

private:
    Ui::WorkersDialog *ui;
    QSqlQuery *query;

    QVector <int> workersId;

    void updateWorkers();
};

#endif // WORKERSDIALOG_H
