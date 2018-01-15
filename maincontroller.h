#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include "./network/httpbase.h"
#include "./network/httpbase2.h"
#include "./network/wsclient.h"
#include "datatypes.h"
#include "keepwake.h"

#define SL_SERVERURL                "https://gbcstaging.zing.vn"
#define PATH_CAHSIN                 "/v001/mctransfer/api/cashin/?"
#define PATH_ZPUSERINFO             "/v001/mctransfer/api/zpuserinfo/?"

#define CM_GET_USER_BY_PHONE        "getzalopayuserinfobyphone"
#define CM_GET_USER_BY_ZPID         "getzalopayuserinfobyzpid"
#define CM_TRANSFERCASHIN_BY_ZPID   "transfercash"
#define CM_TRANSFERCASHIN_BY_TYPE   "transfercashtype"

class ConfigSetting;
class BLEmanager;
class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = 0);
    ~MainController();

    Q_INVOKABLE void screenSave(bool onoff);
    Q_INVOKABLE bool isDebugmode();
    Q_INVOKABLE bool isAndroid();
    Q_INVOKABLE void    appQuit();
    Q_INVOKABLE QString appBuildDate();
    Q_INVOKABLE QString getMacAddress();
    Q_INVOKABLE void connect_to_device();
    Q_INVOKABLE void setMacAddress(const QString &mac_address);
    Q_INVOKABLE QString formatMoney(long long moneyValue);
    Q_INVOKABLE long long getMoneyValue(const  QString &moneyString);
    Q_INVOKABLE bool isConnected();
    Q_INVOKABLE void tranferMoneyByZaloPayId(const QString &zpid, const QString &amount, const QString &transfer_type);
    Q_INVOKABLE void tranferMoneyByPhone(const QString &zpid, const QString &amount, const QString &transfer_type);
    Q_INVOKABLE void getZaloPayUserInfoByPhone(const QString &phone_number);
    Q_INVOKABLE void getZaloPayUserInfoByZpid(const QString &zpid);
    Q_INVOKABLE void resetMoney();
    Q_INVOKABLE long long getCurrMoney();
    Q_INVOKABLE bool get_pong_rcv();
signals:
    void view_tatal_money(long long total_amount);
    void device_connect(bool _status);
    void doneUserInfoByPhone(const QString &data);
    void doneUserInfoByZpID(const QString &data);
    void doneTransferMoneyByPhone(const QString &data);
    void doneTransferMoneyByZpID(const QString &data);
public slots:
    void getTotalAmount(const QString &total_amount);
    void get_device_connect(bool _status);
    void get_userinfo_by_phone(QVariant data);
    void get_userinfo_by_zpid(QVariant data);
    void get_transferinfo_by_phone(QVariant data);
    void get_transferinfo_by_zpid(QVariant data);
private:
    KeepWake        *m_keepwake;
    ConfigSetting   *setting;
    BLEmanager      *bleMgr;
    long long       g_total_amount;
    QString         g_mac_adress;
};

#endif // MAINCONTROLLER_H
