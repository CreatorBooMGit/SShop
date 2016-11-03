#ifndef PROVIDERSDIALOG_H
#define PROVIDERSDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class ProvidersDialog;
}

class ProvidersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProvidersDialog(QSqlQuery *q, QWidget *parent = 0);
    ~ProvidersDialog();

private slots:
    void on_addButton_clicked();

    void on_editButton_clicked();

    void on_removeButton_clicked();

    void on_tableProviders_itemSelectionChanged();

private:
    Ui::ProvidersDialog *ui;
    QSqlQuery *query;
    QVector <int> Providers;
    void updateProviders();

};

#endif // PROVIDERSDIALOG_H
