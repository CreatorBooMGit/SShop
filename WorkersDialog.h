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

    void on_actionAddWorker_triggered();

    void on_actionEditWorker_triggered();

    void on_actionRemoveWorker_triggered();

    void on_actionUpdateWorkers_triggered();

    void on_tableWorkers_customContextMenuRequested(const QPoint &pos);

private:
    Ui::WorkersDialog *ui;
    QSqlQuery *query;

    QVector <int> workersId;

    void updateWorkers();
};

#endif // WORKERSDIALOG_H
