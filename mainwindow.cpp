#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "addpurchasedialog.h"
#include "addgooddialog.h"
#include "editgooddialog.h"
#include "authdialog.h"
#include "WorkersDialog.h"
#include "ProvidersDialog.h"
#include "qtrpt.h"
#include "PostSettingDialog.h"
#include "ServerSettingDialog.h"
#include "access.h"

#include <QDebug>
#include <QMessageBox>
#include <QDate>
#include <QTime>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QFile settingFile("settingUser.ini");
    if(!settingFile.exists())
    {
        settings = new QSettings("settingUser.ini", QSettings::IniFormat);

        settings->beginGroup("Database");
        settings->setValue("host", "localhost");
        settings->setValue("port", 3306);
        settings->setValue("database", "kursach");
        settings->setValue("username", "root");
        settings->setValue("password", "superpassword37");
        settings->endGroup();

        settings->beginGroup("Size");
        settings->setValue("mainWidth", 800);
        settings->setValue("mainHeight", 600);
        settings->setValue("tablePurchaseWidth", 798);
        settings->setValue("tablePurchaseHeight", 467);
        settings->setValue("tableGoodsWidth", 798);
        settings->setValue("tableGoodsHeight", 467);
        settings->endGroup();
    }
    else
        settings = new QSettings("settingUser.ini", QSettings::IniFormat);

    db = QSqlDatabase::addDatabase("QMYSQL");

    settings->beginGroup("Database");
    db.setHostName(settings->value("host", QString()).toString());
    db.setPort(settings->value("port", QString()).toInt());
    db.setDatabaseName(settings->value("database", QString()).toString());
    db.setUserName(settings->value("username", QString()).toString());
    db.setPassword(settings->value("password", QString()).toString());
    settings->endGroup();

    qDebug() << db.open();
    query = new QSqlQuery(db);
    updateTableGoods();

    //    userInfo.iduser = 1;
    //    userInfo.login = "admin";

    AuthDialog *authDialog = new AuthDialog(&userInfo, query);
    authDialog->exec();
    delete authDialog;

    // ##########ACCESS###############
    access = new Access(userInfo.login, query);

    updateAccess();
    // #########END ACCESS############

    settings->beginGroup("Size");
    resize(settings->value("mainWidth", QString()).toInt(), settings->value("mainHeight", QString()).toInt());
    ui->tablePurchase->resize(settings->value("tablePurchaseWidth", QString()).toInt(), settings->value("tablePurchaseHeight", QString()).toInt());
    ui->tableGoods->resize(settings->value("tableGoodsWidth", QString()).toInt(), settings->value("tableGoodsHeight", QString()).toInt());
    settings->endGroup();

    ui->stackedWidget->setCurrentIndex(0);

    hostStatusLabel = new QLabel(this);
    loginStatusLabel = new QLabel(this);
    dateStatusLabel = new QLabel(this);
    timeStatusLabel = new QLabel(this);

    hostStatusLabel->setMinimumWidth(120);
    hostStatusLabel->setAlignment(Qt::AlignVCenter);
    loginStatusLabel->setMinimumWidth(120);
    loginStatusLabel->setAlignment(Qt::AlignVCenter);
    dateStatusLabel->setMinimumWidth(120);
    dateStatusLabel->setAlignment(Qt::AlignVCenter);
    timeStatusLabel->setMinimumWidth(120);
    timeStatusLabel->setAlignment(Qt::AlignVCenter);

    ui->statusBar->addWidget(hostStatusLabel, 0);
    ui->statusBar->addWidget(loginStatusLabel, 0);
    ui->statusBar->addWidget(dateStatusLabel, 0);
    ui->statusBar->addWidget(timeStatusLabel, 0);

    settings->beginGroup("Database");
    hostStatusLabel->setText("<img width=\"15\" height=\"15\" src=\":/icons/icons/host.ico\"/> " + settings->value("host").toString());
    settings->endGroup();
    setWindowTitle("Магазин іграшок");

    loginStatusLabel->setText("<img width=\"15\" height=\"15\" src=\":/icons/icons/user.ico\"/> " + userInfo.login);

    timeTimer = new QTimer(this);
    connect(timeTimer, SIGNAL(timeout()), this, SLOT(updateTimeStatusBar()));
    timeTimer->start(1000);
    updateTimeStatusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionCheck_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionGoods_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::updateTableGoods()
{
    goodsId.clear();
    query->prepare("SELECT goods.idgood, goods.code, goods.name, providers.name, goods.count, goods.price, goods.pricePDV "
                   "FROM goods, providers "
                   "WHERE goods.idprovider=providers.idprovider");
    qDebug() << query->exec();
    int current = 0;

    ui->tableGoods->setRowCount(query->size());
    while(query->next())
    {
        goodsId.push_back(query->value(0).toInt());
        for(int i = 1; i < 7; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(query->value(i).toString());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableGoods->setItem(current, i - 1, item);
        }
        current++;
    }
}

