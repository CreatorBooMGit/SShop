#ifndef POSTSETTINGDIALOG_H
#define POSTSETTINGDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class PostSettingDialog;
}

class PostSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PostSettingDialog(QSqlQuery *q, QWidget *parent = 0);
    ~PostSettingDialog();

private slots:
    void on_addButton_clicked();

    void on_editButton_clicked();

    void on_removeButton_clicked();

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
