#include "EditPostDialog.h"
#include "ui_EditPostDialog.h"

#include <QMessageBox>

EditPostDialog::EditPostDialog(int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q), idPost(id),
    ui(new Ui::EditPostDialog)
{
    ui->setupUi(this);

    query->prepare("SELECT position "
                   "FROM positions "
                   "WHERE idposition = :idposition");
    query->bindValue(":idposition", idPost);
    query->exec();
    query->next();
    ui->postEdit->setText(query->value("position").toString());

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

    for(int i = 0; i < accessLevels.size(); i++)
    {
        query->prepare("SELECT access_post.idaccess "
                       "FROM access_post "
                       "WHERE access_post.access = :access AND access_post.post = :post");
        query->bindValue(":access", accessLevels[i].idAccess);
        query->bindValue(":post", idPost);
        query->exec();
        if(query->size() > 0)
            ui->accessList->item(i)->setCheckState(Qt::Checked);
    }
}

EditPostDialog::~EditPostDialog()
{
    delete ui;
}

void EditPostDialog::on_editButton_clicked()
{
    query->prepare("DELETE FROM `access_post` WHERE `post` = :idPost");
    query->bindValue(":idPost", idPost);
    query->exec();

    query->prepare("UPDATE `positions` "
                   "SET `position`= :position "
                   "WHERE `idposition`= :idposition");
    query->bindValue(":position", ui->postEdit->text());
    query->bindValue(":idPost", idPost);
    if(query->exec())
    {
        for(int i = 0; i < ui->accessList->count(); i++)
        {
            if(ui->accessList->item(i)->checkState() == Qt::Checked)
            {
                query->prepare("INSERT INTO `access_post` (`access`, `post`) "
                               "VALUES (:access, :post)");
                query->bindValue(":access", accessLevels[i].idAccess);
                query->bindValue(":post", idPost);
                query->exec();
            }
        }
        close();
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle(tr("Помилка"));
        msg.setText(tr("Помилка оновлення даних"));
        msg.setInformativeText(tr("Неможливо оновити дані"));
        msg.exec();
    }
}

void EditPostDialog::on_fuckButton_clicked()
{
    close();
}