void MainWindow::checkShow()
{
    query->prepare("SELECT AUTO_INCREMENT "
                   "FROM information_schema.tables "
                   "WHERE table_name = 'checks'");
    query->exec();
    query->next();
    int numberCheck = query->value("AUTO_INCREMENT").toInt();

    QtRPT *check = new QtRPT(this);
    check->recordCount << purchaseList.size();
    check->loadReport(":/rpt/check.xml");

    connect(check, &QtRPT::setValue, [&](const int recNo,
            const QString paramName, QVariant &paramValue,
            const int reportPage) {
        (void) reportPage;
        qDebug() << paramName;
        if(paramName == "code") {
            paramValue = purchaseList[recNo].goodId;
        }
        if(paramName == "name") {
            paramValue = purchaseList[recNo].goodName;
        }
        if(paramName == "provider") {
            paramValue = purchaseList[recNo].goodProvider;
        }
        if(paramName == "count") {
            paramValue = purchaseList[recNo].goodCount;
        }
        if(paramName == "price") {
            paramValue = purchaseList[recNo].goodPrice;
        }
        if(paramName == "pricePDV") {
            paramValue = purchaseList[recNo].goodPricePDV;
        }
        if(paramName == "sum") {
            paramValue = purchaseList[recNo].sum;
        }
        if(paramName == "numberCheck") {
            paramValue = numberCheck;
        }
    });

    check->printExec();
}

void MainWindow::updateAccess()
{
    ui->actionGoods->setVisible(access->checkAccess("goodsEnabled"));
    ui->actionPost->setVisible(access->checkAccess("postsEnabled"));
    ui->actionWorkers->setVisible(access->checkAccess("employeesEnabled"));
    ui->actionProviders->setVisible(access->checkAccess("providerEnabled"));
    ui->menuSettings->setEnabled(access->checkAccess("settingsEnabled"));
    ui->actionSettingServer->setVisible(access->checkAccess("serverSettingEnabled"));
    ui->actionCheck->setVisible(access->checkAccess("purchaseEnabled"));
    if(!access->checkAccess("purchaseEnabled"))
    {
        if(access->checkAccess("goodsEnabled"))
            ui->stackedWidget->setCurrentIndex(2);
        else
            ui->stackedWidget->setCurrentIndex(1);
    }
    ui->addPurchaseButton->setEnabled(access->checkAccess("addPurchaseEnabled"));
    ui->removePurchaseButton->setEnabled(access->checkAccess("removePurchaseEnabled"));
    ui->confirmPurchasesButton->setEnabled(access->checkAccess("confirmPurchaseEnabled"));
    ui->printPurchaseButton->setEnabled(access->checkAccess("printPurchaseEnabled"));

    ui->addGoodButton->setEnabled(access->checkAccess("addGoodEnabled"));
    ui->editGoodButton->setEnabled(access->checkAccess("editGoodEnabled"));
    ui->removeGoodButton->setEnabled(access->checkAccess("removeGoodEnabled"));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    settings->beginGroup("Size");
    settings->setValue("mainWidth", width());
    settings->setValue("mainHeight", height());
    settings->setValue("tablePurchaseWidth", ui->tablePurchase->width());
    settings->setValue("tablePurchaseHeight", ui->tablePurchase->height());
    settings->setValue("tableGoodsWidth", ui->tableGoods->width());
    settings->setValue("tableGoodsHeight", ui->tableGoods->height());
    settings->endGroup();

    for(int i = 0; i < ui->tablePurchase->columnCount(); i++)
        ui->tablePurchase->setColumnWidth(i, ui->tablePurchase->width() / ui->tablePurchase->columnCount());

    for(int i = 0; i < ui->tableGoods->columnCount(); i++)
        ui->tableGoods->setColumnWidth(i, ui->tableGoods->width() / ui->tableGoods->columnCount());
}

