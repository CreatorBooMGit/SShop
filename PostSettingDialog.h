#ifndef POSTSETTINGDIALOG_H
#define POSTSETTINGDIALOG_H

#include <QDialog>
#include <QSqlQuery>

#include "access.h"

namespace Ui {
class PostSettingDialog;
}

class PostSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PostSettingDialog(Access *a, QSqlQuery *q, QWidget *parent = 0);
    ~PostSettingDialog();

private slots:
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_removeButton_clicked();

    void on_actionAddPost_triggered();

    void on_actionEditPost_triggered();

    void on_actionRemovePost_triggered();

    void on_actionUpdatePosts_triggered();

    void on_tableWidget_itemSelectionChanged();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::PostSettingDialog *ui;
    QSqlQuery *query;

    struct post {
        int idpost;
        QString name;
    };

    QVector <post> posts;

    void updatePosts();
};

#endif // POSTSETTINGDIALOG_H
