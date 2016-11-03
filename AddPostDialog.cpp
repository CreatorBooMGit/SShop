#include "AddPostDialog.h"
#include "ui_AddPostDialog.h"

#include <QMessageBox>

AddPostDialog::AddPostDialog(QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::AddPostDialog)
{
    ui->setupUi(this);
    query->prepare("SELECT * "
                   "FROM access");
    query->exec();
    while(query->next())
    {
        access tmp;
        tmp.idAccess = query->value("idaccess").toInt();
        tmp.name = query->value("name").toString();
        tmp.description = query->value("description").toString();
        accessLevels.push_back(tmp);
        QListWidgetItem *item = new QListWidgetItem(query->value("description").toString());
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setCheckState(Qt::Unchecked);
        item->setToolTip(tmp.name);
        ui->accessList->addItem(item);
    }
}

AddPostDialog::~AddPostDialog()
{
    delete ui;
}

void AddPostDialog::on_OKButton_clicked()
{
    query->prepare("INSERT INTO `positions` (`position`) "
                   "VALUES (:position)");
    query->bindValue(":position", ui->postEdit->text());
    if(query->exec())
    {
        int idpost = query->lastInsertId().toInt();
        for(int i = 0; i < ui->accessList->count(); i++)
        {
            if(ui->accessList->item(i)->checkState() == Qt::Checked)
            {
                query->prepare("INSERT INTO `access_post` (`access`, `post`) VALUES (:access, :post)");
                query->bindValue(":access", accessLevels[i].idAccess);
                query->bindValue(":post", idpost);
                query->exec();
            }
        }
        close();
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle(tr("Помилка"));
        msg.setText(tr("Помилка додавання даних"));
        msg.setInformativeText(tr("Дана посада вже існує"));
        msg.exec();
    }
}

void AddPostDialog::on_fuckButton_clicked()
{
    close();
}
