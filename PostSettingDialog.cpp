#include "PostSettingDialog.h"
#include "ui_PostSettingDialog.h"
#include "AddPostDialog.h"
#include "EditPostDialog.h"

#include <QMessageBox>

PostSettingDialog::PostSettingDialog(QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::PostSettingDialog)
{
    ui->setupUi(this);

    updatePosts();
}

PostSettingDialog::~PostSettingDialog()
{
    delete ui;
}

void PostSettingDialog::on_addButton_clicked()
{
    AddPostDialog *addDialog = new AddPostDialog(query, this);
    addDialog->exec();
    delete addDialog;
    updatePosts();
}

void PostSettingDialog::on_editButton_clicked()
{
    EditPostDialog *editDialog = new EditPostDialog(posts[ui->tableWidget->currentRow()].idpost, query, this);
    editDialog->exec();
    delete editDialog;
    updatePosts();
}

void PostSettingDialog::on_removeButton_clicked()
{
    QMessageBox confirmMsg;
    confirmMsg.setWindowTitle(tr("Видалення посади"));
    confirmMsg.setText(tr("Підтвердити видалення?"));
    QAbstractButton *confirmMsgButtonYes = confirmMsg.addButton(tr("Підтвердити"), QMessageBox::YesRole);
    QAbstractButton *confirmMsgButtonNo = confirmMsg.addButton(tr("Відміна"), QMessageBox::NoRole);
    confirmMsg.exec();
    if(confirmMsg.clickedButton() == confirmMsgButtonYes)
    {
        query->prepare("DELETE FROM `positions` "
                       "WHERE `idposition` = :idposition");
        query->bindValue(":idposition", posts[ui->tableWidget->currentRow()].idpost);
        query->exec();
        updatePosts();
    }
}

void PostSettingDialog::updatePosts()
{
    posts.clear();
    query->prepare("SELECT * "
                   "FROM positions");
    query->exec();
    ui->tableWidget->setRowCount(query->size());
    while(query->next())
    {
        post tmp;
        tmp.idpost = query->value("idposition").toInt();
        tmp.name = query->value("position").toString();
        posts.push_back(tmp);
        QTableWidgetItem *item = new QTableWidgetItem(tmp.name);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui->tableWidget->setItem(posts.size() - 1, 0, item);
    }

    for(int i = 0; i < posts.size(); i++)
    {
        query->prepare("SELECT access.description "
                       "FROM access, access_post "
                       "WHERE access.idaccess = access_post.access AND access_post.post = :post");
        query->bindValue(":post", posts[i].idpost);
        query->exec();
        QString descriptions;
        while(query->next())
        {
            descriptions.push_back(query->value("description").toString());
            descriptions.push_back(", ");
        }
        if(descriptions.size() > 0)
            descriptions.remove(descriptions.size() - 2, 2);

        QTableWidgetItem *item = new QTableWidgetItem(descriptions);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui->tableWidget->setItem(i, 1, item);
    }
}
// Апаратная часть компьтера