void MainWindow::on_addPurchaseButton_clicked()
{
    AddPurchaseDialog *addPurchaseDialog = new AddPurchaseDialog(query, this);
    connect(addPurchaseDialog, SIGNAL(sendPurchase(purchase)), this, SLOT(get_Purchase(purchase)));
    addPurchaseDialog->exec();
    delete addPurchaseDialog;

    if(ui->tablePurchase->rowCount())
    {
        ui->printPurchaseButton->setEnabled(true);
        ui->confirmPurchasesButton->setEnabled(true);
    }
}

void MainWindow::on_removePurchaseButton_clicked()
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
        purchaseList.remove(ui->tablePurchase->currentRow());
        ui->tablePurchase->removeRow(ui->tablePurchase->currentRow());
    }

    if(ui->tablePurchase->rowCount())
    {
        ui->printPurchaseButton->setEnabled(true);
        ui->confirmPurchasesButton->setEnabled(true);
    }
}

void MainWindow::on_addGoodButton_clicked()
{
    AddGoodDialog *addDialog = new AddGoodDialog(query, this);
    addDialog->exec();
    delete addDialog;
    updateTableGoods();
}

void MainWindow::on_editGoodButton_clicked()
{
    EditGoodDialog *editDialog = new EditGoodDialog(goodsId[ui->tableGoods->currentRow()], query, this);
    editDialog->exec();
    delete editDialog;
    updateTableGoods();
}

void MainWindow::on_tableGoods_itemSelectionChanged()
{
    ui->editGoodButton->setEnabled(true);
    ui->removeGoodButton->setEnabled(true);
}

void MainWindow::on_removeGoodButton_clicked()
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
        query->prepare("DELETE FROM `goods` "
                       "WHERE `idgood`= :idgood");
        query->bindValue(":idgood", goodsId[ui->tableGoods->currentRow()]);
        query->exec();
        updateTableGoods();
    }
}

void MainWindow::get_Purchase(purchase tmp)
{
    purchaseList.push_back(tmp);
//    query->prepare("SELECT goods.code, providers.name, goods.name, goods.price "
//                   "FROM goods, providers "
//                   "WHERE goods.idprovider = providers.idprovider AND goods.idgood = :idgood");
//    query->bindValue(":idgood", tmp.goodId);
//    query->exec();
//    query->next();

    QTableWidgetItem *itemCode = new QTableWidgetItem(QString::number(tmp.goodCode));
    QTableWidgetItem *itemName = new QTableWidgetItem(tmp.goodName);
    QTableWidgetItem *itemProvider = new QTableWidgetItem(tmp.goodProvider);
    QTableWidgetItem *itemCount = new QTableWidgetItem(QString::number(tmp.goodCount));
    QTableWidgetItem *itemPrice = new QTableWidgetItem(QString::number(tmp.goodPrice));
    QTableWidgetItem *itemPricePDV = new QTableWidgetItem(QString::number(tmp.goodPricePDV));
    QTableWidgetItem *itemSum = new QTableWidgetItem(QString::number(tmp.sum));

    itemCode->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    itemName->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    itemProvider->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    itemCount->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    itemPrice->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    itemPricePDV->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    itemSum->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    ui->tablePurchase->setRowCount(ui->tablePurchase->rowCount() + 1);
    ui->tablePurchase->setItem(ui->tablePurchase->rowCount() - 1, 0,  itemCode);
    ui->tablePurchase->setItem(ui->tablePurchase->rowCount() - 1, 1,  itemName);
    ui->tablePurchase->setItem(ui->tablePurchase->rowCount() - 1, 2,  itemProvider);
    ui->tablePurchase->setItem(ui->tablePurchase->rowCount() - 1, 3,  itemCount);
    ui->tablePurchase->setItem(ui->tablePurchase->rowCount() - 1, 4,  itemPrice);
    ui->tablePurchase->setItem(ui->tablePurchase->rowCount() - 1, 5,  itemPricePDV);
    ui->tablePurchase->setItem(ui->tablePurchase->rowCount() - 1, 6,  itemSum);
}

