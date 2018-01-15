#include "maincontroller.h"
#include "commonfunction.h"
#include "configsetting.h"
#include "blemanager.h"
#include <QDebug>
#include <QUrl>
#include <QThread>
#include <QMessageBox>
#include <QtConcurrent>
#include <QDateTime>
#include <QGuiApplication>

MainController::MainController(QObject *parent) :
    QObject(parent)
  ,m_keepwake(Q_NULLPTR)
{
    g_total_amount = 0;
    setting = new ConfigSetting(this);
    bleMgr = new BLEmanager(this);
    connect(bleMgr,SIGNAL(getTotalAmount(QString)),this,SLOT(getTotalAmount(QString)));
    connect(bleMgr,SIGNAL(device_connect(bool)),this,SLOT(get_device_connect(bool)));
    m_keepwake = new KeepWake();
    g_mac_adress = this->getMacAddress();
    bleMgr->connect_to_device(g_mac_adress);
}

MainController::~MainController()
{
}

// onoff=true: turn screen on; onoff=false: turn screen off (saver)
void MainController::screenSave(bool onoff)
{
    m_keepwake->onChange(onoff);
}

bool MainController::isDebugmode()
{
#ifdef QT_DEBUG
    return true;
#endif
    return false;
}

bool MainController::isAndroid()
{
#ifdef Q_OS_ANDROID
    return true;
#endif
    return false;
}

void MainController::appQuit()
{
    qApp->quit();
}

QString MainController::appBuildDate()
{
    return QString::fromUtf8("%1 - %2").arg(__DATE__).arg(__TIME__);
}

QString MainController::formatMoney(long long moneyValue)
{
    long long absVal = abs(moneyValue);
    QString money = QString::number(absVal);
    int loop = money.length() / 3;
    if (loop > 0)
    {
        if (money.length() % 3 == 0)
        {
            loop = loop - 1;
        }

        int index = money.length();
        for (int i = 0; i < loop ; i++)
        {
            index = index - 3;
            money.insert(index,  QString(","));
        }
    }

    if (moneyValue < 0)
        money.insert(0,  QString("-"));

    return money;
}

long long MainController::getMoneyValue(const  QString &moneyString)
{
    QString money = moneyString;
    money = money.replace(QString(","),QString::null);
    if (money.isEmpty())
    {
        return 0;
    }
    else
    {
        return money.toLongLong();
    }
}

bool MainController::isConnected() {
    return bleMgr->isConnected();
}

QString MainController::getMacAddress()
{
     return setting->mac_address;
}

void MainController::setMacAddress(const QString &mac_address)
{
    setting->setMacAddress(mac_address);
    g_mac_adress = this->getMacAddress();
}

void MainController::connect_to_device() {
    qDebug() << "MainController::connect_to_device(), mac address: " + g_mac_adress;
    bleMgr->connect_to_device(g_mac_adress);
}

void MainController::getTotalAmount(const QString &total_amount) {
    qDebug() << "MainController::getTotalAmount(), total_amount: " << total_amount;
    long long amount = total_amount.toLongLong();
    g_total_amount += amount;
    emit view_tatal_money(g_total_amount);
}

void MainController::get_device_connect(bool _status) {
    emit device_connect(_status);
}

void MainController::tranferMoneyByPhone(const QString &phone_number, const QString &amount, const QString &transfer_type)
{
    HttpBase *http_transferMoney = new HttpBase(QString(""), this);
    QObject::connect(http_transferMoney, SIGNAL(done(QVariant)), this, SLOT(get_transferinfo_by_phone(QVariant)), Qt::UniqueConnection);
    QObject::connect(http_transferMoney, SIGNAL(done(QVariant)),     http_transferMoney, SLOT(deleteLater()), Qt::UniqueConnection);
    QObject::connect(http_transferMoney, SIGNAL(error(int,QString)), http_transferMoney, SLOT(deleteLater()), Qt::UniqueConnection);
    http_transferMoney->setUrl(QUrl(QString("%1%2").arg(SL_SERVERURL).arg(PATH_CAHSIN)));
    http_transferMoney->addParameter("cm", CM_TRANSFERCASHIN_BY_TYPE, true);
    http_transferMoney->addParameter("dt", Cmnfunc::formatTransferByPhone(phone_number, amount, transfer_type));
    http_transferMoney->process();
}

