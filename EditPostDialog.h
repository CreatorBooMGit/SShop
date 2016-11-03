#ifndef EditPostDialog_H
#define EditPostDialog_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class EditPostDialog;
}

class EditPostDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditPostDialog(int id, QSqlQuery *q, QWidget *parent = 0);
    ~EditPostDialog();

private slots:
    void on_editButton_clicked();

    void on_fuckButton_clicked();

private:
    Ui::EditPostDialog *ui;
    QSqlQuery *query;
    int idPost;

    struct access {
        int idAccess;
        QString name;
        QString description;
    };

    QVector <access> accessLevels;
};

#endif // EditPostDialog_H
