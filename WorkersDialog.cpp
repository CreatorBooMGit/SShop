#include "AddWorkerDialog.h"
#include "EditWorkerDialog.h"
#include "WorkersDialog.h"
#include "ui_WorkersDialog.h"

#include <QMenu>
#include <qmessagebox.h>

WorkersDialog::WorkersDialog(QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::WorkersDialog)
{
    ui->setupUi(this);
    updateWorkers();
}

WorkersDialog::~WorkersDialog()
{
    delete ui;
}

void WorkersDialog::updateWorkers()
{
    query->prepare("SELECT workers.idworker, workers.name, workers.surname, workers.phone, positions.position, workers.login "
                   "FROM workers, positions "
                   "where workers.idposition = positions.idposition");
    query->exec();
    int current = 0;
    ui->tableWorkers->setRowCount(query->size());
    while(query->next())
    {
        workersId.push_back(query->value(0).toInt());
        for(int i = 1; i < 6; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(query->value(i).toString());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWorkers->setItem(current, i - 1, item);
        }
        current++;
    }
}

void WorkersDialog::on_addButton_clicked()
{
    AddWorkerDialog *addDialog = new AddWorkerDialog(query, this);
    addDialog->exec();
    delete addDialog;
    updateWorkers();
}

void WorkersDialog::on_removeButton_clicked()
{
    QMessageBox confirmMsg;
    confirmMsg.setWindowTitle(tr("Видалення товару"));
    confirmMsg.setText(tr("Підтвердити видалення?"));
    QAbstractButton *confirmMsgButtonYes = confirmMsg.addButton(tr("Підтвердити"), QMessageBox::YesRole);
    QAbstractButton *confirmMsgButtonNo = confirmMsg.addButton(tr("Відміна"), QMessageBox::NoRole);
//    confirmMsg.setDefaultButton(confirmMsgButtonNo);
    confirmMsg.exec();
    if(confirmMsg.clickedButton() == confirmMsgButtonYes)
    {
        query->prepare("DELETE FROM `workers` "
                       "WHERE `idworker`= :idworker");
        query->bindValue(":idworker", workersId[ui->tableWorkers->currentRow()]);
        query->exec();
        updateWorkers();
    }
}

void WorkersDialog::on_editButton_clicked()
{
    EditWorkerDialog *editDialog = new EditWorkerDialog(workersId[ui->tableWorkers->currentRow()], query, this);
    editDialog->exec();
    delete editDialog;
    updateWorkers();
}

void WorkersDialog::on_tableWorkers_itemSelectionChanged()
{
    ui->editButton->setEnabled(true);
    ui->removeButton->setEnabled(true);
}

void WorkersDialog::on_actionAddWorker_triggered()
{
    on_addButton_clicked();
}

void WorkersDialog::on_actionEditWorker_triggered()
{
    on_editButton_clicked();
}

void WorkersDialog::on_actionRemoveWorker_triggered()
{
    on_removeButton_clicked();
}

void WorkersDialog::on_actionUpdateWorkers_triggered()
{
    updateWorkers();
}

void WorkersDialog::on_tableWorkers_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(ui->actionAddWorker);
    contextTableMenu.addAction(ui->actionEditWorker);
    contextTableMenu.addAction(ui->actionRemoveWorker);
    contextTableMenu.addSeparator();
    contextTableMenu.addAction(ui->actionUpdateWorkers);

    QPoint globalPos = ui->tableWorkers->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}