void MainController::tranferMoneyByZaloPayId(const QString &zpid, const QString &amount, const QString &transfer_type) {
    HttpBase *http_transferMoney= new HttpBase(QString(""), this);
    QObject::connect(http_transferMoney, SIGNAL(done(QVariant)), this, SLOT(get_transferinfo_by_zpid(QVariant)), Qt::UniqueConnection);
    QObject::connect(http_transferMoney, SIGNAL(done(QVariant)),     http_transferMoney, SLOT(deleteLater()), Qt::UniqueConnection);
    QObject::connect(http_transferMoney, SIGNAL(error(int,QString)), http_transferMoney, SLOT(deleteLater()), Qt::UniqueConnection);
    http_transferMoney->setUrl(QUrl(QString("%1%2").arg(SL_SERVERURL).arg(PATH_CAHSIN)));
    http_transferMoney->addParameter("cm", CM_TRANSFERCASHIN_BY_ZPID, true);
    http_transferMoney->addParameter("dt", Cmnfunc::formatTransferByZaloPayID(zpid, amount, transfer_type));
    http_transferMoney->process();
}

void MainController::getZaloPayUserInfoByPhone(const QString &phone_number) {
    HttpBase *http_getuserInfo = new HttpBase(QString(""), this);
    QObject::connect(http_getuserInfo, SIGNAL(done(QVariant)), this, SLOT(get_userinfo_by_phone(QVariant)), Qt::UniqueConnection);
    QObject::connect(http_getuserInfo, SIGNAL(done(QVariant)),     http_getuserInfo, SLOT(deleteLater()), Qt::UniqueConnection);
    QObject::connect(http_getuserInfo, SIGNAL(error(int,QString)), http_getuserInfo, SLOT(deleteLater()), Qt::UniqueConnection);
    http_getuserInfo->setUrl(QUrl(QString("%1%2").arg(SL_SERVERURL).arg(PATH_ZPUSERINFO)));
    http_getuserInfo->addParameter("cm", CM_GET_USER_BY_PHONE, true);
    http_getuserInfo->addParameter("dt", Cmnfunc::formatZPUserInfoByPhone(phone_number));
    http_getuserInfo->process();
}

void MainController::getZaloPayUserInfoByZpid(const QString &zpid) {
    HttpBase *http_getuserInfo = new HttpBase(QString(""), this);
    QObject::connect(http_getuserInfo, SIGNAL(done(QVariant)), this, SLOT(get_userinfo_by_zpid(QVariant)), Qt::UniqueConnection);
    QObject::connect(http_getuserInfo, SIGNAL(done(QVariant)),     http_getuserInfo, SLOT(deleteLater()), Qt::UniqueConnection);
    QObject::connect(http_getuserInfo, SIGNAL(error(int,QString)), http_getuserInfo, SLOT(deleteLater()), Qt::UniqueConnection);
    http_getuserInfo->setUrl(QUrl(QString("%1%2").arg(SL_SERVERURL).arg(PATH_ZPUSERINFO)));
    http_getuserInfo->addParameter("cm", CM_GET_USER_BY_ZPID, true);
    http_getuserInfo->addParameter("dt", Cmnfunc::formatZPUserInfoByZaloPayID(zpid));
    http_getuserInfo->process();
}

void MainController::get_userinfo_by_phone(QVariant data) {
    qDebug() << "MainController::get_userinfo_by_phone: " << data.toString();
    emit doneUserInfoByPhone(data.toString());
}

void MainController::get_userinfo_by_zpid(QVariant data) {
    qDebug() << "MainController::get_userinfo_by_zpid: " << data.toString();
    emit doneUserInfoByZpID(data.toString());
}

void MainController::get_transferinfo_by_phone(QVariant data) {
    qDebug() << "MainController::get_transferinfo_by_phone: " << data.toString();
    emit doneTransferMoneyByPhone(data.toString());
}

void MainController::get_transferinfo_by_zpid(QVariant data) {
    qDebug() << "MainController::get_transferinfo_by_zpid: " << data.toString();
    emit doneTransferMoneyByZpID(data.toString());
}

void MainController::resetMoney() {
    g_total_amount = 0;
    emit view_tatal_money(g_total_amount);
}

long long MainController::getCurrMoney() {
    return g_total_amount;
}

bool MainController::get_pong_rcv() {
    return bleMgr->get_pong_rcv();
}