void MainWindow::on_tablePurchase_itemSelectionChanged()
{
    ui->removePurchaseButton->setEnabled(true);
}

void MainWindow::on_confirmPurchasesButton_clicked()
{
    double sum = 0;
    for(int i = 0; i < purchaseList.size(); i++)
        sum += purchaseList[i].sum;

    query->prepare("INSERT INTO `checks` (`worker`, `client`, `sum`, `date`, `time`) "
                   "VALUES (:worker, :client, :sum, :date, :time)");
    query->bindValue(":worker", userInfo.iduser);
    query->bindValue(":client", 1);
    query->bindValue(":sum", sum);
    query->bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));
    query->bindValue(":time", QTime::currentTime().toString("HH:mm:ss"));
    if(query->exec())
    {
        int currentIndex = query->lastInsertId().toInt();
        for(int i = 0; i < purchaseList.size(); i++)
        {
            query->prepare("INSERT INTO `operations` (`check`, `good`, `count`, `sum`) "
                           "VALUES (:check, :good, :count, :sum)");
            query->bindValue(":check", currentIndex);
            query->bindValue(":good", purchaseList[i].goodId);
            query->bindValue(":count", purchaseList[i].goodCount);
            query->bindValue(":sum", purchaseList[i].sum);
            query->exec();
        }
    }
    purchaseList.clear();
    ui->tablePurchase->clearContents();
    ui->tablePurchase->setRowCount(0);
}

void MainWindow::on_actionLogout_triggered()
{
    AuthDialog *authDialog = new AuthDialog(&userInfo, query);
    authDialog->exec();
    delete authDialog;
    updateAccess();
    loginStatusLabel->setText("<img width=\"15\" height=\"15\" src=\":/icons/icons/user.ico\"/> " + userInfo.login);
}

void MainWindow::on_actionWorkers_triggered()
{
    WorkersDialog *workersDialog = new WorkersDialog(query, this);
    workersDialog->exec();
    delete workersDialog;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionProviders_triggered()
{
    ProvidersDialog *providersDialog = new ProvidersDialog(query, this);
    providersDialog->exec();
    delete providersDialog;
}

void MainWindow::on_printPurchaseButton_clicked()
{
    checkShow();
}

void MainWindow::updateTimeStatusBar()
{
    timeStatusLabel->setText("<img width=\"15\" height=\"15\" src=\":/icons/icons/calendar.ico\"/> " + QTime::currentTime().toString("HH:mm:ss"));
    dateStatusLabel->setText("<img width=\"15\" height=\"15\" src=\":/icons/icons/clock.ico\"/> " + QDate::currentDate().toString("dd.MM.yyyy"));
}

void MainWindow::on_actionPost_triggered()
{
    PostSettingDialog *postDialog = new PostSettingDialog(access, query, this);
    postDialog->exec();
    delete postDialog;
    updateAccess();
}

void MainWindow::on_actionSettingServer_triggered()
{
    ServerSettingDialog *serverDialog = new ServerSettingDialog(this);
    serverDialog->exec();
    delete serverDialog;
}

void MainWindow::on_tablePurchase_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(ui->actionAddPost);
    contextTableMenu.addAction(ui->actionEditPost);
    contextTableMenu.addAction(ui->actionRemovePost);
    contextTableMenu.addSeparator();
    contextTableMenu.addAction(ui->actionUpdatePosts);

    QPoint globalPos = ui->tableWidget->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}

void MainWindow::on_tableGoods_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(ui->actionAddPost);
    contextTableMenu.addAction(ui->actionEditPost);
    contextTableMenu.addAction(ui->actionRemovePost);
    contextTableMenu.addSeparator();
    contextTableMenu.addAction(ui->actionUpdatePosts);

    QPoint globalPos = ui->tableWidget->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}
