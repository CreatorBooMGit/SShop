#ifndef ADDPOSTDIALOG_H
#define ADDPOSTDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QVector>

namespace Ui {
class AddPostDialog;
}

class AddPostDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPostDialog(QSqlQuery *q, QWidget *parent = 0);
    ~AddPostDialog();

private slots:
    void on_OKButton_clicked();
    void on_fuckButton_clicked();

private:
    Ui::AddPostDialog *ui;
    QSqlQuery *query;

    struct access {
        int idAccess;
        QString name;
        QString description;
    };

    QVector <access> accessLevels;
};

#endif // ADDPOSTDIALOG_H
