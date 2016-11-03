#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSettings>
#include <QLabel>
#include <QTimer>
#include "access.h"
#include "usertypes.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionCheck_triggered();
    void on_actionGoods_triggered();
    void on_addPurchaseButton_clicked();
    void on_removePurchaseButton_clicked();
    void on_addGoodButton_clicked();
    void on_editGoodButton_clicked();
    void on_tableGoods_itemSelectionChanged();
    void on_removeGoodButton_clicked();
    void get_Purchase(purchase tmp);
    void on_tablePurchase_itemSelectionChanged();
    void on_confirmPurchasesButton_clicked();
    void on_actionLogout_triggered();
    void on_actionWorkers_triggered();
    void on_actionExit_triggered();
    void on_actionProviders_triggered();
    void on_printPurchaseButton_clicked();
    void updateTimeStatusBar();
    void on_actionPost_triggered();

    void on_actionSettingServer_triggered();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQuery *query;
    QSettings *settings;

    user userInfo;

    QLabel *hostStatusLabel;
    QLabel *loginStatusLabel;
    QLabel *dateStatusLabel;
    QLabel *timeStatusLabel;

    QTimer *timeTimer;

    QVector <int> goodsId;
    QVector <purchase> purchaseList;

    Access *access;

    void updateTableGoods();
    void checkShow();
    void updateAccess();

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event);
};

#endif // MAINWINDOW_H
